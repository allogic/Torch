#include "TorchWeaponUR86.h"

ATorchWeaponUR86::ATorchWeaponUR86()
{
  // Setup mesh group
  mMeshGroupComponent->SetRelativeRotation(FRotator{ 0.0f, 90.0f, -90.0f });
  mMeshGroupComponent->SetWorldScale3D(FVector{ 1.0f });

  // Setup static meshes
  static ConstructorHelpers::FObjectFinder<UStaticMesh> bodyMesh(TEXT("StaticMesh'/Game/Build/Weapons/Rifle/UR86/Meshes/SM_Body.SM_Body'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> scopeMesh(TEXT("StaticMesh'/Game/Build/Weapons/Rifle/UR86/Meshes/SM_Scope.SM_Scope'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> magMesh(TEXT("StaticMesh'/Game/Build/Weapons/Rifle/UR86/Meshes/SM_Mag.SM_Mag'"));

  AddStaticMeshToGroup("Body", bodyMesh.Object);
  AddStaticMeshToGroup("Scope", scopeMesh.Object);
  AddStaticMeshToGroup("Mag", magMesh.Object);
}