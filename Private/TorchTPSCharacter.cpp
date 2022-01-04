#include "TorchTPSCharacter.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TorchBlueprintUtils.h"
#include "TorchMath.h"
#include "TorchCore.h"
#include "TorchWeapon.h"
#include "TorchDebug.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialParameterCollectionInstance.h"

ATorchTPSCharacter::ATorchTPSCharacter()
{
  // Setup actor
  PrimaryActorTick.bCanEverTick = true;

  // Setup camera
  mCameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
  //mCameraComponent->SetupAttachment(RootComponent);

  // Setup capsule
  GetCapsuleComponent()->SetCollisionProfileName(TEXT("Custom"));
  GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

  // Setup skeletal mesh
  GetMesh()->SetRelativeLocationAndRotation(FVector{ 0.0f, 0.0f, -88.0f }, FRotator{ 0.0f, -90.0f, 0.0f });
  GetMesh()->SetRelativeScale3D(FVector{ 2.0f });
  GetMesh()->SetCollisionProfileName(TEXT("Custom"));
  GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
  GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
  static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh(TEXT("SkeletalMesh'/Game/Build/Characters/DeadSpace/Isaac/SM_Isaac.SM_Isaac'"));
  if (skeletalMesh.Succeeded()) GetMesh()->SetSkeletalMesh(skeletalMesh.Object);

  // Setup animation blueprint
  static ConstructorHelpers::FClassFinder<UTorchAnimationInstance> animationClass(TEXT("/Game/Build/Characters/DeadSpace/Isaac/AB_Isaac"));
  if (animationClass.Succeeded()) GetMesh()->SetAnimClass(animationClass.Class);

  // Setup animation montages
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageForward(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Isaac/Montages/AM_Isaac_Walk_Fwd.AM_Isaac_Walk_Fwd'"));
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageBack(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Isaac/Montages/AM_Isaac_Walk_Bwd.AM_Isaac_Walk_Bwd'"));
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageRight(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Isaac/Montages/AM_Isaac_Walk_Right.AM_Isaac_Walk_Right'"));
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageLeft(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Isaac/Montages/AM_Isaac_Walk_Left.AM_Isaac_Walk_Left'"));
  //static ConstructorHelpers::FObjectFinder<UAnimMontage> montageForwardLeft(TEXT(""));
  //static ConstructorHelpers::FObjectFinder<UAnimMontage> montageForwardRight(TEXT(""));
  //static ConstructorHelpers::FObjectFinder<UAnimMontage> montageBackLeft(TEXT(""));
  //static ConstructorHelpers::FObjectFinder<UAnimMontage> montageBackRight(TEXT(""));

  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageRun(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Isaac/Montages/AM_Isaac_Run.AM_Isaac_Run'"));
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageJump(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Isaac/Montages/AM_Isaac_Jump.AM_Isaac_Jump'"));
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageTurn(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Isaac/Montages/AM_Isaac_Turn.AM_Isaac_Turn'"));

  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageWeaponHip(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Isaac/Montages/AM_Isaac_WEapon_Hip.AM_Isaac_Weapon_Hip'"));

  if (montageForward.Succeeded()) mMontageForward = montageForward.Object;
  if (montageBack.Succeeded()) mMontageBack = montageBack.Object;
  if (montageRight.Succeeded()) mMontageRight = montageRight.Object;
  if (montageLeft.Succeeded()) mMontageLeft = montageLeft.Object;
  //if (montageForwardLeft.Succeeded()) mMontageForwardLeft = montageForwardLeft.Object;
  //if (montageForwardRight.Succeeded()) mMontageForwardRight = montageForwardRight.Object;
  //if (montageBackLeft.Succeeded()) mMontageBackLeft = montageBackLeft.Object;
  //if (montageBackRight.Succeeded()) mMontageBackRight = montageBackRight.Object;

  if (montageRun.Succeeded()) mMontageRun = montageRun.Object;
  if (montageJump.Succeeded()) mMontageJump = montageJump.Object;
  if (montageTurn.Succeeded()) mMontageTurn = montageTurn.Object;

  if (montageWeaponHip.Succeeded()) mMontageWeaponHip = montageWeaponHip.Object;

  // Setup animation curves
  static ConstructorHelpers::FObjectFinder<UCurveFloat> curveFloatRecoil(TEXT("CurveFloat'/Game/Torch/Characters/Animations/FC_Recoil.FC_Recoil'"));
  if (curveFloatRecoil.Succeeded()) mCurveFloatRecoil = curveFloatRecoil.Object;
}

void ATorchTPSCharacter::BeginPlay()
{
  Super::BeginPlay();

  // Configure custom animation instance
  mAnimationInstance = Cast<UTorchAnimationInstance>(GetMesh()->GetAnimInstance());
  mAnimationInstance->RootMotionMode = ERootMotionMode::RootMotionFromEverything;

  // Configure initial turn in place
  mLowerBodyRotationTarget = GetActorRotation();
  mLowerBodyRotationTarget.Yaw += -90.0f;
  mLowerBodyRotationDelta = mLowerBodyRotationTarget;
  mUpperBodyRotationDelta = mLowerBodyRotationTarget;

  // Configure material parameter collection
  //if (mMaterialParameterCollection)
  //{
  //  mMaterialParameterCollectionInstance = GetWorld()->GetParameterCollectionInstance(mMaterialParameterCollection);
  //}
}
void ATorchTPSCharacter::Tick(float deltaTime)
{
  Super::Tick(deltaTime);

  SetPrimitiveByName<FFloatProperty>(mAnimationInstance, TEXT("Speed"), GetVelocity().Size());

  UpdateLocomotion();
  UpdateTurnInPlace(90.0f, -90.0f);
  //UpdateAimOffset();

  UpdateSpineIK();
  UpdateHandIK();
  UpdateFootIK();

  UpdateCamera();
  UpdateWeapon();
  UpdateMaterial();
}
void ATorchTPSCharacter::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
  Super::SetupPlayerInputComponent(playerInputComponent);

  playerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATorchTPSCharacter::MoveForward);
  playerInputComponent->BindAxis(TEXT("MoveRight"), this, &ATorchTPSCharacter::MoveRight);

  playerInputComponent->BindAxis(TEXT("TurnHorizontal"), this, &ATorchTPSCharacter::TurnHorizontal);
  playerInputComponent->BindAxis(TEXT("TurnVertical"), this, &ATorchTPSCharacter::TurnVertical);

  playerInputComponent->BindAction(TEXT("Forward"), EInputEvent::IE_Pressed, this, &ATorchTPSCharacter::BeginForward);
  playerInputComponent->BindAction(TEXT("Forward"), EInputEvent::IE_Released, this, &ATorchTPSCharacter::EndForward);
  playerInputComponent->BindAction(TEXT("Back"), EInputEvent::IE_Pressed, this, &ATorchTPSCharacter::BeginBack);
  playerInputComponent->BindAction(TEXT("Back"), EInputEvent::IE_Released, this, &ATorchTPSCharacter::EndBack);
  playerInputComponent->BindAction(TEXT("Right"), EInputEvent::IE_Pressed, this, &ATorchTPSCharacter::BeginRight);
  playerInputComponent->BindAction(TEXT("Right"), EInputEvent::IE_Released, this, &ATorchTPSCharacter::EndRight);
  playerInputComponent->BindAction(TEXT("Left"), EInputEvent::IE_Pressed, this, &ATorchTPSCharacter::BeginLeft);
  playerInputComponent->BindAction(TEXT("Left"), EInputEvent::IE_Released, this, &ATorchTPSCharacter::EndLeft);

  playerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &ATorchTPSCharacter::BeginRun);
  playerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released, this, &ATorchTPSCharacter::EndRun);

  playerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Pressed, this, &ATorchTPSCharacter::BeginAim);
  playerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Released, this, &ATorchTPSCharacter::EndAim);

  playerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &ATorchTPSCharacter::BeginFire);
  playerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, &ATorchTPSCharacter::EndFire);

  playerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &ATorchTPSCharacter::BeginReload);

  playerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Pressed, this, &ATorchTPSCharacter::BeginCrouch);
  playerInputComponent->BindAction(TEXT("Crouch"), EInputEvent::IE_Released, this, &ATorchTPSCharacter::EndCrouch);

  playerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ATorchTPSCharacter::BeginJump);
  playerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ATorchTPSCharacter::EndJump);
}

