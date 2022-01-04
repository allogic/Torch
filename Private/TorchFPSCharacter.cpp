#include "TorchFPSCharacter.h"
#include "TorchBlueprintUtils.h"
#include "TorchWeapon.h"
#include "TorchCore.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"

bool ATorchFPSCharacter::AttachWeapon(ATorchWeapon* weapon)
{
  if (!mWeapon)
  {
    mWeapon = weapon;
    mWeapon->OnAttach();
    mWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r"));
    return true;
  }
  return false;
}
bool ATorchFPSCharacter::DetachWeapon()
{
  if (mWeapon)
  {
    mWeapon->DetachFromActor(FDetachmentTransformRules{ FAttachmentTransformRules::SnapToTargetIncludingScale, false });
    mWeapon->OnDetach();
    mWeapon = nullptr;
    return true;
  }
  return false;
}

void ATorchFPSCharacter::EnableHandIK()
{
  mEnableHandIK = true;
}
void ATorchFPSCharacter::DisableHandIK()
{
  mEnableHandIK = false;
}

ATorchFPSCharacter::ATorchFPSCharacter()
{
  // Setup actor
 	PrimaryActorTick.bCanEverTick = true;
  PrimaryActorTick.bStartWithTickEnabled = true;

  // Setup movement
  bUseControllerRotationYaw = true;
  GetCharacterMovement()->NavAgentProps.bCanJump = true;
  GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
  GetCharacterMovement()->JumpZVelocity = 500.0f;
  GetCharacterMovement()->MaxWalkSpeed = 600.0f;
  GetCharacterMovement()->MaxWalkSpeedCrouched = 60.0f;

  // Setup camera
  mCameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
  mCameraComponent->SetupAttachment(RootComponent);
  mCameraComponent->bUsePawnControlRotation = true;

  // Setup capsule
  GetCapsuleComponent()->SetCollisionProfileName(TEXT("Custom"));
  GetCapsuleComponent()->SetRelativeScale3D(FVector{ 1.0f });
  GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Overlap);

  // Setup perception sphere
  mPerceptionSphereComponent = CreateDefaultSubobject<USphereComponent>("PerceptionSphere");
  mPerceptionSphereComponent->SetupAttachment(RootComponent);
  mPerceptionSphereComponent->SetSphereRadius(mPerceptionSphereRadius);
  mPerceptionSphereComponent->SetCollisionProfileName(TEXT("Custom"));
  mPerceptionSphereComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel4);
  mPerceptionSphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  mPerceptionSphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Overlap);
  mPerceptionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATorchFPSCharacter::OnBeginOverlap);
  mPerceptionSphereComponent->OnComponentEndOverlap.AddDynamic(this, &ATorchFPSCharacter::OnEndOverlap);

  // Setup skeletal mesh
  GetMesh()->SetRelativeRotation(FRotator{ 0.0f, -90.0f, 0.0f });
  GetMesh()->SetupAttachment(RootComponent);
  GetMesh()->SetCollisionProfileName(TEXT("Custom"));
  GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
  GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);
  static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh(TEXT("SkeletalMesh'/Game/M4_Animations/EpicContent/Character/Mesh/SK_Mannequin_Arms.SK_Mannequin_Arms'"));
  if (skeletalMesh.Succeeded()) GetMesh()->SetSkeletalMesh(skeletalMesh.Object);

  // Setup animation blueprint
  static ConstructorHelpers::FClassFinder<UTorchAnimationInstance> animationClass(TEXT("/Game/Build/Characters/DeadSpace/IsaacArms/AB_Isaac_Arms"));
  if (animationClass.Succeeded()) GetMesh()->SetAnimClass(animationClass.Class);

  // Setup animation montages
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageWalk(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/IsaacArms/Montages/AM_Isaac_Arm_Walk.AM_Isaac_Arm_Walk'"));
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageReload(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/IsaacArms/Montages/AM_Isaac_Arm_Realod.AM_Isaac_Arm_Realod'"));
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageAiming(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/IsaacArms/Montages/AM_Isaac_Arm_Aiming.AM_Isaac_Arm_Aiming'"));
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageFiring(TEXT(""));
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageMisc(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/IsaacArms/Montages/AM_Isaac_Arm_Misc.AM_Isaac_Arm_Misc'"));
  if (montageWalk.Succeeded()) mMontageWalk = montageWalk.Object;
  if (montageReload.Succeeded()) mMontageReload = montageReload.Object;
  if (montageAiming.Succeeded()) mMontageAiming = montageAiming.Object;
  if (montageFiring.Succeeded()) mMontageFiring = montageFiring.Object;
  if (montageMisc.Succeeded()) mMontageMisc = montageMisc.Object;
}

void ATorchFPSCharacter::BeginPlay()
{
  Super::BeginPlay();

  // Enable physics on arms
  GetMesh()->SetSimulatePhysics(true);

  // Configure custom animation instance
  mAnimationInstance = Cast<UTorchAnimationInstance>(GetMesh()->GetAnimInstance());

  // Rest hand IK bones
  mHandRRestLocation = GetMesh()->GetSocketTransform(TEXT("hand_r"), ERelativeTransformSpace::RTS_Component).GetLocation();
  mHandLRestLocation = GetMesh()->GetSocketTransform(TEXT("hand_l"), ERelativeTransformSpace::RTS_Component).GetLocation();
  EnableHandIK();
}
void ATorchFPSCharacter::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
  Super::SetupPlayerInputComponent(playerInputComponent);

  playerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATorchFPSCharacter::MoveForward);
  playerInputComponent->BindAxis(TEXT("MoveRight"), this, &ATorchFPSCharacter::MoveRight);

  playerInputComponent->BindAxis(TEXT("TurnHorizontal"), this, &ATorchFPSCharacter::TurnHorizontal);
  playerInputComponent->BindAxis(TEXT("TurnVertical"), this, &ATorchFPSCharacter::TurnVertical);

  playerInputComponent->BindAction(TEXT("Forward"), EInputEvent::IE_Pressed, this, &ATorchFPSCharacter::BeginForward);
  playerInputComponent->BindAction(TEXT("Forward"), EInputEvent::IE_Released, this, &ATorchFPSCharacter::EndForward);

  playerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Pressed, this, &ATorchFPSCharacter::BeginReload);
  playerInputComponent->BindAction(TEXT("Reload"), EInputEvent::IE_Released, this, &ATorchFPSCharacter::EndReload);

  playerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ATorchFPSCharacter::BeginJump);
  playerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &ATorchFPSCharacter::EndJump);

  playerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Pressed, this, &ATorchFPSCharacter::BeginAim);
  playerInputComponent->BindAction(TEXT("Aim"), EInputEvent::IE_Released, this, &ATorchFPSCharacter::EndAim);

  playerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &ATorchFPSCharacter::BeginFire);
  playerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, &ATorchFPSCharacter::EndFire);

  playerInputComponent->BindAction(TEXT("Inspect"), EInputEvent::IE_Pressed, this, &ATorchFPSCharacter::BeginInspect);

  playerInputComponent->BindAction(TEXT("Grab"), EInputEvent::IE_Pressed, this, &ATorchFPSCharacter::BeginGrab);
  playerInputComponent->BindAction(TEXT("Drop"), EInputEvent::IE_Pressed, this, &ATorchFPSCharacter::BeginDrop);
}
void ATorchFPSCharacter::Tick(float deltaTime)
{
	 Super::Tick(deltaTime);

  UpdateArms();
  UpdateCamera();
  UpdateWeapon();

  UpdateAnimationBlueprint();
}

