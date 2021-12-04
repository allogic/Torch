#pragma once

#include "CoreMinimal.h"
#include "TorchWeapon.h"
#include "TorchWeaponENF3C3.generated.h"

class UStaticMeshComponent;

UCLASS()
class TORCH_API ATorchWeaponENF3C3 : public ATorchWeapon
{
  GENERATED_BODY()

public:

  /*
  * Timing system
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchTiming)
  float mFireRate = 0.5f;

  /*
  * Inverse hand kinematic
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHandIK)
  FVector mAimOffset = FVector{ 0.0f, 0.0f, 50.0f };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHandIK)
  FVector mHandJointTargetLocationL = FVector{};

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHandIK)
  FVector mHandJointTargetLocationR = FVector{};

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHandIK)
  FVector mRestLocationL = FVector{};

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHandIK)
  FVector mRestLocationR = FVector{};

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHandIK)
  FVector mAimLocation = FVector{};

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHandIK)
  FVector mHandLocationL = FVector{};

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHandIK)
  FRotator mHandRotationL = FRotator{ 180.0f, 0.0f, -45.0f };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHandIK)
  FVector mHandLocationR = FVector{};

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHandIK)
  FRotator mHandRotationR = FRotator{};

  /*
  * Sub components
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMeshes)
  UStaticMeshComponent* mGlassMeshComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMeshes)
  UStaticMeshComponent* mHandGripMeshComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMeshes)
  UStaticMeshComponent* mSidesMeshComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMeshes)
  UStaticMeshComponent* mFrontMeshComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMeshes)
  UStaticMeshComponent* mDetailMeshComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMeshes)
  UStaticMeshComponent* mBodyMeshComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMeshes)
  UStaticMeshComponent* mBarrelMeshComponent = nullptr;

public:

  /*
  * Inverse hand kinematic
  */

  FVector GetAimOffset() const override { return mAimOffset; }

  FVector GetHandJointTargetLocationL() const override { return mHandJointTargetLocationL; }
  FVector GetHandJointTargetLocationR() const override { return mHandJointTargetLocationR; }

  FVector GetRestLocationL() const override { return mRestLocationL; }
  FVector GetRestLocationR() const override { return mRestLocationR; }

  FVector GetAimLocation() const  override { return mAimLocation; }

  FVector GetHandLocationL() const override { return mHandLocationL; }
  FRotator GetHandRotationL() const override { return mHandRotationL; }

  FVector GetHandLocationR() const override { return mHandLocationR; }
  FRotator GetHandRotationR() const override { return mHandRotationR; }

public:

  ATorchWeaponENF3C3();

protected:

  virtual void BeginPlay() override;
};