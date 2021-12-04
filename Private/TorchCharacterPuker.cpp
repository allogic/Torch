#include "TorchCharacterPuker.h"
#include "Components/CapsuleComponent.h"

ATorchCharacterPuker::ATorchCharacterPuker(const FObjectInitializer& initializer)
  : ATorchCharacter{ initializer }
{
  // Setup actor
  SetActorScale3D(FVector{ 2.0f });

  // Setup capsule
  GetCapsuleComponent()->SetCapsuleHalfHeight(92.0f);
  GetCapsuleComponent()->SetCapsuleRadius(35.0f);

  // Setup skeletal mesh
  GetMesh()->SetRelativeLocationAndRotation(FVector{ 0.0f, 0.0f, -88.0f }, FRotator{ 0.0f, -90.0f, 0.0f });
  static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMesh(TEXT("SkeletalMesh'/Game/Build/Characters/DeadSpace/Puker/SM_Puker.SM_Puker'"));
  if (skeletalMesh.Succeeded()) GetMesh()->SetSkeletalMesh(skeletalMesh.Object);

  // Setup animation blueprint
  static ConstructorHelpers::FClassFinder<UTorchAnimationInstance> animationInstance(TEXT("/Game/Build/Characters/DeadSpace/Puker/AB_Puker"));
  if (animationInstance.Succeeded()) GetMesh()->SetAnimInstanceClass(animationInstance.Class);

  // Setup animation montages
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageWalk(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Puker/Montages/AM_Puker_Walk.AM_Puker_Walk'"));
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageWalkAggressive(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Puker/Montages/AM_Puker_Walk_Aggressive.AM_Puker_Walk_Aggressive'"));
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageRun(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Puker/Montages/AM_Puker_Run.AM_Puker_Run'"));

  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageTurnLeft90(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Puker/Montages/AM_Puker_Turn_Left_90.AM_Puker_Turn_Left_90'"));
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageTurnLeft180(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Puker/Montages/AM_Puker_Turn_Left_180.AM_Puker_Turn_Left_180'"));
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageTurnRight90(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Puker/Montages/AM_Puker_Turn_Right_90.AM_Puker_Turn_Right_90'"));
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageTurnRight180(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Puker/Montages/AM_Puker_Turn_Right_180.AM_Puker_Turn_Right_180'"));

  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageEvadeLeft(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Puker/Montages/AM_Puker_Evade_Left.AM_Puker_Evade_Left'"));
  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageEvadeRight(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Puker/Montages/AM_Puker_Evade_Right.AM_Puker_Evade_Right'"));

  static ConstructorHelpers::FObjectFinder<UAnimMontage> montageAttack(TEXT("AnimMontage'/Game/Build/Characters/DeadSpace/Puker/Montages/AM_Puker_Attack.AM_Puker_Attack'"));

  if (montageWalk.Succeeded()) mMontageWalk = montageWalk.Object;
  if (montageWalkAggressive.Succeeded()) mMontageWalkAggressive = montageWalkAggressive.Object;
  if (montageRun.Succeeded()) mMontageRun = montageRun.Object;

  if (montageTurnLeft90.Succeeded()) mMontageTurnLeft90 = montageTurnLeft90.Object;
  if (montageTurnLeft180.Succeeded()) mMontageTurnLeft180 = montageTurnLeft180.Object;
  if (montageTurnRight90.Succeeded()) mMontageTurnRight90 = montageTurnRight90.Object;
  if (montageTurnRight180.Succeeded()) mMontageTurnRight180 = montageTurnRight180.Object;

  if (montageEvadeLeft.Succeeded()) mMontageEvadeLeft = montageEvadeLeft.Object;
  if (montageEvadeRight.Succeeded()) mMontageEvadeRight = montageEvadeRight.Object;

  if (montageAttack.Succeeded()) mMontageAttack = montageAttack.Object;
}

void ATorchCharacterPuker::BeginPlay()
{
  Super::BeginPlay();

  // Setup physical animation
  FPhysicalAnimationData physicalAnimationFoot{};
  physicalAnimationFoot.bIsLocalSimulation = true;
  physicalAnimationFoot.OrientationStrength = 2000.0f;
  physicalAnimationFoot.AngularVelocityStrength = 200.0f;
  physicalAnimationFoot.PositionStrength = 2000.0f;
  physicalAnimationFoot.VelocityStrength = 200.0f;
  FPhysicalAnimationData physicalAnimationFlesh{};
  physicalAnimationFlesh.bIsLocalSimulation = true;
  physicalAnimationFlesh.OrientationStrength = 30.0f;
  physicalAnimationFlesh.AngularVelocityStrength = 3.0f;
  physicalAnimationFlesh.PositionStrength = 30.0f;
  physicalAnimationFlesh.VelocityStrength = 3.0f;
  FPhysicalAnimationData physicalAnimationArmL{};
  physicalAnimationArmL.bIsLocalSimulation = false;
  physicalAnimationArmL.OrientationStrength = 2000.0f;
  physicalAnimationArmL.AngularVelocityStrength = 200.0f;
  physicalAnimationArmL.PositionStrength = 2000.0f;
  physicalAnimationArmL.VelocityStrength = 200.0f;
  FPhysicalAnimationData physicalAnimationArmR{};
  physicalAnimationArmR.bIsLocalSimulation = false;
  physicalAnimationArmR.OrientationStrength = 10000.0f;
  physicalAnimationArmR.AngularVelocityStrength = 1000.0f;
  physicalAnimationArmR.PositionStrength = 10000.0f;
  physicalAnimationArmR.VelocityStrength = 1000.0f;
  FPhysicalAnimationData physicalAnimationSpine{};
  physicalAnimationSpine.bIsLocalSimulation = false;
  physicalAnimationSpine.OrientationStrength = 3000.0f;
  physicalAnimationSpine.AngularVelocityStrength = 300.0f;
  physicalAnimationSpine.PositionStrength = 3000.0f;
  physicalAnimationSpine.VelocityStrength = 300.0f;
  mPhysicalAnimationComponent->SetSkeletalMeshComponent(GetMesh());
  mPhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(TEXT("spine_01"), physicalAnimationSpine);
  mPhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(TEXT("foot_add"), physicalAnimationFoot);
  mPhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(TEXT("clavicle_l"), physicalAnimationArmR);
  mPhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(TEXT("clavicle_r"), physicalAnimationArmR);
  mPhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(TEXT("loop_foot"), physicalAnimationFlesh);
  mPhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(TEXT("loop_arm"), physicalAnimationFlesh);
  mPhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(TEXT("loop_foot"), physicalAnimationFlesh);
  mPhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(TEXT("flesh_arm"), physicalAnimationFlesh);
  mPhysicalAnimationComponent->ApplyPhysicalAnimationSettingsBelow(TEXT("flesh_chest"), physicalAnimationFlesh);
  GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("spine_01"), true);
}