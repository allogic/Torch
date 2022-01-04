#pragma once

#include "CoreMinimal.h"
#include "TorchWeapon.h"
#include "TorchWeaponUR86.generated.h"

UCLASS()
class TORCH_API ATorchWeaponUR86 : public ATorchWeapon
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
	FVector mRelativeLocationOffset = FVector{ -42.915958f, 13.924067f, 5.412118f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
	FRotator mRelativeRotationOffset = FRotator{ 8.0f, 70.0f, 5.0f };

public:

	ATorchWeaponUR86();

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