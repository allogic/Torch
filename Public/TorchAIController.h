#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TorchPathFinding.h"
#include "TorchAIController.generated.h"

class UNavigationSystemV1;
class ATorchAICharacter;

namespace EPathFindingState
{
  enum Type
  {
    None,
    Found,
  };
}
namespace EPathFollowingState
{
  enum Type
  {
    None,
    Idle,
    Moving,
  };
}
namespace ESurfaceState
{
  enum Type
  {
    None,
    Grounded,
    Airborne,
  };
}

UCLASS()
class TORCH_API ATorchAIController : public AAIController
{
  GENERATED_BODY()

public:
  /*
  * Debugging
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  bool mEnableDebug = true;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  bool mDebugPersistence = true;

  /*
  * Path finding
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  int32 mNumPredictions = 3;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  int32 mNumSubPredictions = 10;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  float mNumPathSegments = 32;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  float mViewAngle = 25.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  float mTraceSphereRadius = 22.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  float mRandomRotationIntensity = 1.5f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  float mTargetRotationIntensity = 0.25f;

  /*
  * Path following
  */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  float mMovementSpeed = 10.0f;

  ATorchAIController();

protected:
  virtual void BeginPlay() override;
  virtual void Tick(float deltaTime) override;

private:
  /** Animation instance */
  UAnimInstance* mAnimationInstance;

  /** Behavior states */
  EPathFindingState::Type mPathFindingState = EPathFindingState::None;
  EPathFollowingState::Type mPathFollowingState = EPathFollowingState::None;
  ESurfaceState::Type mSurfaceState = ESurfaceState::Grounded; // Currently grounded by default; needs ray evaluation

  /** Path finding */
  TArray<FPathSample> mPredictionSamples;
  
  /** Path following */
  int32 mCurrentSampleIndex;
  int32 mCurrentSegmentIndex;
  float mCurrentInterpolTime;

  void FindPath();
  void FollowPath(float deltaTime);
  void DrawDebugGizmo();
};