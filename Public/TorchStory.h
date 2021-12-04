#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TorchStory.generated.h"

class ATorchStoryTask;

UCLASS()
class TORCH_API ATorchStory : public AActor
{
  GENERATED_BODY()

public:

  ATorchStory();

protected:

  virtual void BeginPlay() override;
  virtual void Tick(float deltaTime) override;

private:

  /*
  * Story system
  */

  TArray<AActor*> mStoryActors = {};
};