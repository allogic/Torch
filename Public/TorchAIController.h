#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Controller.h"
#include "TorchPathFinding.h"
#include "TorchAIController.generated.h"

class UNavigationSystemV1;
class ATorchAICharacter;

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
class TORCH_API ATorchAIController : public AController
{
  GENERATED_BODY()

public:
  /*
  * Debugging
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchDebug)
  bool mEnableDebug = true;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchDebug)
  bool mDebugPersistence = false;

  /*
  * Path finding
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  int32 mNumSamples = 2;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  int32 mNumSubSamples = 10;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  float mNumPathSegments = 32;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  float mViewAngle = 25.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  float mTraceSphereRadius = 22.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  float mRandomRotationIntensity = 0.2f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  float mTargetRotationIntensity = 0.1f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  int32 mFlowMapGridSize = 10;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  float mFlowMapCellSize = 100.0f;

  /*
  * Path following
  */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  float mMovementSpeed = 10.0f;

  ATorchAIController();

protected:
  virtual void BeginPlay() override;
  virtual void Tick(float deltaTime) override;

  void PhysicsTick(float deltaTime, FBodyInstance* bodyInstance);

private:
  /** Animation instance */
  UAnimInstance* mAnimationInstance;

  /** Behavior states */
  EPathFollowingState::Type mPathFollowingState = EPathFollowingState::None;
  ESurfaceState::Type mSurfaceState = ESurfaceState::Grounded; // Currently grounded by default; needs body evaluation

  /** Path finding */
  TArray<FPathSample> mPathSamples;
  TArray<FTransform> mCurrentPath;

  /** Path following */
  TArray<FFlowMapNode> mFlowMapNodes;

  /** Physic specific */
  FCalculateCustomPhysics mOnCalculateCustomPhysics;

  void ComputePath();
  void ComputeFlowMap();

  void FollowPath(FBodyInstance* bodyInstance);
  void DrawDebugGizmo();
};