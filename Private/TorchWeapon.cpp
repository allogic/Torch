#include "TorchWeapon.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "TorchBlueprintUtils.h"
#include "TorchTPSCharacter.h"
#include "TorchCore.h"

ATorchWeapon::ATorchWeapon()
{
  // Setup root
  PrimaryActorTick.bCanEverTick = true;
  RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

  // Setup equipment sphere
  mEquipmentSphereComponent = CreateDefaultSubobject<USphereComponent>("Collider");
  mEquipmentSphereComponent->SetupAttachment(RootComponent);
  mEquipmentSphereComponent->SetSphereRadius(mEquipmentRadius);
  mEquipmentSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATorchWeapon::OnBeginOverlap);

  // Setup mesh group
  mMeshGroupComponent = CreateDefaultSubobject<USceneComponent>("MeshGroup");
  mMeshGroupComponent->SetupAttachment(RootComponent);

  // Setup timer delegates
  mTimerDelegateFireRateDecay.BindUFunction(this, TEXT("UpdateFireRateDecay"));
}

void ATorchWeapon::Tick(float deltaTime)
{
  Super::Tick(deltaTime);

  // Rotate weapon if unequipped
  if (!mTPSCharacter)
  {
    RootComponent->AddRelativeRotation(FQuat{ FVector::UpVector, deltaTime });
  }

  // Update shooting
  if (mIsFiring && mCanFire)
  {
    TORCH_LOG("Fire");
    mCanFire = false;
  }
}

void ATorchWeapon::SetupTimers(float fireRate)
{
  GetWorldTimerManager().SetTimer(mTimerHandleFireRateDecay, mTimerDelegateFireRateDecay, fireRate, true);
}

void ATorchWeapon::UpdateFireRateDecay()
{
  mCanFire = true;
}

void ATorchWeapon::OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
  if (otherActor != this)
  {
    mTPSCharacter = Cast<ATorchTPSCharacter>(otherActor);
    if (mTPSCharacter)
    {
      mEquipmentSphereComponent->OnComponentBeginOverlap.RemoveDynamic(this, &ATorchWeapon::OnBeginOverlap);
      AttachToComponent(mTPSCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("root"));
      mTPSCharacter->SetCurrentWeaponActor(this);
    }
  }
}