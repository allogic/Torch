#include "TorchCharacter.h"
#include "DrawDebugHelpers.h"
#include "TorchAIController.h"
#include "TorchBlueprintUtils.h"
#include "TorchMath.h"
#include "TorchCore.h"
#include "TorchTPSCharacter.h"
#include "TorchPathFinding.h"
#include "TorchPathSharing.h"
#include "TorchAnimationInstance.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"

ATorchCharacter::ATorchCharacter(const FObjectInitializer& initializer)
{
  // Setup actor
  PrimaryActorTick.bCanEverTick = true;

  // Setup physical animtion
  mPhysicalAnimationComponent = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("PhysicalAnimation"));
  mPhysicalAnimationComponent->SetStrengthMultiplyer(1.0f);

  // Setup perception
  mPerceptionSphere = CreateDefaultSubobject<USphereComponent>("PerceptionSphere");
  mPerceptionSphere->SetupAttachment(RootComponent);
  mPerceptionSphere->SetSphereRadius(mPerceptionSphereRadius);
  mPerceptionSphere->SetCollisionProfileName(TEXT("Custom"));
  mPerceptionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  mPerceptionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
  mPerceptionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATorchCharacter::OnBeginOverlap);
  mPerceptionSphere->OnComponentEndOverlap.AddDynamic(this, &ATorchCharacter::OnEndOverlap);

  // Setup capsule
  GetCapsuleComponent()->SetCollisionProfileName(TEXT("Custom"));
  GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);

  // Setup skeletal mesh
  GetMesh()->SetCollisionProfileName(TEXT("Custom"));
  GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
  GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

  // Setup timer delegates
  mTimerDelegateFindPath.BindUFunction(this, TEXT("UpdatePath"));
}

void ATorchCharacter::BeginPlay()
{
  Super::BeginPlay();

  // Configure custom animation instance
  mAnimationInstance = Cast<UTorchAnimationInstance>(GetMesh()->GetAnimInstance());
  mAnimationInstance->RootMotionMode = ERootMotionMode::RootMotionFromEverything;

  // Setup pathfinding
  GetWorldTimerManager().SetTimer(mTimerHandleFindPath, mTimerDelegateFindPath, 1.0f, true, 1.0f);
}
void ATorchCharacter::Tick(float deltaTime)
{
  Super::Tick(deltaTime);

  // Path following
  if (mFollowPath)
  {
    FollowPath();
  }

  // Check if can see player
  if (mPlayer)
  {
    FHitResult hitResult;
    FCollisionQueryParams queryParams;
    queryParams.AddIgnoredActor(this);
    mCanSeePlayer = !GetWorld()->LineTraceSingleByChannel(hitResult, GetActorLocation(), mPlayer->GetActorLocation(), ECollisionChannel::ECC_Visibility, queryParams);
  }

  Debug();
}

void ATorchCharacter::UpdatePath()
{
  // Path separation when close to each other
  //if (mEnemies.Num() > 0)
  //{
  //  // Compute escape direction
  //  FVector escapeLocation = FVector::ZeroVector;
  //  for (int32 i = 0; i < mEnemies.Num(); ++i)
  //  {
  //    escapeLocation += mEnemies[i]->GetActorLocation();
  //  }
  //  escapeLocation /= mEnemies.Num();
  //  FVector escapeDirection = GetActorLocation() - escapeLocation;
  //  //escapeDirection.Normalize();
  //
  //  DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + escapeDirection, FColor::Red, false, -1.0f, 0, 3.0f);
  //
  //  mTargetLocation = escapeDirection;
  //}

  if (mPlayer)
  {
    // Has vision
    FVector playerLocation = mPlayer->GetActorLocation();
    playerLocation.Z -= mPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

    if (FTorchPathFinding::RequestPathToLocation(GetWorld(), this, playerLocation, mPath))
    {
      mTargetLocation = playerLocation;
      mCurrentPathIndex = 0;
    }
  }
  else
  {
    // No vision
    if (mIsSharedPath)
    {
      // Create unique path
      if (mUpdateTarget)
      {
        mUpdateTarget = false;

        if (FTorchSharedPath::GetOrCreate(GetWorld(), this, mSharedPathKey, mPath))
        {
          mTargetLocation = mPath->GetPathPointLocation(mPath->GetPathPoints().Num() - 1).Position;
          mCurrentPathIndex = 0;
        }
      }
      else
      {
        if (FTorchSharedPath::GetOrCreate(GetWorld(), this, mSharedPathKey, mPath))
        {

        }
      }
    }
    else
    {
      if (mUpdateTarget)
      {
        mUpdateTarget = false;

        if (FTorchPathFinding::RequestPathInsideSphere(GetWorld(), this, mPerceptionSphereRadius, mPath, mRelativeLocation))
        {
          mTargetLocation = mPath->GetPathPointLocation(mPath->GetPathPoints().Num() - 1).Position;
          mCurrentPathIndex = 0;
        }
      }
      else
      {
        if (FTorchPathFinding::RequestPathToLocation(GetWorld(), this, mTargetLocation, mPath))
        {

        }
      }
    }
  }
}

