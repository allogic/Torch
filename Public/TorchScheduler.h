#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TorchScheduler.generated.h"

class UMaterialParameterCollection;
class UMaterialParameterCollectionInstance;

UCLASS()
class TORCH_API ATorchScheduler : public AActor
{
  GENERATED_BODY()

public:

  /*
  * Hell sphere
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHellSphere)
  FVector mHellSphereLocation = FVector::ZeroVector;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHellSphere)
  float mHellSphereRadius = 1000.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHellSphere)
  float mHellSphereHardness = 0.5f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHellSphere)
  float mHellSphereRadiusDecreaseIntensity = 1.0f;

public:

  ATorchScheduler();

protected:

  virtual void BeginPlay() override;
  virtual void Tick(float deltaTime) override;

private:

  /*
  * Hell sphere
  */

  UMaterialParameterCollection* mHellShaderParameters = nullptr;
  UMaterialParameterCollectionInstance* mHellShaderParametersInstance = nullptr;

  bool mEnableHellSphereDecay = false;

  UFUNCTION()
  void HellSphereDecreaseBegin();
  UFUNCTION()
  void HellSphereDecreaseEnd();

  void SyncHellSphereParameters();

  /*
  * Current tasks
  */

  struct FTorchTask
  {
    FTimerHandle Handle = {};
    FTimerDelegate Delegate = {};
  };

  TMap<FName, FTorchTask> mTasks = {};

  FTorchTask& FindOrAdd(const FName& taskName);
  FTorchTask& StartTask(const FName& taskName, float delaySeconds);
};