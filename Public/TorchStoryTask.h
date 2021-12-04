#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TorchStoryTask.generated.h"

UCLASS()
class TORCH_API ATorchStoryTask : public AActor
{
  GENERATED_BODY()

public:
  ATorchStoryTask();

protected:

  virtual void BeginPlay() override;
  virtual void Tick(float deltaTime) override;

private:

  FTimerDelegate mTimerDelegate;
  FTimerHandle mTimerHandle;

  UFUNCTION()
  void UpdateFireRateDecay();
};