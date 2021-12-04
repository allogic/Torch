#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TorchAnimationInstance.h"
#include "TorchTPSCharacter.generated.h"

class UCameraComponent;
class ATorchWeapon;

UCLASS()
class TORCH_API ATorchTPSCharacter : public ACharacter
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

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  UCameraComponent* mCameraComponent = nullptr;

  /*
  * Controller
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchController)
  bool mAiming = false;

  /*
  * Spine IK
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchSpineIK)
  float mSpinePitch = -45.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchSpineIK)
  float mSpineUpDistance = 50.0f;

  /*
  * Hand IK
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHandIK)
  FVector mHandLocationL = FVector{ 50.0f, -20.0f, 30.0f };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchHandIK)
  FVector mHandLocationR = FVector{ 50.0f, 20.0f, 30.0f };

  /*
  * Camera system
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchCamera)
  bool mCameraInvertYaw = false;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchCamera)
  bool mCameraInvertPitch = true;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchCamera)
  bool mCameraOrbitIgnoreYawConstraints = true;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchCamera)
  bool mCameraOrbitIgnorePitchConstraints = false;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchCamera)
  float mCameraOrbitMinYaw = -180.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchCamera)
  float mCameraOrbitMaxYaw = 180.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchCamera)
  float mCameraOrbitMinPitch = -75.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchCamera)
  float mCameraOrbitMaxPitch = 75.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchCamera)
  FVector mCameraOrbitLocation = FVector{ -150.0f, 60.0f, 60.0f };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchCamera)
  FVector mCameraAimLocation = FVector{ -100.0f, 40.0f, 80.0f };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchCamera)
  FVector mCameraAimOffset = FVector{ 30.0f, 10.0f, -100.0f };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchCamera)
  FVector mCameraShakeFrequency = FVector{ 0.5f, 0.5f, 0.1f };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchCamera)
  FVector mCameraShakeIntensity = FVector{ 2.0f, 2.0f, 0.75f };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchCamera)
  FVector mCameraAnimationShakeIntensity = FVector{ 0.05f, 0.05f, 0.05f };

  /*
  * Movement system
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMovement)
  float mMovementSpeedWalking = 120.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMovement)
  float mMovementSpeedCrouching = mMovementSpeedWalking / 2.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMovement)
  float mMovementSpeedRunning = 320.0f;

public:

  /*
  * Weapon system
  */

  FORCEINLINE void SetCurrentWeaponActor(ATorchWeapon* weaponActor) { mCurrentWeaponActor = weaponActor; }

public:

  ATorchTPSCharacter(const FObjectInitializer& initializer);

protected:

  virtual void BeginPlay() override;
  virtual void Tick(float deltaTime) override;
  virtual void SetupPlayerInputComponent(UInputComponent* playerInputComponent) override;

private:

  /*
  * Locomotion system
  */

  bool mIsRunning = false;

  bool mLocomotionForward = false;
  bool mLocomotionBack = false;
  bool mLocomotionRight = false;
  bool mLocomotionLeft = false;
  bool mLocomotionForwardLeft = false;
  bool mLocomotionForwardRight = false;
  bool mLocomotionBackLeft = false;
  bool mLocomotionBackRight = false;

  FRotator mUpperBodyRotationTarget = FRotator::ZeroRotator;
  FRotator mLowerBodyRotationTarget = FRotator::ZeroRotator;

  FRotator mUpperBodyRotationDelta = FRotator::ZeroRotator;
  FRotator mLowerBodyRotationDelta = FRotator::ZeroRotator;

  void UpdateLocomotion();
  void UpdateTurnInPlace(float maxTurnAngle = 45.0f, float angleOffset = 0.0f);
  void UpdateAimOffset();

private:

  /*
  * Animation system
  */

  UTorchAnimationInstance* mAnimationInstance = nullptr;

  UAnimMontage* mMontageForward = nullptr;
  UAnimMontage* mMontageBack = nullptr;
  UAnimMontage* mMontageRight = nullptr;
  UAnimMontage* mMontageLeft = nullptr;
  UAnimMontage* mMontageForwardLeft = nullptr;
  UAnimMontage* mMontageForwardRight = nullptr;
  UAnimMontage* mMontageBackLeft = nullptr;
  UAnimMontage* mMontageBackRight = nullptr;

  UAnimMontage* mMontageRun = nullptr;
  UAnimMontage* mMontageJump = nullptr;
  UAnimMontage* mMontageTurn = nullptr;

  UAnimMontage* mMontageWeaponHip = nullptr;

  UCurveFloat* mCurveFloatRecoil = nullptr;

  bool mReevaluate = true;

  void UpdateSpineIK();
  void UpdateHandIK();
  void UpdateFootIK();

private:

  /*
  * Camera system
  */

  FVector mCameraLocation = FVector::ZeroVector;
  FRotator mCameraRotation = FRotator::ZeroRotator;

  void UpdateCamera();

private:

  /*
  * Weapon system
  */

  ATorchWeapon* mCurrentWeaponActor = nullptr;

  void UpdateWeapon();

private:

  /*
  * Material parameter collections
  */

  UMaterialParameterCollection* mMaterialParameterCollection = nullptr;

  UMaterialParameterCollectionInstance* mMaterialParameterCollectionInstance = nullptr;

  void UpdateMaterial();

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
  void BeginBack();
  UFUNCTION()
  void EndBack();
  UFUNCTION()
  void BeginRight();
  UFUNCTION()
  void EndRight();
  UFUNCTION()
  void BeginLeft();
  UFUNCTION()
  void EndLeft();

  UFUNCTION()
  void BeginAim();
  UFUNCTION()
  void EndAim();

  UFUNCTION()
  void BeginRun();
  UFUNCTION()
  void EndRun();

  UFUNCTION()
  void BeginFire();
  UFUNCTION()
  void EndFire();

  UFUNCTION()
  void BeginReload();

  UFUNCTION()
  void BeginCrouch();
  UFUNCTION()
  void EndCrouch();

  UFUNCTION()
  void BeginJump();
  UFUNCTION()
  void EndJump();
};