void ATorchFPSCharacter::UpdateAnimationBlueprint()
{
  SetStructureByName(mAnimationInstance, TEXT("UpperArmROffset"), mUpperArmROffset + (mEnableHandIK ? FVector::ZeroVector : mVelocityOffset));
  SetStructureByName(mAnimationInstance, TEXT("UpperArmLOffset"), mUpperArmLOffset + (mEnableHandIK ? FVector::ZeroVector : mVelocityOffset));
  SetStructureByName(mAnimationInstance, TEXT("HandRLocation"), mUpperArmROffset + mHandRRestLocation + mHandROffset + mVelocityOffset + mCurrHandAimOffset + mRecoilVelocity);
  SetStructureByName(mAnimationInstance, TEXT("HandLLocation"), mUpperArmLOffset + mHandLRestLocation + mHandLOffset + mVelocityOffset + mCurrHandAimOffset + mRecoilVelocity);
  SetStructureByName(mAnimationInstance, TEXT("HandRRotation"), mCurrHandRAimRotation);
  SetStructureByName(mAnimationInstance, TEXT("HandLRotation"), FRotator::ZeroRotator);
  SetPrimitiveByName<FFloatProperty>(GetMesh()->GetAnimInstance(), TEXT("HandRIKAlpha"), mHandRIKAlpha);
  SetPrimitiveByName<FFloatProperty>(GetMesh()->GetAnimInstance(), TEXT("HandLIKAlpha"), mHandLIKAlpha);
}