void ATorchTPSCharacter::UpdateLocomotion()
{
  //TORCH_LOG("F:%d B:%d R:%d L:%d FL:%d FR:%d BL:%d BR:%d", mLocomotionForward, mLocomotionBack, mLocomotionRight, mLocomotionLeft, mLocomotionForwardRight, mLocomotionForwardLeft, mLocomotionBackLeft, mLocomotionBackRight);

  // Select locomotion montage
  if (mReevaluate)
  {
    mReevaluate = false;

    if (mIsRunning && mLocomotionForward)
    {
      mAnimationInstance->Play(mMontageRun, TEXT("Left"), 1.0f);
    }
    else
    {
      if (mLocomotionForward)
      {
        if (mLocomotionLeft)
        {
          mAnimationInstance->Play(mMontageLeft, TEXT("Walk"), 1.0f);
        }
        else if (mLocomotionRight)
        {
          mAnimationInstance->Play(mMontageRight, TEXT("Walk"), 1.0f);
        }
        else
        {
          mAnimationInstance->Play(mMontageForward, TEXT("Walk"), 1.0f);
        }
      }
      else if (mLocomotionBack)
      {
        if (mLocomotionLeft)
        {
          mAnimationInstance->Play(mMontageLeft, TEXT("Walk"), 1.0f);
        }
        else if (mLocomotionRight)
        {
          mAnimationInstance->Play(mMontageRight, TEXT("Walk"), 1.0f);
        }
        else
        {
          mAnimationInstance->Play(mMontageBack, TEXT("Walk"), 1.0f);
        }
      }
      else if (mLocomotionLeft)
      {
        mAnimationInstance->Play(mMontageLeft, TEXT("Walk"), 1.0f);
      }
      else if (mLocomotionRight)
      {
        mAnimationInstance->Play(mMontageRight, TEXT("Walk"), 1.0f);
      }
      else
      {
        mAnimationInstance->Stop(mMontageRun);
        mAnimationInstance->Stop(mMontageForward);
        mAnimationInstance->Stop(mMontageBack);
        mAnimationInstance->Stop(mMontageLeft);
        mAnimationInstance->Stop(mMontageRight);
      }
    }
  }

  //if (mAnimationInstance->GetCurveValue(TEXT("MotionCorrection"), motionCorrection))
  //{
  //  TORCH_LOG("Curve %f", motionCorrection);
  //  //GetMesh()->RootBoneTranslation += FVector{ motionCorrection, 0.0f, 0.0f };
  //}
}
void ATorchTPSCharacter::UpdateTurnInPlace(float maxTurnAngle, float angleOffset)
{
  mUpperBodyRotationTarget = GetActorRotation();
  mUpperBodyRotationTarget.Yaw += angleOffset;
  if (GetVelocity().Size() > 0.0f)
  {
    // Rotate towards camera looking at
    mLowerBodyRotationTarget = mUpperBodyRotationTarget;
  }
  else
  {
    // Rotate lower and upper relative to actor rotation
    FRotator deltaRotation = (mLowerBodyRotationTarget.Quaternion() * mUpperBodyRotationTarget.Quaternion().Inverse()).Rotator().GetNormalized();
    if (deltaRotation.Yaw > maxTurnAngle)
    {
      mLowerBodyRotationTarget.Yaw += -maxTurnAngle;
      mUpperBodyRotationTarget.Yaw = mLowerBodyRotationTarget.Yaw;
      mAnimationInstance->Play(mMontageTurn, TEXT("Right"), 0.65f);
    }
    else if (deltaRotation.Yaw < -maxTurnAngle)
    {
      mLowerBodyRotationTarget.Yaw += maxTurnAngle;
      mUpperBodyRotationTarget.Yaw = mLowerBodyRotationTarget.Yaw;
      mAnimationInstance->Play(mMontageTurn, TEXT("Left"), 0.65f);
    }
  }
  mUpperBodyRotationDelta = FMath::RInterpTo(mUpperBodyRotationDelta, mUpperBodyRotationTarget, GetWorld()->GetDeltaSeconds(), 20.0f);
  mLowerBodyRotationDelta = FMath::RInterpTo(mLowerBodyRotationDelta, mLowerBodyRotationTarget, GetWorld()->GetDeltaSeconds(), 5.0f);
  SetPrimitiveByName<FFloatProperty>(mAnimationInstance, TEXT("UpperBodyYaw"), mUpperBodyRotationDelta.Yaw);
  SetPrimitiveByName<FFloatProperty>(mAnimationInstance, TEXT("LowerBodyYaw"), mLowerBodyRotationDelta.Yaw);
}
void ATorchTPSCharacter::UpdateAimOffset()
{
  static float yaw = 0.0f;
  static float yawLastFrame = 0.0f;
  static float yawOffset = 0.0f;
  static float pitch = 0.0f;
  static float pitchLastFrame = 0.0f;
  static float pitchOffset = 0.0f;
  yawLastFrame = yaw;
  yaw = mCameraRotation.Yaw;
  float yawChangeFrame = yawLastFrame - yaw;
  yawOffset += yawChangeFrame;
  yawOffset = FMath::Clamp(yawOffset, -180.0f, 180.0f);
  pitchLastFrame = pitch;
  pitch = mCameraRotation.Pitch;
  float pitchChangeFrame = pitchLastFrame - pitch;
  pitchOffset += pitchChangeFrame;
  pitchOffset = FMath::Clamp(pitchOffset, -180.0f, 180.0f);
  SetStructureByName<FRotator>(mAnimationInstance, TEXT("CameraRotation"), FRotator{ -pitchOffset, yawOffset, 0.0f });
  TORCH_LOG("Yaw %f", yawOffset);
  TORCH_LOG("Pitch %f", pitchOffset);
}

