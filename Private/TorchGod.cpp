#include "TorchGod.h"

ATorchGod::ATorchGod()
{
  PrimaryActorTick.bCanEverTick = true;
}

void ATorchGod::BeginPlay()
{
  Super::BeginPlay();
}
void ATorchGod::Tick(float deltaTime)
{
  Super::Tick(deltaTime);
}