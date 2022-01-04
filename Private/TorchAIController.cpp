#include "TorchAIController.h"
#include "TorchCore.h"
#include "TorchFPSCharacter.h"
#include "TorchAICharacter.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"

EBehaviorState::Type ATorchAIController::GetBehaviorState()
{
  return mBehaviorState;
}

ATorchAIController::ATorchAIController()
{
  // Setup actor
  PrimaryActorTick.bCanEverTick = true;

  // Setup timers
  mTimerDelegateRecalculatePath.BindUFunction(this, TEXT("ForceRecalculatePath"));
}

void ATorchAIController::BeginPlay()
{
  Super::BeginPlay();

  // Setup navigation system
  mNavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());

  // Setup timers
  GetWorldTimerManager().SetTimer(mTimerHandleRecalculatePath, mTimerDelegateRecalculatePath, 0.2f, true);
}
void ATorchAIController::Tick(float deltaTime)
{
  Super::Tick(deltaTime);

  // Continue fetching possesed character and player
  mAICharacter = Cast<ATorchAICharacter>(GetCharacter());
  if (mAICharacter)
  {
    // Fetch player reference
    mPlayerCharacter = mAICharacter->GetPlayerCharacter();

    if (mPlayerCharacter)
    {
      // Compute some stuff
      mTargetLocation = mPlayerCharacter->GetActorLocation();
      mTargetDirection = (mTargetLocation - mAICharacter->GetActorLocation()).GetSafeNormal();

      // Compute rotation rotations
      mTargetRotation = FRotationMatrix::MakeFromX(mTargetDirection).Rotator();
      mTargetRotationDelta = (mTargetRotation.Quaternion() * mAICharacter->GetActorRotation().Quaternion().Inverse()).Rotator();
    }

    // AI brain
    switch (mBehaviorState)
    {
      case EBehaviorState::Idle:
      {
        // State reset
        if (mStateChanged)
        {
          TORCH_LOG("Idle");
          mStateChanged = false;
          mAICharacter->StopWalkAnimation();
          mAICharacter->DisableControllerRotation();
        }
        // Cancel current path
        if (mIsFollowingPath)
        {
          StopMovement();
        }
        // Update perceptions
        if (CanSeePlayer())
        {
          if (InAttackRange())
          {
            if (FMath::Abs(mTargetRotationDelta.Yaw) < mMinRotationCorrectionAngle)
            {
              mBehaviorState = (EBehaviorState::Type)FMath::FloorToInt(FMath::RandRange(EBehaviorState::MeleeBegin + 1, EBehaviorState::MeleeEnd - 1));
            }
            else
            {
              mBehaviorState = EBehaviorState::TurnToTargetLocation;
            }
          }
          else
          {
            if (FMath::Abs(mTargetRotationDelta.Yaw) < mMinRotationCorrectionAngle)
            {
              mBehaviorState = EBehaviorState::WalkToTargetLocation;
            }
            else
            {
              mBehaviorState = EBehaviorState::TurnToTargetLocation;
            }
          }
        }
        break;
      }
      case EBehaviorState::WalkToTargetLocation:
      {
        // State reset
        if (mStateChanged)
        {
          TORCH_LOG("WalkToTargetLocation");
          mStateChanged = false;
          mAICharacter->EnableControllerRotation();
          mAICharacter->PlayWalkAnimation(1.0f, 80.0f);
        }
        // Continue computing paths towards player location
        if (mPlayerCharacter && (!mIsFollowingPath || mForceRecalculatePath))
        {
          mForceRecalculatePath = false;
          StopMovement();
          mIsFollowingPath = MoveToNearLocation(mTargetLocation);
        }
        // Update perceptions
        if (CanSeePlayer())
        {
          if (InAttackRange())
          {
            mBehaviorState = EBehaviorState::Idle;
          }
        }
        else
        {
          if (!InAttackRange())
          {
            mBehaviorState = EBehaviorState::WalkToTargetLocationLastSeen;
          }
        }
        break;
      }
      case EBehaviorState::WalkToTargetLocationLastSeen:
      {
        // State reset
        if (mStateChanged)
        {
          TORCH_LOG("WalkToTargetLocationLastSeen");
          mStateChanged = false;
          mAICharacter->EnableControllerRotation();
          mAICharacter->PlayWalkAnimation(1.0f, 80.0f);
        }
        // Computing path towards last seen location
        if (mPlayerCharacter && !mIsFollowingPath)
        {
          StopMovement();
          mIsFollowingPath = MoveToNearLocation(mTargetLocation);
        }
        // Update perceptions
        if (CanSeePlayer())
        {
          if (!InAttackRange())
          {
            mBehaviorState = EBehaviorState::WalkToTargetLocation;
          }
        }
        else
        {
          if (IsNearLocation(100.0f, mTargetLocation))
          {
            mBehaviorState = EBehaviorState::Idle;
          }
        }
        break;
      }
      case EBehaviorState::TurnToTargetLocation:
      {
        // State reset
        if (mStateChanged)
        {
          TORCH_LOG("TurnToTargetLocation");
          mStateChanged = false;
        }
        // Rotate towards target location
        mAICharacter->SetActorRotation(FMath::Lerp(mAICharacter->GetActorRotation().Quaternion(), mTargetRotation.Quaternion(), 0.1f));
        // Check if we facing target
        if (FMath::Abs(mTargetRotationDelta.Yaw) < mMinRotationCorrectionAngle)
        {
          mBehaviorState = EBehaviorState::Idle;
        }
        // Update foot root offset

        break;
      }
      case EBehaviorState::MeleeSingleAttack:
      {
        // State reset
        if (mStateChanged)
        {
          TORCH_LOG("MeleeSingleAttack");
          mStateChanged = false;
          mAICharacter->PlayAttackAnimation(1.0f);
          mAICharacter->IsAttacking(true);
        }
        // Check if we finished attacking
        if (mAICharacter->IsAttacking())
        {
          
        }
        else
        {
          mBehaviorState = EBehaviorState::Idle;
        }
        break;
      }
      default:
      {
        mBehaviorState = EBehaviorState::Idle;
        break;
      }
    }
  }

  // Check if behavior state changed since last frame
  if (mBehaviorStatePrev != mBehaviorState)
  {
    mBehaviorStatePrev = mBehaviorState;
    mStateChanged = true;
  }

  // Draw some debugs
  Debug();
}
void ATorchAIController::OnPossess(APawn* pawn)
{
  Super::OnPossess(pawn);
}
void ATorchAIController::OnUnPossess()
{
  Super::OnUnPossess();
}
void ATorchAIController::OnMoveCompleted(FAIRequestID requestID, const FPathFollowingResult& result)
{
  Super::OnMoveCompleted(requestID, result);

  switch (result.Code)
  {
    case EPathFollowingResult::Blocked:
    case EPathFollowingResult::Aborted:
    case EPathFollowingResult::Invalid:
    case EPathFollowingResult::OffPath:
    case EPathFollowingResult::Success:
    {
      mIsFollowingPath = false;
      break;
    }
  }
}

