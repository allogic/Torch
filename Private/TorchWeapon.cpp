#include "TorchWeapon.h"
#include "TorchBlueprintUtils.h"
#include "TorchFPSCharacter.h"
#include "TorchCore.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"

ATorchWeapon::ATorchWeapon()
{
  // Setup root
  PrimaryActorTick.bCanEverTick = true;

  // Setup collider
  RootComponent = CreateDefaultSubobject<UBoxComponent>("Collider");
  ((UBoxComponent*)RootComponent)->SetCollisionProfileName(TEXT("Custom"));
  ((UBoxComponent*)RootComponent)->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  ((UBoxComponent*)RootComponent)->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
  ((UBoxComponent*)RootComponent)->SetSimulatePhysics(true);

  // Setup equipment sphere
  mPerceptionSphereComponent = CreateDefaultSubobject<USphereComponent>("Perception");
  mPerceptionSphereComponent->SetupAttachment(RootComponent);
  mPerceptionSphereComponent->SetSphereRadius(mEquipmentRadius);
  mPerceptionSphereComponent->SetCollisionProfileName(TEXT("Custom"));
  mPerceptionSphereComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
  mPerceptionSphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  mPerceptionSphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Overlap);

  // Setup audio
  static ConstructorHelpers::FObjectFinder<USoundWave> soundWave(TEXT("SoundWave'/Game/Build/Audio/Guns/gunshot.gunshot'"));
  if (soundWave.Succeeded()) mSound = soundWave.Object;
  mAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
  mAudioComponent->SetupAttachment(RootComponent);
  mAudioComponent->bAutoActivate = false;
  mAudioComponent->SetSound(mSound);

  // Setup mesh group
  mMeshGroupComponent = CreateDefaultSubobject<USceneComponent>("MeshGroup");
  mMeshGroupComponent->SetupAttachment(RootComponent);
}

void ATorchWeapon::BeginPlay()
{
  Super::BeginPlay();

  // Setup audio
  mAudioComponent->SetSound(mSound);
}
void ATorchWeapon::Tick(float deltaTime)
{
  Super::Tick(deltaTime);
}

void ATorchWeapon::OnAttach()
{
  ((UBoxComponent*)RootComponent)->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  ((UBoxComponent*)RootComponent)->SetSimulatePhysics(false);
  mPerceptionSphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Ignore);
}
void ATorchWeapon::OnDetach()
{
  ((UBoxComponent*)RootComponent)->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  ((UBoxComponent*)RootComponent)->SetSimulatePhysics(true);
  mPerceptionSphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Overlap);
}

void ATorchWeapon::AddStaticMeshToGroup(const FString& meshName, UStaticMesh* staticMesh)
{
  if (staticMesh)
  {
    int32 index = mStaticMeshes.Emplace(CreateDefaultSubobject<UStaticMeshComponent>(&meshName[0]));
    mStaticMeshes[index]->SetupAttachment(mMeshGroupComponent);
    mStaticMeshes[index]->SetStaticMesh(staticMesh);
    mStaticMeshes[index]->SetCollisionProfileName(TEXT("Custom"));
    mStaticMeshes[index]->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  }
}