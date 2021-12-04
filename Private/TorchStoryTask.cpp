#include "TorchStoryTask.h"

ATorchStoryTask::ATorchStoryTask()
{
  // Setup root actor
  PrimaryActorTick.bCanEverTick = true;
}

void ATorchStoryTask::BeginPlay()
{
  Super::BeginPlay();
}
void ATorchStoryTask::Tick(float deltaTime)
{
  Super::Tick(deltaTime);
}

void ATorchStoryTask::UpdateFireRateDecay()
{

}