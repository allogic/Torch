#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TorchAnimationInstance.h"
#include "TorchFPSCharacter.generated.h"

class UInputComponent;
class UCameraComponent;
class USphereComponent;
class UAnimMontage;
class UPhysicsHandleComponent;

class ATorchWeapon;

UCLASS()
class TORCH_API ATorchFPSCharacter : public ACharacter
{
  GENERATED_BODY()

public:

	/*
	* Components
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
	UCameraComponent* mCameraComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
	USphereComponent* mPerceptionSphereComponent = nullptr;

	/*
	* Perception system
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPerception)
	float mPerceptionSphereRadius = 300.0f;

	/*
	* Camera settings
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchCamera)
	FVector mCameraRelativeLocation = FVector{ 0.0f, -5.0f, 25.0f };

	/*
	* IK settings
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
	FRotator mArmRotationOffset = FRotator{ 0.0f, -90.0f, 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
	FVector mUpperArmROffset = FVector{ 0.0f, 0.0f, 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
	FVector mUpperArmLOffset = FVector{ -1.0f, 0.0f, -8.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
	FVector mHandROffset = FVector{ 0.0f, 0.0f, 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
	FVector mHandLOffset = FVector{ 0.0f, 0.0f, 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
	FVector mHandAimOffset = FVector{ 15.5f, 0.0f, -1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
	FRotator mHandRAimRotationOffset = FRotator{ 3.0f, 10.0f, -4.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
	float mAimDamping = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
	float mArmVelocityIntensity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
	float mArmVelocityDamping = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
	float mArmVelocityDampingWhileAiming = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
	float mHandRIKAlphaDamping = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
	float mHandLIKAlphaDamping = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
	FVector mRecoilDirection = FVector{ 0.0f, 1.0f, 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
	float mRecoilIntensity = -20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchIK)
	float mRecoilDamping = 0.2f;

public:

	/*
	* Weapon system
	*/

	bool AttachWeapon(ATorchWeapon* weapon);
	bool DetachWeapon();

public:

	/*
	* IK system
	*/

	void EnableHandIK();
	void DisableHandIK();

public:

	ATorchFPSCharacter();

protected:

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* playerInputComponent) override;
	virtual void Tick(float deltaTime) override;

private:

	/*
	* Animation system
	*/

	UAnimMontage* mMontageWalk = nullptr;
	UAnimMontage* mMontageReload = nullptr;
	UAnimMontage* mMontageAiming = nullptr;
	UAnimMontage* mMontageFiring = nullptr;
	UAnimMontage* mMontageMisc = nullptr;

	UTorchAnimationInstance* mAnimationInstance = nullptr;

	void UpdateAnimationBlueprint();

private:

	/*
	* IK system
	*/

	FVector mArmVelocity = FVector{};
	FVector mVelocityOffset = FVector{};
	FVector mRecoilVelocity = FVector{};

	FVector mHandRRestLocation = FVector{};
	FVector mHandLRestLocation = FVector{};

	FVector mCurrHandAimOffset = FVector{};
	FRotator mCurrHandRAimRotation = FRotator{};

	bool mEnableHandIK = false;
	float mHandRIKAlpha = 0.0f;
	float mHandLIKAlpha = 0.0f;

	void UpdateArms();

private:

	/*
	* Camera system
	*/

	FVector mCameraVelocity = FVector{};
	FRotator mCameraRotation = FRotator{};

	void UpdateCamera();

private:

	/*
	* Weapon system
	*/

	ATorchWeapon* mWeapon = nullptr;

	bool mAiming = false;

	void UpdateWeapon();

private:

	/*
	* Perception system
	*/

	TArray<ATorchWeapon*> mWeapons = {};

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

private:

	/*
	* Input delegates
	*/

	UFUNCTION()
	void MoveForward(float value);
	UFUNCTION()
	void MoveRight(float value);

	UFUNCTION()
	void TurnHorizontal(float value);
	UFUNCTION()
	void TurnVertical(float value);

	UFUNCTION()
	void BeginForward();
	UFUNCTION()
	void EndForward();

	UFUNCTION()
	void BeginReload();
	UFUNCTION()
	void EndReload();

	UFUNCTION()
	void BeginJump();
	UFUNCTION()
	void EndJump();

	UFUNCTION()
	void BeginAim();
	UFUNCTION()
	void EndAim();

	UFUNCTION()
	void BeginFire();
	UFUNCTION()
	void EndFire();

	UFUNCTION()
	void BeginInspect();

	UFUNCTION()
	void BeginGrab();
	UFUNCTION()
	void BeginDrop();
};