void ATorchTPSCharacter::UpdateSpineIK()
{
  FVector spineUp = FVector::UpVector;
  float spineUpDistance = 0.0f;

  if (mAiming)
  {
    spineUp = FRotationMatrix{ FRotator{ mCameraRotation.Pitch, 0.0f, 0.0f } }.TransformVector(spineUp);
    spineUp = GetActorRotation().RotateVector(spineUp);

    spineUpDistance = mSpineUpDistance;
  }
  else
  {
    spineUp = FRotationMatrix{ FRotator{ mSpinePitch, 0.0f, 0.0f } }.TransformVector(spineUp);
    spineUp = GetActorRotation().RotateVector(spineUp);

    spineUpDistance = mSpineUpDistance;
  }

  SetStructureByName<FVector>(mAnimationInstance, TEXT("SpineLocation"), GetActorLocation() + spineUp * spineUpDistance);
}
void ATorchTPSCharacter::UpdateHandIK()
{
  FVector handJointTargetLocationL = FVector::ZeroVector;
  FVector handJointTargetLocationR = FVector::ZeroVector;

  FVector handLocationL = FVector::ZeroVector;
  FVector handLocationR = FVector::ZeroVector;

  if (mCurrentWeaponActor)
  {
    //handJointTargetLocationL = GetActorRotation().RotateVector(mCurrentWeaponActor->GetHandJointTargetLocationL());
    //handJointTargetLocationR = GetActorRotation().RotateVector(mCurrentWeaponActor->GetHandJointTargetLocationR());

    //handLocationL = GetActorRotation().RotateVector(mCurrentWeaponActor->GetRestLocationL());
    //handLocationR = GetActorRotation().RotateVector(mCurrentWeaponActor->GetRestLocationR());
  }
  else
  {
    handLocationL = GetActorRotation().RotateVector(mHandLocationL);
    handLocationR = GetActorRotation().RotateVector(mHandLocationR);
  }

  SetStructureByName<FVector>(mAnimationInstance, TEXT("HandJointTargetLocationL"), GetActorLocation() + handJointTargetLocationL);
  SetStructureByName<FVector>(mAnimationInstance, TEXT("HandJointTargetLocationR"), GetActorLocation() + handJointTargetLocationR);

  SetStructureByName<FVector>(mAnimationInstance, TEXT("HandLocationL"), GetActorLocation() + handLocationL);
  SetStructureByName<FVector>(mAnimationInstance, TEXT("HandLocationR"), GetActorLocation() + handLocationR);
}
void ATorchTPSCharacter::UpdateFootIK()
{
  SetStructureByName<FVector>(mAnimationInstance, TEXT("FootEffectorLocationL"), FVector{ 0.0f, 0.0f, 0.0f });
  SetStructureByName<FVector>(mAnimationInstance, TEXT("FootEffectorLocationR"), FVector{ 0.0f, 0.0f, 0.0f });
}

