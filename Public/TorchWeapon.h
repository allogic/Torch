#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TorchWeapon.generated.h"

class USphereComponent;
class USceneComponent;
class UAudioComponent;
class UBoxComponent;
class UStaticMeshComponent;

class ATorchTPSCharacter;

UCLASS()
class TORCH_API ATorchWeapon : public AActor
{
  GENERATED_BODY()

public:

  /*
  * Components
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  USphereComponent* mPerceptionSphereComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  USceneComponent* mMeshGroupComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  UAudioComponent* mAudioComponent = nullptr;

  /*
  * Sub meshes
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchSubMeshes)
  TArray<UStaticMeshComponent*> mStaticMeshes = {};

  /*
  * Equipment system
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchEquipment)
  float mEquipmentRadius = 100.0f;

public:

  /*
  * Weapon system
  */

  void EnableFire() { mIsFiring = true; }
  void DisableFire() { mIsFiring = false; }

public:

  ATorchWeapon();

protected:

  virtual void BeginPlay() override;
  virtual void Tick(float deltaTime) override;

public:

  /*
  * Equipment system
  */

  virtual void OnAttach();
  virtual void OnDetach();

protected:

  /*
  * Helper utilities
  */

  void AddStaticMeshToGroup(const FString& meshName, UStaticMesh* staticMesh);

protected:

  /*
  * Weapon system
  */

  bool mIsFiring = false;
  float mCurrFireRateDecay = 1.0f;

  virtual float GetFireRate() const { return 0.0f; }

public:

  /*
  * IK system
  */

  virtual FVector GetRelativeLocation() { return FVector{}; }
  virtual FRotator GetRelativeRotation() { return FRotator{}; }

private:

  /*
  * Sound system
  */

  USoundWave* mSound = nullptr;
};