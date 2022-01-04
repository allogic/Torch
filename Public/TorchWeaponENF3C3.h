#pragma once

#include "CoreMinimal.h"
#include "TorchWeapon.h"
#include "TorchWeaponENF3C3.generated.h"

UCLASS()
class TORCH_API ATorchWeaponENF3C3 : public ATorchWeapon
{
  GENERATED_BODY()

public:

  /*
  * Settings
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchSettings)
  float mFireRate = 0.5f;

  /*
  * IK system
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
  FVector mRelativeLocationOffset = FVector{ -0.509159f, -3.112507f, 21.534008f };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
  FRotator mRelativeRotationOffset = FRotator{ 8.0f, 70.0f, 5.0f };

public:

  ATorchWeaponENF3C3();

protected:

  /*
  * Weapon system
  */

  float GetFireRate() const override { return mFireRate; }

public:

  /*
  * IK system
  */

  FVector GetRelativeLocation() override { return mRelativeLocationOffset; }
  FRotator GetRelativeRotation() override { return mRelativeRotationOffset; }
};