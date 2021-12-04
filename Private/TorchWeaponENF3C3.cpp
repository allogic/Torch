#include "TorchWeaponENF3C3.h"
#include "Components/StaticMeshComponent.h"

ATorchWeaponENF3C3::ATorchWeaponENF3C3()
{
  // Setup mesh group
  mMeshGroupComponent->SetRelativeRotation(FRotator{ 180.0f, 0.0f, 180.0f });
  mMeshGroupComponent->SetRelativeScale3D(FVector{ 0.05f, 0.05f, 0.05f });

  // Setup static meshes
  mGlassMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Glass");
  mHandGripMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("HandGrip");
  mSidesMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Sides");
  mFrontMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Front");
  mDetailMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Detail");
  mBodyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Body");
  mBarrelMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Barrel");

  mGlassMeshComponent->SetupAttachment(mMeshGroupComponent);
  mHandGripMeshComponent->SetupAttachment(mMeshGroupComponent);
  mSidesMeshComponent->SetupAttachment(mMeshGroupComponent);
  mFrontMeshComponent->SetupAttachment(mMeshGroupComponent);
  mDetailMeshComponent->SetupAttachment(mMeshGroupComponent);
  mBodyMeshComponent->SetupAttachment(mMeshGroupComponent);
  mBarrelMeshComponent->SetupAttachment(mMeshGroupComponent);

  static ConstructorHelpers::FObjectFinder<UStaticMesh> glassMesh(TEXT("StaticMesh'/Game/Torch/Weapons/Pistols/ENF3C3/Enf2_Glass.Enf2_Glass'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> handGripMesh(TEXT("StaticMesh'/Game/Torch/Weapons/Pistols/ENF3C3/Enf2_Low_10.Enf2_Low_10'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> sidesMesh(TEXT("StaticMesh'/Game/Torch/Weapons/Pistols/ENF3C3/Enf2_Low_11.Enf2_Low_11'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> frontMesh(TEXT("StaticMesh'/Game/Torch/Weapons/Pistols/ENF3C3/Enf2_Low_01.Enf2_Low_01'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> detailMesh(TEXT("StaticMesh'/Game/Torch/Weapons/Pistols/ENF3C3/Enf2_Low_04.Enf2_Low_04'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> bodyMesh(TEXT("StaticMesh'/Game/Torch/Weapons/Pistols/ENF3C3/Enf2_Low_05.Enf2_Low_05'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> barrelMesh(TEXT("StaticMesh'/Game/Torch/Weapons/Pistols/ENF3C3/Enf2_Low_09.Enf2_Low_09'"));

  if (glassMesh.Succeeded()) mGlassMeshComponent->SetStaticMesh(glassMesh.Object);
  if (handGripMesh.Succeeded()) mHandGripMeshComponent->SetStaticMesh(handGripMesh.Object);
  if (sidesMesh.Succeeded()) mSidesMeshComponent->SetStaticMesh(sidesMesh.Object);
  if (frontMesh.Succeeded()) mFrontMeshComponent->SetStaticMesh(frontMesh.Object);
  if (detailMesh.Succeeded()) mDetailMeshComponent->SetStaticMesh(detailMesh.Object);
  if (bodyMesh.Succeeded()) mBodyMeshComponent->SetStaticMesh(bodyMesh.Object);
  if (barrelMesh.Succeeded()) mBarrelMeshComponent->SetStaticMesh(barrelMesh.Object);

  // Setup materials
  static ConstructorHelpers::FObjectFinder<UMaterial> meshMaterial(TEXT("Material'/Game/Torch/Weapons/Pistols/ENF3C3/Enforcer.Enforcer'"));

  if (meshMaterial.Succeeded())
  {
    mGlassMeshComponent->SetMaterial(0, meshMaterial.Object);
    mHandGripMeshComponent->SetMaterial(0, meshMaterial.Object);
    mSidesMeshComponent->SetMaterial(0, meshMaterial.Object);
    mFrontMeshComponent->SetMaterial(0, meshMaterial.Object);
    mDetailMeshComponent->SetMaterial(0, meshMaterial.Object);
    mBodyMeshComponent->SetMaterial(0, meshMaterial.Object);
    mBarrelMeshComponent->SetMaterial(0, meshMaterial.Object);
  }

  // Set collisions
  mGlassMeshComponent->SetCollisionProfileName(TEXT("Custom"));
  mHandGripMeshComponent->SetCollisionProfileName(TEXT("Custom"));
  mSidesMeshComponent->SetCollisionProfileName(TEXT("Custom"));
  mFrontMeshComponent->SetCollisionProfileName(TEXT("Custom"));
  mDetailMeshComponent->SetCollisionProfileName(TEXT("Custom"));
  mBodyMeshComponent->SetCollisionProfileName(TEXT("Custom"));
  mBarrelMeshComponent->SetCollisionProfileName(TEXT("Custom"));

  mGlassMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  mHandGripMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  mSidesMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  mFrontMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  mDetailMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  mBodyMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  mBarrelMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

  mGlassMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
  mHandGripMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
  mSidesMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
  mFrontMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
  mDetailMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
  mBodyMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
  mBarrelMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void ATorchWeaponENF3C3::BeginPlay()
{
  SetupTimers(mFireRate);
}