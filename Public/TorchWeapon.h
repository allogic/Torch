#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TorchWeapon.generated.h"

class USphereComponent;
class ATorchTPSCharacter;
class USceneComponent;

UCLASS()
class TORCH_API ATorchWeapon : public AActor
{
  GENERATED_BODY()

public:

  /*
  * Debugging
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchDebug)
  bool mEnableDebug = true;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchDebug)
  bool mDebugPersistence = false;

  /*
  * Sub components
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponents)
  USphereComponent* mEquipmentSphereComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponents)
  USceneComponent* mMeshGroupComponent = nullptr;

  /*
  * Equipment system
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchEquipment)
  float mEquipmentRadius = 100.0f;

public:

  /*
  * Inverse hand kinematic
  */

  virtual FVector GetAimOffset() const { return FVector{}; }

  virtual FVector GetHandJointTargetLocationL() const { return FVector{}; }
  virtual FVector GetHandJointTargetLocationR() const { return FVector{}; }

  virtual FVector GetRestLocationL() const { return FVector{}; }
  virtual FVector GetRestLocationR() const { return FVector{}; }

  virtual FVector GetAimLocation() const { return FVector{}; }

  virtual FVector GetHandLocationL() const { return FVector{}; }
  virtual FRotator GetHandRotationL() const { return FRotator{}; }
  
  virtual FVector GetHandLocationR() const { return FVector{}; }
  virtual FRotator GetHandRotationR() const { return FRotator{}; }

  /*
  * Firing system
  */

  virtual float GetFireRate() const { return 1.0f; }
  virtual FVector GetBulletSpawnLocation() const { return FVector{}; }
  void EnableFire() { mIsFiring = true; }
  void DisableFire() { mIsFiring = false; }

public:

  ATorchWeapon();

protected:

  virtual void Tick(float deltaTime) override;

  /*
  * Timing system
  */

  virtual void SetupTimers(float fireRate);

private:

  /*
  * Timing system
  */

  FTimerDelegate mTimerDelegateFireRateDecay;
  FTimerHandle mTimerHandleFireRateDecay;

  bool mIsFiring = false;
  bool mCanFire = true;

  UFUNCTION()
  void UpdateFireRateDecay();

  /*
  * Equipment system
  */

  ATorchTPSCharacter* mTPSCharacter;

  UFUNCTION()
  void OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
};