bool ATorchCharacter::IsRestPathEven(float maxAngle)
{
  if (mPath)
  {
    FVector prevLocation = mPath->GetPathPointLocation(mCurrentPathIndex).Position;
    FVector prevDirection = GetActorForwardVector();
    for (int32 i = mCurrentPathIndex + 1; i < (mPath->GetPathPoints().Num() - 1); ++i)
    {
      FVector currLocation = mPath->GetPathPointLocation(i).Position;
      FVector currDirection = currLocation - prevLocation;
      currDirection.Normalize();
      float deltaAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(currDirection, prevDirection)));
      DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector{ 0.0f, 0.0f, 1000.0f }, FColor::Green, false, -1.0f, 0, 2.0f);
      prevLocation = currLocation;
      prevDirection = currDirection;
      if (deltaAngle > maxAngle)
      {
        return false;
      }
    }
    return true;
  }
  return false;
}
EHemisphere::Type ATorchCharacter::ComputeHemisphere(const FVector& direction)
{
  float deltaAngleX = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(direction, GetActorRightVector())));
  float deltaAngleY = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(direction, GetActorForwardVector())));

  if (deltaAngleX == 180.0f && deltaAngleY == 0.0f) return EHemisphere::NorthWest;
  if (deltaAngleX == 0.0f && deltaAngleY == 0.0f) return EHemisphere::NorthEast;

  if (deltaAngleX == 180.0f && deltaAngleY == 180.0f) return EHemisphere::SouthWest;
  if (deltaAngleX == 0.0f && deltaAngleY == 180.0f) return EHemisphere::SouthEast;

  return EHemisphere::None;
}
void ATorchCharacter::FollowPath()
{
  if (mPath)
  {
    if (mCurrentPathIndex >= mPath->GetPathPoints().Num())
    {
      // Reached end
      mPath = nullptr;
      mUpdateTarget = true;
      if (mIsSharedPath)
      {
        mIsSharedPath = false;
        TORCH_LOG("Shared path %d finished", mSharedPathKey);
      }
    }
    else
    {
      FVector actorLocation = GetActorLocation();
      actorLocation.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
      FVector targetLocation = mPath->GetPathPointLocation(mCurrentPathIndex).Position;
      FVector deltaDirection = targetLocation - actorLocation;
      FVector deltaDirectionNorm = deltaDirection;
      deltaDirectionNorm.Normalize();

      DrawDebugLine(GetWorld(), GetActorLocation(), targetLocation, FColor::Yellow);
      DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + deltaDirection, FColor::Emerald);

      if (deltaDirection.Size() < mAcceptanceRadius)
      {
        // Next segment
        mCurrentPathIndex++;
      }
      else
      {
        // Rotate towards next path segment
        FRotator targetRotation = FRotationMatrix::MakeFromXZ(deltaDirection, FVector::UpVector).Rotator();
        targetRotation.Pitch = 0.0f;
        targetRotation.Roll = 0.0f;
        SetActorRotation(FMath::RInterpTo(GetActorRotation(), targetRotation, GetWorld()->GetDeltaSeconds(), mRotationSpeed));

        // Walking/Attacking
        if (mPlayer)
        {
          FVector playerActorDir = mPlayer->GetActorLocation() - actorLocation;
          if (playerActorDir.Size() < mAttackRadius)
          {
            mAnimationInstance->Play(mMontageAttack, TEXT(""));
          }
          else
          {
            if (IsRestPathEven())
            {
              DrawDebugCapsule(GetWorld(), GetActorLocation() + GetActorRightVector() * 400.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight(), GetCapsuleComponent()->GetScaledCapsuleRadius(), GetActorRotation().Quaternion(), FColor::Red, false, -1.0f, 0, 3.0f);
              DrawDebugCapsule(GetWorld(), GetActorLocation() + -GetActorRightVector() * 400.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight(), GetCapsuleComponent()->GetScaledCapsuleRadius(), GetActorRotation().Quaternion(), FColor::Red, false, -1.0f, 0, 3.0f);

              SetPrimitiveByName<FFloatProperty>(mAnimationInstance, TEXT("LeftHandAlpha"), 1.0f);
              SetPrimitiveByName<FFloatProperty>(mAnimationInstance, TEXT("RightHandAlpha"), 1.0f);

              // Evade left/right
              if (FMath::FRandRange(0.0f, 1.0f) > mEvadePropability)
              {
                FHitResult hitResult;
                FCollisionShape collisionShape;
                collisionShape.ShapeType = ECollisionShape::Capsule;
                collisionShape.SetCapsule(GetCapsuleComponent()->GetScaledCapsuleRadius(), GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
                FCollisionQueryParams queryParams;
                queryParams.AddIgnoredActor(this);

                if (FMath::FRandRange(0.0f, 1.0f) > 0.5f)
                {
                  if (GetWorld()->SweepSingleByChannel(hitResult, GetActorLocation(), GetActorRightVector() * 400.0f, GetActorRotation().Quaternion(), ECollisionChannel::ECC_Visibility, collisionShape, queryParams))
                  {
                    // First direction
                    mAnimationInstance->Play(mMontageEvadeRight, TEXT(""), 0.75f, true);
                  }
                }
                else
                {
                  if (GetWorld()->SweepSingleByChannel(hitResult, GetActorLocation(), -GetActorRightVector() * 400.0f, GetActorRotation().Quaternion(), ECollisionChannel::ECC_Visibility, collisionShape, queryParams))
                  {
                    // Second direction
                    mAnimationInstance->Play(mMontageEvadeLeft, TEXT(""), 0.75f, true);
                  }
                }
              }
              else
              {
                // Continue walking
                mAnimationInstance->Play(mMontageWalkAggressive, TEXT(""), 3.0f);
              }
            }
            else
            {
              SetPrimitiveByName<FFloatProperty>(mAnimationInstance, TEXT("LeftHandAlpha"), 0.0f);
              SetPrimitiveByName<FFloatProperty>(mAnimationInstance, TEXT("RightHandAlpha"), 0.0f);

              mAnimationInstance->Play(mMontageWalk, TEXT(""));
            }
          }
        }
        else
        {
          mAnimationInstance->Play(mMontageWalk, TEXT(""));
        }
      }
    }
  }
  else
  {
    mAnimationInstance->StopAllMontages(0.2f);
  }
}