void ATorchTPSCharacter::UpdateCamera()
{
  if (mCameraComponent)
  {
    FRotationMatrix cameraMatrix = FRotationMatrix{ FRotator{ mCameraRotation.Pitch, mUpperBodyRotationTarget.Yaw, 0.0f } };

    //FVector cameraRight = cameraMatrix.GetUnitAxis(EAxis::Y);
    //FVector cameraUp = cameraMatrix.GetUnitAxis(EAxis::Z);
    //FVector cameraForward = cameraMatrix.GetUnitAxis(EAxis::X);

    if (mAiming)
    {
      FVector cameraRight = GetActorRotation().RotateVector(cameraRight) * mCameraAimOffset.X;
      FVector cameraUp = GetActorRotation().RotateVector(cameraUp) * mCameraAimOffset.Y;
      FVector cameraForward = GetActorRotation().RotateVector(cameraForward) * mCameraAimOffset.Z;

      FVector cameraPivot = GetActorRotation().RotateVector(mCameraAimLocation);
      mCameraRotation = FRotationMatrix::MakeFromX(cameraRight).Rotator();
      mCameraLocation = cameraPivot + cameraRight + cameraUp + cameraForward;

      DrawDebugSphere(GetWorld(), GetActorLocation() + cameraPivot, 10.0f, 16, FColor::Green);
      DrawDebugSphere(GetWorld(), GetActorLocation() + mCameraLocation, 10.0f, 16, FColor::Green);

      //FVector cameraRight = FRotator{ mCameraRotation.Pitch, mUpperBodyRotationTarget.Yaw, 0.0f }.RotateVector(FVector::RightVector);
      //mCameraRotation = FRotationMatrix::MakeFromX(cameraRight).Rotator();
      //mCameraLocation = mCameraRotation.RotateVector(mCameraAimLocation);
    }
    else
    {
      FRotationMatrix rotationMatrix = FRotator{ mCameraRotation.Pitch, mUpperBodyRotationTarget.Yaw, 0.0f };
      FVector cameraRight = rotationMatrix.Rotator().RotateVector(FVector::RightVector);
      FVector cameraUp = rotationMatrix.Rotator().RotateVector(FVector::UpVector);
      mCameraRotation = FRotationMatrix::MakeFromYZ(cameraRight, cameraUp).Rotator();
      mCameraRotation.Yaw += 90.0f;
      mCameraLocation = GetActorRotation().RotateVector(mCameraOrbitLocation);
    }

    mCameraComponent->SetWorldLocation(GetActorLocation() + mCameraLocation);
    mCameraComponent->SetRelativeRotation(mCameraRotation);

    // Add default camera shake
    {
      float timeSinceStart = GetWorld()->GetRealTimeSeconds();

      float pitch = FMath::Sin(timeSinceStart * mCameraShakeFrequency.X) * mCameraShakeIntensity.X;
      float yaw = FMath::Cos(timeSinceStart * mCameraShakeFrequency.Y) * mCameraShakeIntensity.Y;
      float roll = (pitch + yaw) * mCameraShakeIntensity.Z;

      mCameraComponent->AddRelativeRotation(FRotator{ pitch, yaw, roll });
    }

    // Add camera bone shake
    {
      FTransform pelvisTransform = GetMesh()->GetSocketTransform(TEXT("pelvis"), ERelativeTransformSpace::RTS_Actor);
      FRotator deltaRotation = FRotator{GetActorRotation().Quaternion() * pelvisTransform.GetRotation().Inverse() };

      float pitch = deltaRotation.Pitch * mCameraAnimationShakeIntensity.X;
      float yaw = deltaRotation.Yaw * mCameraAnimationShakeIntensity.Y;
      float roll = (deltaRotation.Roll + 90.0f) * mCameraAnimationShakeIntensity.Z;

      mCameraComponent->AddRelativeRotation(FRotator{ pitch, roll, 0.0f });
    }
  }
}
void ATorchTPSCharacter::UpdateWeapon()
{
  if (mCurrentWeaponActor)
  {
    if (mAiming)
    {
      // Compute optimal weapon distance
      float zAimOffset = 0.0f;
      if (mCameraRotation.Pitch > 0.0f)
      {
        zAimOffset = FTorchMath::RemapValue(mCameraRotation.Pitch, 0.0f, 90.0f, 40.0f, 20.0f);
      }
      else
      {
        zAimOffset = FTorchMath::RemapValue(mCameraRotation.Pitch, 0.0f, -90.0f, 40.0f, 70.0f);
      }

      // Compute optimal spine Z motion
      FTransform spine3Transform = GetMesh()->GetSocketTransform(TEXT("spine_03"), ERelativeTransformSpace::RTS_Actor);

      FVector spine3Location = GetActorRotation().RotateVector(spine3Transform.GetLocation());

      //FVector weaponRight = mAimRight * mCurrentWeaponActor->GetAimOffset().X;
      //FVector weaponUp = mAimUp * mCurrentWeaponActor->GetAimOffset().Y;
      //FVector weaponForward = mAimForward * mCurrentWeaponActor->GetAimOffset().Z; // Add in Z correction

      //FVector weaponRight = GetMesh()->GetBoneAxis(TEXT("spine_03"), EAxis::Z) * mCurrentWeaponActor->GetAimOffset().X;
      //FVector weaponUp = GetMesh()->GetBoneAxis(TEXT("spine_03"), EAxis::X) * mCurrentWeaponActor->GetAimOffset().Y;
      //FVector weaponForward = GetMesh()->GetBoneAxis(TEXT("spine_03"), EAxis::Y) * mCurrentWeaponActor->GetAimOffset().Z;

      FVector weaponPivot = GetActorRotation().RotateVector(mCameraAimLocation); // Add in spine Z motion
      //FVector weaponLocation = weaponPivot + weaponRight + weaponUp + weaponForward;
      //FVector weaponLocation = spine3Location + weaponUp;

      //DrawDebugSphere(GetWorld(), GetActorLocation() + weaponPivot, 10.0f, 16, FColor::Red);
      //DrawDebugSphere(GetWorld(), GetActorLocation() + weaponLocation, 10.0f, 16, FColor::Red);

      //mCurrentWeaponActor->SetActorLocation(GetActorLocation() + weaponLocation);
      //mCurrentWeaponActor->SetActorRelativeRotation(FRotator{ 0.0f, 0.0f, -mCameraRotation.Pitch });
    }
    else
    {
      //mCurrentWeaponActor->SetActorRelativeLocation(mCurrentWeaponActor->GetRestLocation());
      //mCurrentWeaponActor->SetActorRelativeRotation(FRotator{ 0.0f, 0.0f, 0.0f });
    }
  }
}
void ATorchTPSCharacter::UpdateMaterial()
{
  // Update material parameter collection
  if (mMaterialParameterCollectionInstance)
  {
    mMaterialParameterCollectionInstance->SetVectorParameterValue(TEXT("EffectorLocation"), GetActorLocation());
    mMaterialParameterCollectionInstance->SetScalarParameterValue(TEXT("EffectorRadius"), 500.0f);
    mMaterialParameterCollectionInstance->SetScalarParameterValue(TEXT("EffectorHardness"), 1.0f);
  }
}

