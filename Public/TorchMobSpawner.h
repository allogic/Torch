#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TorchMobSpawner.generated.h"

class ATorchAICharacter;

UCLASS()
class TORCH_API ATorchMobSpawner : public AActor
{
  GENERATED_BODY()

public:

  /*
  * Debugging
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchDebug)
  bool mEnableDebug = true;

  /*
  * Spawner
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchSpawner)
  int32 mMobCount = 1;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchSpawner)
  float mSpawnRadius = 100.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchSpawner)
  TSubclassOf<ATorchAICharacter> mDefaultMob = nullptr;

public:

  ATorchMobSpawner();

protected:

  virtual void BeginPlay() override;
  virtual void Tick(float deltaTime) override;
};