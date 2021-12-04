#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TorchMobSpawner.generated.h"

class ATorchCharacter;

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
  int32 mMobCount;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchSpawner)
  float mSpawnRadius;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchSpawner)
  TSubclassOf<ATorchCharacter> mDefaultMob;

public:

  ATorchMobSpawner();

protected:

  virtual void BeginPlay() override;
  virtual void Tick(float deltaTime) override;
};