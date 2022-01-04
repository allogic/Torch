#include "TorchMobSpawner.h"
#include "DrawDebugHelpers.h"
#include "TorchMath.h"
#include "TorchAICharacter.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "TorchAICharacter.h"

ATorchMobSpawner::ATorchMobSpawner()
{
  // Setup actor
  RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
  PrimaryActorTick.bCanEverTick = true;
}

void ATorchMobSpawner::BeginPlay()
{
  Super::BeginPlay();

  // Spawn mobs
  if (mDefaultMob)
  {
    for (int32 i = 0; i < mMobCount; ++i)
    {
      FVector location = FTorchMath::RandomInsideUnitCircle() * mSpawnRadius;

      if (mEnableDebug)
      {
        DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + location, FColor::Red, false, 1.0f);
        DrawDebugSphere(GetWorld(), GetActorLocation() + location, 100.0f, 32, FColor::Red, false, 1.0f);
      }

      ATorchAICharacter* character = Cast<ATorchAICharacter>(UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), mDefaultMob, nullptr, GetActorLocation() + location, FRotator{}));
      if (character)
      {
        
      }
    }
  }
}
void ATorchMobSpawner::Tick(float deltaTime)
{
  Super::Tick(deltaTime);

  if (mEnableDebug)
  {
    DrawDebugSphere(GetWorld(), GetActorLocation(), mSpawnRadius, 32, FColor::Red, false);
  }
}