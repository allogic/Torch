#include "TorchWeaponKT500.h"

ATorchWeaponKT500::ATorchWeaponKT500()
{
  // Setup mesh group
  mMeshGroupComponent->SetRelativeRotation(FRotator{ 0.0f, 180.0f, -90.0f });

  // Setup static meshes
  static ConstructorHelpers::FObjectFinder<UStaticMesh> bodyMesh(TEXT("StaticMesh'/Game/Build/Weapons/Rifle/KT500/Meshes/SM_Body.SM_Body'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> scopeMesh(TEXT("StaticMesh'/Game/Build/Weapons/Rifle/KT500/Meshes/SM_Scope.SM_Scope'"));

  AddStaticMeshToGroup("Body", bodyMesh.Object);
  AddStaticMeshToGroup("Scope", scopeMesh.Object);
}