void ATorchFPSCharacter::UpdateArms()
{
  // Compute velocity offset
  mArmVelocity -= GetVelocity().GetSafeNormal() * mArmVelocityIntensity;
  mArmVelocity = FMath::Lerp(mArmVelocity, FVector::ZeroVector, (mAiming ? mArmVelocityDampingWhileAiming : mArmVelocityDamping));
  mVelocityOffset = GetActorRotation().UnrotateVector(mArmVelocity);
  mVelocityOffset = mArmRotationOffset.RotateVector(mVelocityOffset);

  // Rotate mesh
  FRotator relativeRotation = FRotator{ mArmRotationOffset.Pitch, mArmRotationOffset.Yaw, -GetControlRotation().Pitch + mArmRotationOffset.Roll };
  GetMesh()->SetRelativeRotation(relativeRotation);

  // Update hand IK alphas
  mHandRIKAlpha = FMath::Lerp(mHandRIKAlpha, (float)mEnableHandIK, mHandRIKAlphaDamping);
  mHandLIKAlpha = FMath::Lerp(mHandLIKAlpha, (float)mEnableHandIK, mHandLIKAlphaDamping);

  // Update aiming offsets and rotation
  mCurrHandAimOffset = FMath::Lerp(mCurrHandAimOffset, (mAiming ? mHandAimOffset : FVector::ZeroVector), mAimDamping);
  mCurrHandRAimRotation = FMath::Lerp(mCurrHandRAimRotation, (mAiming ? mHandRAimRotationOffset : FRotator::ZeroRotator), 0.5f);

  // Compute recoil
  mRecoilVelocity = FMath::Lerp(mRecoilVelocity, FVector::ZeroVector, mRecoilDamping);
}

void ATorchFPSCharacter::UpdateCamera()
{
  // Translate camera
  mCameraComponent->SetRelativeLocation(GetMesh()->GetRelativeRotation().RotateVector(mCameraRelativeLocation));
}
void ATorchFPSCharacter::UpdateWeapon()
{
  if (mWeapon)
  {
    mWeapon->SetActorRelativeLocation(mWeapon->GetRelativeLocation());
    mWeapon->SetActorRelativeRotation(mWeapon->GetRelativeRotation());
    mWeapon->SetActorScale3D(FVector{ 1.5f });
  }
}

void ATorchFPSCharacter::OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
  TORCH_LOG("overlap begin");
  if (otherActor != nullptr && otherActor != this)
  {
    // Found weapon
    if (ATorchWeapon* weapon = Cast<ATorchWeapon>(otherActor))
    {
      mWeapons.Add(weapon);
    }
  }
}
void ATorchFPSCharacter::OnEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
  TORCH_LOG("overlap end");
  if (otherActor != nullptr && otherActor != this)
  {
    // Lost weapon
    if (ATorchWeapon* weapon = Cast<ATorchWeapon>(otherActor))
    {
      mWeapons.Remove(weapon);
    }
  }
}

void ATorchFPSCharacter::MoveForward(float value)
{
  if (value != 0.0f)
  {
    AddMovementInput(GetActorForwardVector(), value);
  }
}
void ATorchFPSCharacter::MoveRight(float value)
{
  if (value != 0.0f)
  {
    AddMovementInput(GetActorRightVector(), value);
  }
}

void ATorchFPSCharacter::TurnHorizontal(float value)
{
  if (value != 0.0f)
  {
    AddControllerYawInput(value);
  }
}
void ATorchFPSCharacter::TurnVertical(float value)
{
  if (value != 0.0f)
  {
    AddControllerPitchInput(value);
  }
}

void ATorchFPSCharacter::BeginForward()
{
  if (mAnimationInstance)
  {
    mAnimationInstance->Play(mMontageWalk);
  }
}
void ATorchFPSCharacter::EndForward()
{
  if (mAnimationInstance)
  {
    if (mAnimationInstance->Montage_IsPlaying(mMontageWalk))
    {
      mAnimationInstance->Montage_StopWithBlendOut(FAlphaBlendArgs{ 0.2f }, mMontageWalk);
    }
  }
}

void ATorchFPSCharacter::BeginReload()
{
  if (mAnimationInstance && mWeapon)
  {
    mAnimationInstance->Play(mMontageReload, TEXT(""), 1.0f, true);
  }
}
void ATorchFPSCharacter::EndReload()
{

}

void ATorchFPSCharacter::BeginJump()
{
  bPressedJump = true;
}
void ATorchFPSCharacter::EndJump()
{
  bPressedJump = false;
}

void ATorchFPSCharacter::BeginAim()
{
  mAiming = true;
}
void ATorchFPSCharacter::EndAim()
{
  mAiming = false;
}

void ATorchFPSCharacter::BeginFire()
{
  if (mWeapon)
  {
    mRecoilVelocity += mRecoilDirection * mRecoilIntensity;
    mWeapon->EnableFire();
  }
}
void ATorchFPSCharacter::EndFire()
{
  if (mWeapon)
  {
    mWeapon->DisableFire();
  }
}

void ATorchFPSCharacter::BeginInspect()
{
  if (mAnimationInstance && mWeapon)
  {
    mAnimationInstance->Play(mMontageMisc, TEXT("Inspect"), 1.0f, true);
  }
}

void ATorchFPSCharacter::BeginGrab()
{
  if (mWeapons.Num() > 0)
  {
    AttachWeapon(mWeapons[FMath::FloorToInt(FMath::RandRange(0, mWeapons.Num() - 1))]);
  }
}
void ATorchFPSCharacter::BeginDrop()
{
  DetachWeapon();
}