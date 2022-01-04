#include "TorchWeaponMK7.h"

ATorchWeaponMK7::ATorchWeaponMK7()
{
  // Setup mesh group
  mMeshGroupComponent->SetRelativeRotation(FRotator{ -180.0f, -90.0f, 90.0f });
  mMeshGroupComponent->SetWorldScale3D(FVector{ 0.11f });

  // Setup static meshes
  static ConstructorHelpers::FObjectFinder<UStaticMesh> bodyMesh(TEXT("StaticMesh'/Game/Build/Weapons/Special/MK7/Meshes/SM_Body.SM_Body'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> barrelMesh(TEXT("StaticMesh'/Game/Build/Weapons/Special/MK7/Meshes/SM_Barrel.SM_Barrel'"));

  AddStaticMeshToGroup("Body", bodyMesh.Object);
  AddStaticMeshToGroup("Barrel", barrelMesh.Object);
}