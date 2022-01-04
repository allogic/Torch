#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TorchAIController.generated.h"

class UNavigationSystemV1;

class ATorchAICharacter;
class ATorchFPSCharacter;

namespace EBehaviorState
{
  enum Type : int32
  {
    None,

    // Idle states
    Idle,

    // Translational
    WalkToTargetLocation,
    WalkToTargetLocationLastSeen,

    // Rotational
    TurnToTargetLocation,

    // Melee combat states
    MeleeBegin,
    MeleeSingleAttack,
    MeleeEnd,

    MoveToRandomLocation,
    MoveToLocationSingleAttack,
    JumpToRandomLocation,
    Circling,
    RandomizeStats,
    Patrolling,
    Scouting,
  };
}

UCLASS()
class TORCH_API ATorchAIController : public AAIController
{
  GENERATED_BODY()

public:

  /*
  * AI brain
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchBrain)
  float mAttackRadiusMelee = 200.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchBrain)
  float mMinRotationCorrectionAngle = 5.0f;

public:

  /*
  * Character access
  */

  EBehaviorState::Type GetBehaviorState();

public:

  ATorchAIController();

protected:

  virtual void BeginPlay() override;
  virtual void Tick(float deltaTime) override;
  virtual void OnPossess(APawn* pawn) override;
  virtual void OnUnPossess() override;
  virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

private:

  /*
  * Character references
  */

  ATorchAICharacter* mAICharacter = nullptr;
  ATorchFPSCharacter* mPlayerCharacter = nullptr;

private:

  /*
  * State machine
  */

  EBehaviorState::Type mBehaviorStatePrev = EBehaviorState::None;
  EBehaviorState::Type mBehaviorState = EBehaviorState::Idle;

  bool mStateChanged = false;

private:

  /*
  * Perception system
  */

  bool CanSeePlayer();
  bool InAttackRange();
  bool IsNearLocation(float acceptanceRadius, const FVector& location);

private:

  /*
  * Path following
  */

  UNavigationSystemV1* mNavigationSystem = nullptr;

  FVector mTargetLocation = FVector{};
  FVector mTargetDirection = FVector{};

  FRotator mTargetRotation = FRotator{};
  FRotator mTargetRotationDelta = FRotator{};

  bool mIsFollowingPath = false;
  bool mForceRecalculatePath = false;

  FTimerHandle mTimerHandleRecalculatePath = {};
  FTimerDelegate mTimerDelegateRecalculatePath = {};

  UFUNCTION()
  void ForceRecalculatePath();

private:

  /*
  * Combat system
  */

  bool MoveToNearLocation(const FVector& location, float acceptanceRadius = 1.0f);

private:

  /*
  * Debugging
  */

  void Debug();
};