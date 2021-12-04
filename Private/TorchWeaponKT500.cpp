#include "TorchWeaponKT500.h"

ATorchWeaponKT500::ATorchWeaponKT500()
{
  // Setup mesh group
  mMeshGroupComponent->SetRelativeRotation(FRotator{ 0.0f, 180.0f, 0.0f });
  mMeshGroupComponent->SetRelativeScale3D(FVector{ 1.1f, 1.1f, 1.1f });

  // Setup static meshes
  mScopeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Scope");
  mSideBodyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("SideBody");
  mClipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Clip");
  mFrontBarrelMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("FrontBarrel");
  mHandGripMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("HandGrip");
  mMainBodyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MainBody");
  mBackBodyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BackBody");
  mButtMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Butt");
  mStockMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Stock");
  mTriggerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Trigger");
  mUnderBarrelMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("UnderBarrel");
  mUnderPartMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("UnderPart");

  mScopeMeshComponent->SetupAttachment(mMeshGroupComponent);
  mSideBodyMeshComponent->SetupAttachment(mMeshGroupComponent);
  mClipMeshComponent->SetupAttachment(mMeshGroupComponent);
  mFrontBarrelMeshComponent->SetupAttachment(mMeshGroupComponent);
  mHandGripMeshComponent->SetupAttachment(mMeshGroupComponent);
  mMainBodyMeshComponent->SetupAttachment(mMeshGroupComponent);
  mBackBodyMeshComponent->SetupAttachment(mMeshGroupComponent);
  mButtMeshComponent->SetupAttachment(mMeshGroupComponent);
  mStockMeshComponent->SetupAttachment(mMeshGroupComponent);
  mTriggerMeshComponent->SetupAttachment(mMeshGroupComponent);
  mUnderBarrelMeshComponent->SetupAttachment(mMeshGroupComponent);
  mUnderPartMeshComponent->SetupAttachment(mMeshGroupComponent);

  static ConstructorHelpers::FObjectFinder<UStaticMesh> scopeMesh(TEXT("StaticMesh'/Game/Torch/Weapons/Rifles/KT500/ScifiRiffleLowPoly_Rifle_Scope_LowPoly.ScifiRiffleLowPoly_Rifle_Scope_LowPoly'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> sideBodyMesh(TEXT("StaticMesh'/Game/Torch/Weapons/Rifles/KT500/ScifiRiffleLowPoly_Rifle_SideBody_LowPoly.ScifiRiffleLowPoly_Rifle_SideBody_LowPoly'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> clipMesh(TEXT("StaticMesh'/Game/Torch/Weapons/Rifles/KT500/ScifiRiffleLowPoly_Rifle_Clip_LowPoly.ScifiRiffleLowPoly_Rifle_Clip_LowPoly'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> frontBarrelMesh(TEXT("StaticMesh'/Game/Torch/Weapons/Rifles/KT500/ScifiRiffleLowPoly_Rifle_FrontBarrel_LowPoly.ScifiRiffleLowPoly_Rifle_FrontBarrel_LowPoly'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> handGripMesh(TEXT("StaticMesh'/Game/Torch/Weapons/Rifles/KT500/ScifiRiffleLowPoly_Rifle_HandGrip_LowPoly.ScifiRiffleLowPoly_Rifle_HandGrip_LowPoly'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> mainBodyMesh(TEXT("StaticMesh'/Game/Torch/Weapons/Rifles/KT500/ScifiRiffleLowPoly_Rifle_MainBody_LowPoly.ScifiRiffleLowPoly_Rifle_MainBody_LowPoly'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> backBodyMesh(TEXT("StaticMesh'/Game/Torch/Weapons/Rifles/KT500/ScifiRiffleLowPoly_Rifle_BackBody_LowPoly.ScifiRiffleLowPoly_Rifle_BackBody_LowPoly'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> buttMesh(TEXT("StaticMesh'/Game/Torch/Weapons/Rifles/KT500/ScifiRiffleLowPoly_Rifle_Butt_LowPoly.ScifiRiffleLowPoly_Rifle_Butt_LowPoly'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> stockMesh(TEXT("StaticMesh'/Game/Torch/Weapons/Rifles/KT500/ScifiRiffleLowPoly_Rifle_Stock_LowPoly.ScifiRiffleLowPoly_Rifle_Stock_LowPoly'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> triggerMesh(TEXT("StaticMesh'/Game/Torch/Weapons/Rifles/KT500/ScifiRiffleLowPoly_Rifle_Trigger_LowPoly.ScifiRiffleLowPoly_Rifle_Trigger_LowPoly'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> underBarrelMesh(TEXT("StaticMesh'/Game/Torch/Weapons/Rifles/KT500/ScifiRiffleLowPoly_Rifle_UnderBarrel_LowPoly.ScifiRiffleLowPoly_Rifle_UnderBarrel_LowPoly'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> underPartMesh(TEXT("StaticMesh'/Game/Torch/Weapons/Rifles/KT500/ScifiRiffleLowPoly_Rifle_UnderPart_LowPoly.ScifiRiffleLowPoly_Rifle_UnderPart_LowPoly'"));

  if (scopeMesh.Succeeded()) mScopeMeshComponent->SetStaticMesh(scopeMesh.Object);
  if (sideBodyMesh.Succeeded()) mSideBodyMeshComponent->SetStaticMesh(sideBodyMesh.Object);
  if (clipMesh.Succeeded()) mClipMeshComponent->SetStaticMesh(clipMesh.Object);
  if (frontBarrelMesh.Succeeded()) mFrontBarrelMeshComponent->SetStaticMesh(frontBarrelMesh.Object);
  if (handGripMesh.Succeeded()) mHandGripMeshComponent->SetStaticMesh(handGripMesh.Object);
  if (mainBodyMesh.Succeeded()) mMainBodyMeshComponent->SetStaticMesh(mainBodyMesh.Object);
  if (backBodyMesh.Succeeded()) mBackBodyMeshComponent->SetStaticMesh(backBodyMesh.Object);
  if (buttMesh.Succeeded()) mButtMeshComponent->SetStaticMesh(buttMesh.Object);
  if (stockMesh.Succeeded()) mStockMeshComponent->SetStaticMesh(stockMesh.Object);
  if (triggerMesh.Succeeded()) mTriggerMeshComponent->SetStaticMesh(triggerMesh.Object);
  if (underBarrelMesh.Succeeded()) mUnderBarrelMeshComponent->SetStaticMesh(underBarrelMesh.Object);
  if (underPartMesh.Succeeded()) mUnderPartMeshComponent->SetStaticMesh(underPartMesh.Object);

  // Setup materials
  static ConstructorHelpers::FObjectFinder<UMaterial> scopeMaterial(TEXT("Material'/Game/Torch/Weapons/Rifles/KT500/M_Rifle_Scope.M_Rifle_Scope'"));
  static ConstructorHelpers::FObjectFinder<UMaterial> sideBodyMaterial(TEXT("Material'/Game/Torch/Weapons/Rifles/KT500/M_Rifle_SideBody.M_Rifle_SideBody'"));
  static ConstructorHelpers::FObjectFinder<UMaterial> clipMaterial(TEXT("Material'/Game/Torch/Weapons/Rifles/KT500/M_Rifle_Clip.M_Rifle_Clip'"));
  static ConstructorHelpers::FObjectFinder<UMaterial> frontBarrelMaterial(TEXT("Material'/Game/Torch/Weapons/Rifles/KT500/M_Rifle_FrontBarrel.M_Rifle_FrontBarrel'"));
  static ConstructorHelpers::FObjectFinder<UMaterial> handGripMaterial(TEXT("Material'/Game/Torch/Weapons/Rifles/KT500/M_Rifle_HandGrip.M_Rifle_HandGrip'"));
  static ConstructorHelpers::FObjectFinder<UMaterial> mainBodyMaterial(TEXT("Material'/Game/Torch/Weapons/Rifles/KT500/M_Rifle_MainBody.M_Rifle_MainBody'"));
  static ConstructorHelpers::FObjectFinder<UMaterial> backBodyMaterial(TEXT("Material'/Game/Torch/Weapons/Rifles/KT500/M_Rifle_BackBody.M_Rifle_BackBody'"));
  static ConstructorHelpers::FObjectFinder<UMaterial> buttMaterial(TEXT("Material'/Game/Torch/Weapons/Rifles/KT500/M_Rifle_Butt.M_Rifle_Butt'"));
  static ConstructorHelpers::FObjectFinder<UMaterial> stockMaterial(TEXT("Material'/Game/Torch/Weapons/Rifles/KT500/M_Rifle_Stock.M_Rifle_Stock'"));
  static ConstructorHelpers::FObjectFinder<UMaterial> triggerMaterial(TEXT("Material'/Game/Torch/Weapons/Rifles/KT500/M_Rifle_Trigger.M_Rifle_Trigger'"));
  static ConstructorHelpers::FObjectFinder<UMaterial> underBarrelMaterial(TEXT("Material'/Game/Torch/Weapons/Rifles/KT500/M_Rifle_UnderBarrel.M_Rifle_UnderBarrel'"));
  static ConstructorHelpers::FObjectFinder<UMaterial> underPartMaterial(TEXT("Material'/Game/Torch/Weapons/Rifles/KT500/M_Rifle_UnderPart.M_Rifle_UnderPart'"));

  if (scopeMaterial.Succeeded()) mScopeMeshComponent->SetMaterial(0, scopeMaterial.Object);
  if (sideBodyMaterial.Succeeded()) mSideBodyMeshComponent->SetMaterial(0, sideBodyMaterial.Object);
  if (clipMaterial.Succeeded()) mClipMeshComponent->SetMaterial(0, clipMaterial.Object);
  if (frontBarrelMaterial.Succeeded()) mFrontBarrelMeshComponent->SetMaterial(0, frontBarrelMaterial.Object);
  if (handGripMaterial.Succeeded()) mHandGripMeshComponent->SetMaterial(0, handGripMaterial.Object);
  if (mainBodyMaterial.Succeeded()) mMainBodyMeshComponent->SetMaterial(0, mainBodyMaterial.Object);
  if (backBodyMaterial.Succeeded()) mBackBodyMeshComponent->SetMaterial(0, backBodyMaterial.Object);
  if (buttMaterial.Succeeded()) mButtMeshComponent->SetMaterial(0, buttMaterial.Object);
  if (stockMaterial.Succeeded()) mStockMeshComponent->SetMaterial(0, stockMaterial.Object);
  if (triggerMaterial.Succeeded()) mTriggerMeshComponent->SetMaterial(0, triggerMaterial.Object);
  if (underBarrelMaterial.Succeeded()) mUnderBarrelMeshComponent->SetMaterial(0, underBarrelMaterial.Object);
  if (underPartMaterial.Succeeded()) mUnderPartMeshComponent->SetMaterial(0, underPartMaterial.Object);

  // Set collisions
  mScopeMeshComponent->SetCollisionProfileName(TEXT("Custom"));
  mSideBodyMeshComponent->SetCollisionProfileName(TEXT("Custom"));
  mClipMeshComponent->SetCollisionProfileName(TEXT("Custom"));
  mFrontBarrelMeshComponent->SetCollisionProfileName(TEXT("Custom"));
  mHandGripMeshComponent->SetCollisionProfileName(TEXT("Custom"));
  mMainBodyMeshComponent->SetCollisionProfileName(TEXT("Custom"));
  mBackBodyMeshComponent->SetCollisionProfileName(TEXT("Custom"));
  mButtMeshComponent->SetCollisionProfileName(TEXT("Custom"));
  mStockMeshComponent->SetCollisionProfileName(TEXT("Custom"));
  mTriggerMeshComponent->SetCollisionProfileName(TEXT("Custom"));
  mUnderBarrelMeshComponent->SetCollisionProfileName(TEXT("Custom"));
  mUnderPartMeshComponent->SetCollisionProfileName(TEXT("Custom"));

  mScopeMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  mSideBodyMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  mClipMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  mFrontBarrelMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  mHandGripMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  mMainBodyMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  mBackBodyMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  mButtMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  mStockMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  mTriggerMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  mUnderBarrelMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  mUnderPartMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

  mScopeMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
  mSideBodyMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
  mClipMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
  mFrontBarrelMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
  mHandGripMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
  mMainBodyMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
  mBackBodyMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
  mButtMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
  mStockMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
  mTriggerMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
  mUnderBarrelMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
  mUnderPartMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void ATorchWeaponKT500::BeginPlay()
{
  SetupTimers(mFireRate);
}