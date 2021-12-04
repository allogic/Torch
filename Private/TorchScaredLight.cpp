#include "TorchScaredLight.h"
#include "TorchMath.h"
#include "Components/PointLightComponent.h"
#include "Components/SceneComponent.h"

ATorchScaredLight::ATorchScaredLight()
{
  // Setup root actor
  RootComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
  PrimaryActorTick.bCanEverTick = true;

  // Setup light component
  mPointLightComponent = CreateDefaultSubobject<UPointLightComponent>("PointLightComponent");
  mPointLightComponent->SetupAttachment(RootComponent);
}

void ATorchScaredLight::BeginPlay()
{
  Super::BeginPlay();

  mLightColor = mPointLightComponent->GetLightColor();
}
void ATorchScaredLight::Tick(float deltaTime)
{
  Super::Tick(deltaTime);

  if (mPointLightComponent)
  {
    if (FMath::FRandRange(0.0f, 1.0f) > mFlickerIntensity)
    {
      static bool flipFlop = true;
      if (flipFlop)
      {
        mPointLightComponent->SetLightColor(FLinearColor{ 0.0f, 0.0f, 0.0f, 0.0f });
      }
      else
      {
        mPointLightComponent->SetLightColor(mLightColor);
      }
      flipFlop = !flipFlop;
    }

  }
}