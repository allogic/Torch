#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TorchGod.generated.h"

UCLASS()
class TORCH_API ATorchGod : public AActor
{
  GENERATED_BODY()
  
public:
  ATorchGod();

protected:
  virtual void BeginPlay() override;
  virtual void Tick(float deltaTime) override;
};