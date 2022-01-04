#pragma once

#include "CoreMinimal.h"
#include "TorchWeapon.h"
#include "TorchWeaponDSU.generated.h"

UCLASS()
class TORCH_API ATorchWeaponDSU : public ATorchWeapon
{
	GENERATED_BODY()

public:

	/*
	* Settings
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchSettings)
	float mFireRate = 0.04f;


	/*
	* IK system
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
	FVector mRelativeLocationOffset = FVector{ -75.978104f, 24.828535f, 5.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
	FRotator mRelativeRotationOffset = FRotator{ 8.0f, 70.0f, 5.0f };

public:

	ATorchWeaponDSU();

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