#pragma once

#include "CoreMinimal.h"
#include "TorchWeapon.h"
#include "TorchWeaponMK7.generated.h"

UCLASS()
class TORCH_API ATorchWeaponMK7 : public ATorchWeapon
{
  GENERATED_BODY()

public:

	/*
	* Settings
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchSettings)
	float mFireRate = 0.05f;

	/*
	* IK system
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
	FVector mRelativeLocationOffset = FVector{ -46.683884f, 16.052101f, 6.522908f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
	FRotator mRelativeRotationOffset = FRotator{ 8.0f, 70.0f, 5.0f };

public:

	ATorchWeaponMK7();

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