void ATorchTPSCharacter::MoveForward(float value)
{
  if (value != 0.0f)
  {
    //AddMovementInput(GetActorForwardVector(), value);
  }
}
void ATorchTPSCharacter::MoveRight(float value)
{
  if (value != 0.0f)
  {
    //AddMovementInput(GetActorRightVector(), value);
  }
}

void ATorchTPSCharacter::TurnHorizontal(float value)
{
  if (value != 0.0f)
  {
    AddControllerYawInput(value);

    // Update camera rotation
    mCameraRotation.Yaw += mCameraInvertYaw ? -value : value;
    if (!mCameraOrbitIgnoreYawConstraints)
    {
      mCameraRotation.Yaw = FMath::Max(mCameraOrbitMinYaw, FMath::Min(mCameraOrbitMaxYaw, mCameraRotation.Yaw));
    }
  }
}
void ATorchTPSCharacter::TurnVertical(float value)
{
  if (value != 0.0f)
  {
    // Update camera rotation
    mCameraRotation.Pitch += mCameraInvertPitch ? -value : value;
    if (!mCameraOrbitIgnorePitchConstraints)
    {
      mCameraRotation.Pitch = FMath::Max(mCameraOrbitMinPitch, FMath::Min(mCameraOrbitMaxPitch, mCameraRotation.Pitch));
    }
  }
}

