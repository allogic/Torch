#include "TorchWeaponDSU.h"

ATorchWeaponDSU::ATorchWeaponDSU()
{
  // Setup mesh group
  mMeshGroupComponent->SetRelativeRotation(FRotator{ 0.0f, 0.0f, -90.0f });
  mMeshGroupComponent->SetWorldScale3D(FVector{ 0.7f });

  // Setup static meshes
  static ConstructorHelpers::FObjectFinder<UStaticMesh> bodyMesh(TEXT("StaticMesh'/Game/Build/Weapons/Sniper/DSU/Meshes/SM_Body.SM_Body'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> bulletEjectionMesh(TEXT("StaticMesh'/Game/Build/Weapons/Sniper/DSU/Meshes/SM_Bullet_Ejection.SM_Bullet_Ejection'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> magMesh(TEXT("StaticMesh'/Game/Build/Weapons/Sniper/DSU/Meshes/SM_Mag.SM_Mag'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> reloadHandleMesh(TEXT("StaticMesh'/Game/Build/Weapons/Sniper/DSU/Meshes/SM_Reload_Handle.SM_Reload_Handle'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> scopeMesh(TEXT("StaticMesh'/Game/Build/Weapons/Sniper/DSU/Meshes/SM_Scope.SM_Scope'"));

  AddStaticMeshToGroup("Body", bodyMesh.Object);
  AddStaticMeshToGroup("BulletEjection", bulletEjectionMesh.Object);
  AddStaticMeshToGroup("Mag", magMesh.Object);
  AddStaticMeshToGroup("ReloadHandle", reloadHandleMesh.Object);
  AddStaticMeshToGroup("Scope", scopeMesh.Object);
}