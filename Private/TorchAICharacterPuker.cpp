#include "TorchAICharacterPuker.h"
#include "TorchBlueprintUtils.h"
#include "TorchMath.h"
#include "TorchCore.h"
#include "TorchAIControllerPuker.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ATorchAICharacterPuker::ATorchAICharacterPuker()
{
  // Setup actor
  PrimaryActorTick.bCanEverTick = true;

  // Setup capsule
  GetCapsuleComponent()->SetCapsuleHalfHeight(92.0f);
  GetCapsuleComponent()->SetCapsuleRadius(10.0f);

  // Setup skeletal mesh
  GetMesh()->SetRelativeLocationAndRotation(FVector{ 0.0f, 0.0f, -88.0f }, FRotator{ 0.0f, -90.0f, 0.0f });
  static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh(TEXT("SkeletalMesh'/Game/Build/Characters/DeadSpace/Puker/SM_Puker.SM_Puker'"));
  if (skeletalMesh.Succeeded()) GetMesh()->SetSkeletalMesh(skeletalMesh.Object);

  // Setup animation blueprint
  static ConstructorHelpers::FClassFinder<UTorchAnimationInstance> animationInstance(TEXT("/Game/Build/Characters/DeadSpace/Puker/AB_Puker"));
  if (animationInstance.Succeeded()) GetMesh()->SetAnimInstanceClass(animationInstance.Class);

  // Setup animation montages
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageWalk(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Puker/Montages/AM_Puker_Walk.AM_Puker_Walk'"));
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageWalkAggressive(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Puker/Montages/AM_Puker_Walk_Aggressive.AM_Puker_Walk_Aggressive'"));
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageRun(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Puker/Montages/AM_Puker_Run.AM_Puker_Run'"));

  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageTurnLeft90(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Puker/Montages/AM_Puker_Turn_Left_90.AM_Puker_Turn_Left_90'"));
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageTurnLeft180(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Puker/Montages/AM_Puker_Turn_Left_180.AM_Puker_Turn_Left_180'"));
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageTurnRight90(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Puker/Montages/AM_Puker_Turn_Right_90.AM_Puker_Turn_Right_90'"));
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageTurnRight180(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Puker/Montages/AM_Puker_Turn_Right_180.AM_Puker_Turn_Right_180'"));

  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageEvadeLeft(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Puker/Montages/AM_Puker_Evade_Left.AM_Puker_Evade_Left'"));
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageEvadeRight(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Puker/Montages/AM_Puker_Evade_Right.AM_Puker_Evade_Right'"));

  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageAttack(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Puker/Montages/AM_Puker_Attack.AM_Puker_Attack'"));

  if (montageWalk.Succeeded()) mMontageWalk = montageWalk.Object;
  if (montageWalkAggressive.Succeeded()) mMontageWalkAggressive = montageWalkAggressive.Object;
  if (montageRun.Succeeded()) mMontageRun = montageRun.Object;

  if (montageTurnLeft90.Succeeded()) mMontageTurnLeft90 = montageTurnLeft90.Object;
  if (montageTurnLeft180.Succeeded()) mMontageTurnLeft180 = montageTurnLeft180.Object;
  if (montageTurnRight90.Succeeded()) mMontageTurnRight90 = montageTurnRight90.Object;
  if (montageTurnRight180.Succeeded()) mMontageTurnRight180 = montageTurnRight180.Object;

  if (montageEvadeLeft.Succeeded()) mMontageEvadeLeft = montageEvadeLeft.Object;
  if (montageEvadeRight.Succeeded()) mMontageEvadeRight = montageEvadeRight.Object;

  if (montageAttack.Succeeded()) mMontageAttack = montageAttack.Object;

  // Setup AI controller
  static ConstructorHelpers::FClassFinder<ATorchAIControllerPuker> AIController(TEXT("/Script/Torch.TorchAIControllerPuker"));
  if (AIController.Succeeded()) AIControllerClass = AIController.Class;
}

void ATorchAICharacterPuker::BeginPlay()
{
  Super::BeginPlay();

  // Setup physical animation
  FPhysicalAnimationData physicalAnimationFoot{};
  physicalAnimationFoot.bIsLocalSimulation = true;
  physicalAnimationFoot.OrientationStrength = 2000.0f;
  physicalAnimationFoot.AngularVelocityStrength = 200.0f;
  physicalAnimationFoot.PositionStrength = 2000.0f;
  physicalAnimationFoot.VelocityStrength = 200.0f;
  FPhysicalAnimationData physicalAnimationFlesh{};
  physicalAnimationFlesh.bIsLocalSimulation = true;
  physicalAnimationFlesh.OrientationStrength = 30.0f;
  physicalAnimationFlesh.AngularVelocityStrength = 3.0f;
  physicalAnimationFlesh.PositionStrength = 30.0f;
  physicalAnimationFlesh.VelocityStrength = 3.0f;
  FPhysicalAnimationData physicalAnimationArmL{};
  physicalAnimationArmL.bIsLocalSimulation = false;
  physicalAnimationArmL.OrientationStrength = 2000.0f;
  physicalAnimationArmL.AngularVelocityStrength = 200.0f;
  physicalAnimationArmL.PositionStrength = 2000.0f;
  physicalAnimationArmL.VelocityStrength = 200.0f;
  FPhysicalAnimationData physicalAnimationArmR{};
  physicalAnimationArmR.bIsLocalSimulation = false;
  physicalAnimationArmR.OrientationStrength = 10000.0f;
  physicalAnimationArmR.AngularVelocityStrength = 1000.0f;
  physicalAnimationArmR.PositionStrength = 10000.0f;
  physicalAnimationArmR.VelocityStrength = 1000.0f;
  FPhysicalAnimationData physicalAnimationSpine{};
  physicalAnimationSpine.bIsLocalSimulation = false;
  physicalAnimationSpine.OrientationStrength = 3000.0f;
  physicalAnimationSpine.AngularVelocityStrength = 300.0f;
  physicalAnimationSpine.PositionStrength = 3000.0f;
  physicalAnimationSpine.VelocityStrength = 300.0f;
  mPhysicalAnimationComponent->SetSkeletalMeshComponent(GetMesh());
  mPhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(TEXT("spine_01"), physicalAnimationSpine);
  mPhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(TEXT("foot_add"), physicalAnimationFoot);
  mPhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(TEXT("clavicle_l"), physicalAnimationArmR);
  mPhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(TEXT("clavicle_r"), physicalAnimationArmR);
  mPhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(TEXT("loop_foot"), physicalAnimationFlesh);
  mPhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(TEXT("loop_arm"), physicalAnimationFlesh);
  mPhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(TEXT("loop_foot"), physicalAnimationFlesh);
  mPhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(TEXT("flesh_arm"), physicalAnimationFlesh);
  mPhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(TEXT("flesh_chest"), physicalAnimationFlesh);
  GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("spine_01"), true);

  // Setup path arrays
  mLeftPath.Init(FVector{}, mNumPathSegments);
  mRightPath.Init(FVector{}, mNumPathSegments);

  // Setup foot state
  mFootLeftPrevLocation = GetActorForwardVector() * -1.0f;
  mFootRightPrevLocation = GetActorForwardVector() * -1.0f;
}
void ATorchAICharacterPuker::Tick(float deltaTime)
{
  Super::Tick(deltaTime);

  // Get root offset
  UpdateRootLocation();

  // Compute foot ground hits
  TraceFootRays();
  //FollowPathSegment();

  // Draw some debugs
  Debug();
}

void ATorchAICharacterPuker::StopWalkAnimation()
{
  if (mAnimationInstance)
  {
    if (mAnimationInstance->Montage_IsPlaying(mMontageWalk))
    {
      mAnimationInstance->Montage_StopWithBlendOut(FAlphaBlendArgs{ 0.2f }, mMontageWalk);
    }
  }
}
void ATorchAICharacterPuker::PlayWalkAnimation(float playRate, float movementSpeed)
{
  if (mAnimationInstance && mMontageWalk && mAnimationMontageWalkSections.Num() > 0)
  {
    const FString& sectionName = mAnimationMontageWalkSections[FMath::FloorToInt(FMath::RandRange(0, mAnimationMontageWalkSections.Num() - 1))];
    mAnimationInstance->Montage_JumpToSection(&sectionName[0], mMontageWalk);
    mAnimationInstance->Montage_PlayWithBlendIn(mMontageWalk, FAlphaBlendArgs{ 0.2f });
    GetCharacterMovement()->MaxWalkSpeed = movementSpeed;
  }
}
void ATorchAICharacterPuker::PlayRunAnimation(float playRate, float movementSpeed)
{
  if (mAnimationInstance && mMontageWalkAggressive && mAnimationMontageWalkAggressiveSections.Num() > 0)
  {
    const FString& sectionName = mAnimationMontageWalkAggressiveSections[FMath::FloorToInt(FMath::RandRange(0, mAnimationMontageWalkAggressiveSections.Num() - 1))];
    mAnimationInstance->Montage_JumpToSection(&sectionName[0], mMontageWalkAggressive);
    mAnimationInstance->Montage_PlayWithBlendIn(mMontageWalkAggressive, FAlphaBlendArgs{ 0.2f });
    GetCharacterMovement()->MaxWalkSpeed = movementSpeed;
  }
}
void ATorchAICharacterPuker::PlayAttackAnimation(float playRate)
{
  if (mAnimationInstance && mMontageAttack && mAnimationMontageAttackSections.Num() > 0)
  {
    const FString& sectionName = mAnimationMontageAttackSections[FMath::FloorToInt(FMath::RandRange(0, mAnimationMontageAttackSections.Num() - 1))];
    mAnimationInstance->Montage_JumpToSection(&sectionName[0], mMontageAttack);
    mAnimationInstance->Montage_PlayWithBlendIn(mMontageAttack, FAlphaBlendArgs{ 0.2f });
  }
}

void ATorchAICharacterPuker::UpdateRootLocation()
{
  mRootVelocity = FMath::Lerp(mRootVelocity, FVector::ZeroVector, 0.1f);
  mRootLocation = mRootVelocity;
}

void ATorchAICharacterPuker::TraceFootRays()
{
  FHitResult leftHit;
  FHitResult rightHit;

  mRotationDistance = FMath::Abs((GetVelocity().Rotation().Quaternion() * GetActorRotation().Quaternion().Inverse()).Rotator().GetNormalized().Yaw);
  mVelocityDistance = FVector::Distance(FVector::ZeroVector, GetVelocity());

  FVector leftOffset = mFootLeftLocationOffset;
  FVector rightOffset = mFootRightLocationOffset;
  leftOffset.X = mVelocityDistance * mFootLeftRayVelocityScale;
  rightOffset.X = mVelocityDistance * mFootRightRayVelocityScale;
  if (mRotationDistance > mFootVelocityKillAngle)
  {
    leftOffset *= -mRotationDistance * 0.001f;
    rightOffset *= -mRotationDistance * 0.001f;
  }
  mFootLeftStart = GetActorLocation() + GetActorRotation().RotateVector(leftOffset);
  mFootRightStart = GetActorLocation() + GetActorRotation().RotateVector(rightOffset);

  DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetVelocity() * 10, FColor::Red, false, -1.0f, 0, 2.0f);

  FCollisionQueryParams queryParams;
  queryParams.AddIgnoredActor(this);

  float distanceLeft = FVector::Distance(mFootLeftLocation, mFootLeftStart);
  float distanceRight = FVector::Distance(mFootRightLocation, mFootRightStart);

  static bool isInitLeft = true;
  static bool isInitRight = true;

  if ((distanceLeft > mFootLeftCorrectionDistance && mCurrentLeftTime >= 1.0f) || mRotationDistance > 1001.0f)
  {
    if (GetWorld()->LineTraceSingleByChannel(leftHit, mFootLeftStart, mFootLeftStart + GetActorUpVector() * -300.0f, ECollisionChannel::ECC_Visibility, queryParams))
    {
      if (isInitLeft)
      {
        isInitLeft = false;
        mFootLeftPrevLocation = leftHit.Location + GetActorForwardVector() * -1.0f;
      }
      else
      {
        mFootLeftPrevLocation = mFootLeftLocation;
      }
      mFootLeftLocation = leftHit.Location;
      // Recompute denbug path
      ComputePathSegments(mLeftPath, mFootLeftPrevLocation, mFootLeftLocation);
      // Add ground impact velocity
      mRootVelocity += FVector::DownVector * 5.0f;
      //TORCH_LOG("left hit");
    }
  }
  if ((distanceRight > mFootRightCorrectionDistance && mCurrentRightTime >= 1.0f) || mRotationDistance > 1001.0f)
  {
    if (GetWorld()->LineTraceSingleByChannel(rightHit, mFootRightStart, mFootRightStart + GetActorUpVector() * -300.0f, ECollisionChannel::ECC_Visibility, queryParams))
    {
      if (isInitRight)
      {
        isInitRight = false;
        mFootRightPrevLocation = rightHit.Location + GetActorForwardVector() * -1.0f;
      }
      else
      {
        mFootRightPrevLocation = mFootRightLocation;
      }
      mFootRightLocation = rightHit.Location;
      // Recompute debug path
      ComputePathSegments(mRightPath, mFootRightPrevLocation, mFootRightLocation);
      // Add ground impact velocity
      mRootVelocity += FVector::DownVector * 5.0f;
      //TORCH_LOG("right hit");
    }
  }
}
void ATorchAICharacterPuker::ComputePathSegments(TArray<FVector>& pathSegments, const FVector& previous, const FVector& current)
{
  for (int32 i = 0; i < mNumPathSegments; ++i)
  {
    float time = (1.0f / (mNumPathSegments - 1)) * i;
    pathSegments[i] = FTorchMath::CubicBezier(previous, previous + GetActorRotation().RotateVector(mFootBezierBeginDirection) * mFootBezierBeginScale * mVelocityDistance * mFootBezierVelocityScale, current + GetActorRotation().RotateVector(mFootBezierEndDirection) * mFootBezierEndScale * mVelocityDistance * mFootBezierVelocityScale, current, time);
  }
}
void ATorchAICharacterPuker::FollowPathSegment()
{
  static bool flip = true;
  if (flip)
  {
    if (mCurrentLeftTime < 1.0f)
    {
      mCurrentLeftTime += mFootUpdateTime;
      mFootLeftCurrLocation = FTorchMath::CubicBezier(mFootLeftPrevLocation, mFootLeftPrevLocation + GetActorRotation().RotateVector(mFootBezierBeginDirection) * mFootBezierBeginScale * mVelocityDistance * mFootBezierVelocityScale, mFootLeftLocation + GetActorRotation().RotateVector(mFootBezierEndDirection) * mFootBezierEndScale * mVelocityDistance * mFootBezierVelocityScale, mFootLeftLocation, mCurrentLeftTime);
    }
    else
    {
      mCurrentLeftTime = 0.0f;
      flip = !flip;
    }
  }
  else
  {
    if (mCurrentRightTime < 1.0f)
    {
      mCurrentRightTime += mFootUpdateTime;
      mFootRightCurrLocation = FTorchMath::CubicBezier(mFootRightPrevLocation, mFootRightPrevLocation + GetActorRotation().RotateVector(mFootBezierBeginDirection) * mFootBezierBeginScale * mVelocityDistance * mFootBezierVelocityScale, mFootRightLocation + GetActorRotation().RotateVector(mFootBezierEndDirection) * mFootBezierEndScale * mVelocityDistance * mFootBezierVelocityScale, mFootRightLocation, mCurrentRightTime);
    }
    else
    {
      mCurrentRightTime = 0.0f;
      flip = !flip;
    }
  }
}

void ATorchAICharacterPuker::UpdateAnimationBlueprint()
{
  SetStructureByName(mAnimationInstance, TEXT("LeftFootLocation"), mFootLeftCurrLocation - mRootLocation);
  SetStructureByName(mAnimationInstance, TEXT("RightFootLocation"), mFootRightCurrLocation - mRootLocation);
  SetStructureByName(mAnimationInstance, TEXT("RootLocation"), mRootLocation);
}

void ATorchAICharacterPuker::Debug()
{
  if (mEnableDebug)
  {
    FVector start = FVector{ 0.0f, 0.0f, -88.0f } * GetActorScale();
    FVector end = GetActorRotation().RotateVector(mRootVelocity * 1000.0f);
    DrawDebugLine(GetWorld(), GetActorLocation() + start, GetActorLocation() + end, FColor::Blue, false, -1.0f, 0, 2.0f);

    // Draw foot IK paths
    DrawDebugLine(GetWorld(), mFootLeftStart, mFootLeftStart + GetActorUpVector() * -100.0f, FColor::Green, false, -1.0f, 0, 2.0f);
    DrawDebugLine(GetWorld(), mFootRightStart, mFootRightStart + GetActorUpVector() * -100.0f, FColor::Green, false, -1.0f, 0, 2.0f);
    DrawDebugSphere(GetWorld(), mFootLeftLocation, 10.0f, 32, FColor::Yellow, false, -1.0f, 0, 2.0f);
    DrawDebugSphere(GetWorld(), mFootRightLocation, 10.0f, 32, FColor::Yellow, false, -1.0f, 0, 2.0f);
    DrawDebugSphere(GetWorld(), mFootLeftPrevLocation, 10.0f, 32, FColor::Purple, false, -1.0f, 0, 2.0f);
    DrawDebugSphere(GetWorld(), mFootRightPrevLocation, 10.0f, 32, FColor::Purple, false, -1.0f, 0, 2.0f);
    for (int32 i = 0; i < (mNumPathSegments - 2); ++i)
    {
      DrawDebugLine(GetWorld(), mLeftPath[i], mLeftPath[i + 1], FColor::Yellow, false, -1.0f, 0, 2.0f);
      DrawDebugLine(GetWorld(), mLeftPath[i], mLeftPath[i] + GetActorUpVector() * 10.0f, FColor::Yellow, false, -1.0f, 0, 2.0f);
    }
    DrawDebugLine(GetWorld(), mLeftPath[mLeftPath.Num() - 2], mLeftPath[mLeftPath.Num() - 1], FColor::Yellow, false, -1.0f, 0, 2.0f);
    for (int32 i = 0; i < (mNumPathSegments - 2); ++i)
    {
      DrawDebugLine(GetWorld(), mRightPath[i], mRightPath[i + 1], FColor::Yellow, false, -1.0f, 0, 2.0f);
      DrawDebugLine(GetWorld(), mRightPath[i], mRightPath[i] + GetActorUpVector() * 10.0f, FColor::Yellow, false, -1.0f, 0, 2.0f);
    }
    DrawDebugLine(GetWorld(), mRightPath[mRightPath.Num() - 2], mRightPath[mRightPath.Num() - 1], FColor::Yellow, false, -1.0f, 0, 2.0f);
  }
}