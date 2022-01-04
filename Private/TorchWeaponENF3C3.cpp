#include "TorchWeaponENF3C3.h"
#include "Components/StaticMeshComponent.h"

ATorchWeaponENF3C3::ATorchWeaponENF3C3()
{
  // Setup mesh group
  mMeshGroupComponent->SetRelativeRotation(FRotator{ 0.0f, 90.0f, -90.0f });
  mMeshGroupComponent->SetWorldScale3D(FVector{ 1.0f });

  // Setup static meshes
  static ConstructorHelpers::FObjectFinder<UStaticMesh> bodyMesh(TEXT("StaticMesh'/Game/Build/Weapons/Pistol/ENF3C3/Meshes/SM_Body.SM_Body'"));

  AddStaticMeshToGroup("Body", bodyMesh.Object);
}