bool ATorchAIController::CanSeePlayer()
{
  if (mAICharacter && mPlayerCharacter)
  {
    FHitResult hitResult;
    FCollisionQueryParams queryParams;
    queryParams.AddIgnoredActor(mAICharacter);
    FVector start = mAICharacter->GetActorLocation();
    FVector end = mPlayerCharacter->GetActorLocation();
    return !GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility, queryParams);
  }
  return false;
}
bool ATorchAIController::InAttackRange()
{
  if (mAICharacter && mPlayerCharacter)
  {
    float distance = FVector::Distance(mAICharacter->GetActorLocation(), mPlayerCharacter->GetActorLocation());
    return distance < mAttackRadiusMelee;
  }
  return false;
}
bool ATorchAIController::IsNearLocation(float acceptanceRadius, const FVector& location)
{
  if (mAICharacter)
  {
    float distance = FVector::Distance(mAICharacter->GetActorLocation(), location);
    return distance < acceptanceRadius;
  }
  return false;
}

void ATorchAIController::ForceRecalculatePath()
{
  mForceRecalculatePath = true;
}

bool ATorchAIController::MoveToNearLocation(const FVector& location, float acceptanceRadius)
{
  if (mNavigationSystem)
  {
    FNavLocation navLocation;
    if (mNavigationSystem->GetRandomPointInNavigableRadius(location, acceptanceRadius, navLocation, nullptr))
    {
      EPathFollowingRequestResult::Type result = MoveToLocation(navLocation.Location);
      if (result == EPathFollowingRequestResult::RequestSuccessful || result == EPathFollowingRequestResult::AlreadyAtGoal)
      {
        return true;
      }
    }
  }
  return false;
}

void ATorchAIController::Debug()
{
  ATorchAICharacter* controller = GetPawn<ATorchAICharacter>();
  if (controller && controller->mEnableDebug)
  {
    // Draw segment target
    if (GetPathFollowingComponent()->HasValidPath())
    {
      FNavPathSharedPtr path = GetPathFollowingComponent()->GetPath();
      int32 pathIndex = GetPathFollowingComponent()->GetNextPathIndex();
      DrawDebugLine(GetWorld(), controller->GetActorLocation(), path->GetPathPointLocation(pathIndex).Position, FColor::White, false);
    }

    // Draw path
    if (GetPathFollowingComponent()->HasValidPath())
    {
      FNavPathSharedPtr optimalPath = GetPathFollowingComponent()->GetPath();
      int32 pathSize = optimalPath->GetPathPoints().Num();
      FVector floorOffset = FVector{ 0.0f, 0.0f, 88.0f };
      for (int32 i = 0; i < (pathSize - 1); ++i)
      {
        FVector currLocation = optimalPath->GetPathPointLocation(i).Position;
        FVector nextLocation = optimalPath->GetPathPointLocation(i + 1).Position;
        DrawDebugLine(GetWorld(), currLocation, currLocation + floorOffset, FColor::White, false);
        if (i < (pathSize - 1))
        {
          DrawDebugLine(GetWorld(), currLocation, nextLocation, FColor::White, false);
        }
      }
    }
  }
}