void ATorchTPSCharacter::BeginForward()
{
  mLocomotionForward = true;
  mReevaluate = true;
}
void ATorchTPSCharacter::EndForward()
{
  mLocomotionForward = false;
  mReevaluate = true;
}
void ATorchTPSCharacter::BeginBack()
{
  mLocomotionBack = true;
  mReevaluate = true;
}
void ATorchTPSCharacter::EndBack()
{
  mLocomotionBack = false;
  mReevaluate = true;
}
void ATorchTPSCharacter::BeginRight()
{
  mLocomotionRight = true;
  mReevaluate = true;
}
void ATorchTPSCharacter::EndRight()
{
  mLocomotionRight = false;
  mReevaluate = true;
}
void ATorchTPSCharacter::BeginLeft()
{
  mLocomotionLeft = true;
  mReevaluate = true;
}
void ATorchTPSCharacter::EndLeft()
{
  mLocomotionLeft = false;
  mReevaluate = true;
}

void ATorchTPSCharacter::BeginAim()
{
  mAiming = true;
  SetPrimitiveByName<FBoolProperty>(mAnimationInstance, TEXT("IsAiming"), true);

  // Rotate towards camera forward
  mCameraRotation.Yaw = 0.0f;
  
}
void ATorchTPSCharacter::EndAim()
{
  mAiming = false;
  SetPrimitiveByName<FBoolProperty>(mAnimationInstance, TEXT("IsAiming"), false);

  // Rotate towards camera forward
  mCameraRotation.Yaw = 0.0f;
}

