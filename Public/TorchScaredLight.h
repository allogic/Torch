#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TorchScaredLight.generated.h"

class USceneComponent;
class UPointLightComponent;

UCLASS()
class TORCH_API ATorchScaredLight : public AActor
{
  GENERATED_BODY()

public:

  /*
  * Light system
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchLight)
  float mFlickerIntensity = 1.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchLight)
  FLinearColor mLightColor = FLinearColor{};

  /*
  * Sub components
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  UPointLightComponent* mPointLightComponent = nullptr;

public:

  ATorchScaredLight();

protected:

  virtual void BeginPlay() override;
  virtual void Tick(float deltaTime) override;
};