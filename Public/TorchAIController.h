#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TorchAIController.generated.h"

class UNavigationSystemV1;
class ATorchAICharacter;
class USplineComponent;

namespace EBehaviorState
{
  enum Type
  {
    Surface,
    Airborne,
  };
}

UCLASS()
class TORCH_API ATorchAIController : public AAIController
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  bool mDebug = true;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  AActor* mTargetActor = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  int32 mNumPredictions = 10;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  int32 mNumSubPredictions = 9;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  float mNumPathSegments = 32;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  float mViewAngle = 45.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  float mTraceSphereRadius = 22.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  float mRandomRotationIntensity = 1.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  float mTargetRotationIntensity = 0.2f;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = TorchPathFinding)
  USplineComponent* mSplineComponent = nullptr;

  ATorchAIController();

protected:
  virtual void BeginPlay() override;
  virtual void Tick(float deltaTime) override;

private:
  /** Animation instance */
  UAnimInstance* mAnimationInstance;

  /** Path prediction */
  FVector mDirectionTarget;
  FTimerHandle mPredictionTimeHdl;
  TArray<FTransform> mPath;

  void PredictPath();
  void FollowPath();
  void DrawDebugGizmo();
};