void ATorchCharacter::OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
  if (otherActor != nullptr && otherActor != this)
  {
    // Found enemy
    if (Cast<ATorchCharacter>(otherActor))
    {
      mEnemies.Add(Cast<ATorchCharacter>(otherActor));
    }

    // Found player if lost
    if (!mPlayer)
    {
      if (Cast<ATorchTPSCharacter>(otherActor))
      {
        mPlayer = Cast<ATorchTPSCharacter>(otherActor);
      }
    }

    // Create shared path if not already following
    //if (!mIsSharedPath)
    //{
    //  if (mEnemies.Num() >= 3)
    //  {
    //    mIsSharedPath = true;
    //
    //    // Compute path hash
    //    mSharedPathKey = GetUniqueID();
    //    for (int32 i = 0; i < mEnemies.Num(); ++i)
    //    {
    //      mSharedPathKey ^= mEnemies[i]->GetUniqueID();
    //    }
    //    TORCH_LOG("Shared path %d created", mSharedPathKey);
    //  }
    //}

    mUpdateTarget = true;
  }
}
void ATorchCharacter::OnEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
  if (otherActor != nullptr && otherActor != this)
  {
    // Lost enemy
    if (Cast<ATorchCharacter>(otherActor))
    {
      mEnemies.Remove(Cast<ATorchCharacter>(otherActor));
    }

    if (mPlayer)
    {
      // Lost player
      if (Cast<ATorchTPSCharacter>(otherActor))
      {
        mPlayer = nullptr;
      }
    }

    mUpdateTarget = true;
  }
}

void ATorchCharacter::Debug()
{
  if (mEnableDebug)
  {
    // Draw target location
    if (mPath)
    {
      DrawDebugSphere(GetWorld(), mTargetLocation, 100.0f, 32, FColor::Red);
    }

    // Draw optimal path
    if (mPath)
    {
      int32 pathSize = mPath->GetPathPoints().Num();
      FVector floorOffset = FVector{ 0.0f, 0.0f, 88.0f };
      for (int32 i = 0; i < (pathSize - 1); ++i)
      {
        FVector currLocation = mPath->GetPathPointLocation(i).Position;
        FVector nextLocation = mPath->GetPathPointLocation(i + 1).Position;
        DrawDebugLine(GetWorld(), currLocation, currLocation + floorOffset, FColor::White);
        if (i < (pathSize - 1))
        {
          DrawDebugLine(GetWorld(), currLocation, nextLocation, FColor::White);
        }
      }
    }

    // Debug perception
    if (mDrawPerceptionSphere)
    {
      DrawDebugSphere(GetWorld(), GetActorLocation(), mPerceptionSphereRadius, 32, FColor::White);
    }

    // Draw shared path
    if (mDrawSharedPath)
    {
      if (mEnemies.Num() > 0)
      {
        FVector escapeLocation = FVector::ZeroVector;
        for (int32 i = 0; i < mEnemies.Num(); ++i)
        {
          escapeLocation += mEnemies[i]->GetActorLocation();
        }
        escapeLocation /= mEnemies.Num();
        FVector escapeDirection = GetActorLocation() - escapeLocation;
      }
    }
  }
}