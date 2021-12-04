#include "TorchStory.h"
#include "TorchStoryTask.h"
#include "Kismet/GameplayStatics.h"

ATorchStory::ATorchStory()
{
  // Setup root actor
  PrimaryActorTick.bCanEverTick = true;
}

void ATorchStory::BeginPlay()
{
  Super::BeginPlay();

  // Get all story tasks
  UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATorchStoryTask::StaticClass(), mStoryActors);
}
void ATorchStory::Tick(float deltaTime)
{
  Super::Tick(deltaTime);
}