void ATorchTPSCharacter::BeginRun()
{
  mIsRunning = true;
  mReevaluate = true;
}
void ATorchTPSCharacter::EndRun()
{
  mIsRunning = false;
  mReevaluate = true;
}

void ATorchTPSCharacter::BeginFire()
{
  if (mCurrentWeaponActor)
  {
    mCurrentWeaponActor->EnableFire();
  }
  SetPrimitiveByName<FBoolProperty>(mAnimationInstance, TEXT("IsFiring"), true);
}
void ATorchTPSCharacter::EndFire()
{
  if (mCurrentWeaponActor)
  {
    mCurrentWeaponActor->DisableFire();
  }
  SetPrimitiveByName<FBoolProperty>(mAnimationInstance, TEXT("IsFiring"), false);
}

void ATorchTPSCharacter::BeginReload()
{
  
}

void ATorchTPSCharacter::BeginCrouch()
{
  Crouch();
}
void ATorchTPSCharacter::EndCrouch()
{
  UnCrouch();
}

void ATorchTPSCharacter::BeginJump()
{
  bPressedJump = true;
  mAnimationInstance->Play(mMontageJump, TEXT("Begin"), 1.0f, true); // fix me!
}
void ATorchTPSCharacter::EndJump()
{
  bPressedJump = false;
  mReevaluate = true;
}