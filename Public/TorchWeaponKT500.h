#pragma once

#include "CoreMinimal.h"
#include "TorchWeapon.h"
#include "TorchWeaponKT500.generated.h"

UCLASS()
class TORCH_API ATorchWeaponKT500 : public ATorchWeapon
{
  GENERATED_BODY()

public:

  /*
  * Timing system
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchTiming)
  float mFireRate = 0.05f;

  /*
  * Inverse hand kinematic
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHandIK)
  FVector mAimOffset = FVector{ 0.0f, 0.0f, 50.0f };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHandIK)
  FVector mHandJointTargetLocationL = FVector{ 0.0f, -1000.0f, -1000.0f };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHandIK)
  FVector mHandJointTargetLocationR = FVector{ 0.0f, -1000.0f, -1000.0f };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHandIK)
  FVector mRestLocationL = FVector{};

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHandIK)
  FVector mRestLocationR = FVector{};

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHandIK)
  FVector mAimLocation = FVector{};

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHandIK)
  FRotator mAimRotation = FRotator{};

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
  UStaticMeshComponent* mScopeMeshComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMeshes)
  UStaticMeshComponent* mSideBodyMeshComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMeshes)
  UStaticMeshComponent* mClipMeshComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMeshes)
  UStaticMeshComponent* mFrontBarrelMeshComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMeshes)
  UStaticMeshComponent* mHandGripMeshComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMeshes)
  UStaticMeshComponent* mMainBodyMeshComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMeshes)
  UStaticMeshComponent* mBackBodyMeshComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMeshes)
  UStaticMeshComponent* mButtMeshComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMeshes)
  UStaticMeshComponent* mStockMeshComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMeshes)
  UStaticMeshComponent* mTriggerMeshComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMeshes)
  UStaticMeshComponent* mUnderBarrelMeshComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMeshes)
  UStaticMeshComponent* mUnderPartMeshComponent = nullptr;

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

  ATorchWeaponKT500();

protected:

  virtual void BeginPlay() override;
};