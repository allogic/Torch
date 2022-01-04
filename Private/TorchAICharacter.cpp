#include "TorchAICharacter.h"
#include "TorchBlueprintUtils.h"
#include "TorchMath.h"
#include "TorchCore.h"
#include "TorchFPSCharacter.h"
#include "TorchAnimationInstance.h"
#include "TorchAIController.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"

void UTorchAICharacterNotifyCombat::NotifyBegin(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation, float totalDuration)
{
  ATorchAICharacter* character = meshComp->GetOwner<ATorchAICharacter>();
  if (character)
  {
    character->IsAttacking(true);
  }
}
void UTorchAICharacterNotifyCombat::NotifyTick(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation, float frameDeltaTime)
{

}
void UTorchAICharacterNotifyCombat::NotifyEnd(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation)
{
  ATorchAICharacter* character = meshComp->GetOwner<ATorchAICharacter>();
  if (character)
  {
    character->IsAttacking(false);
  }
}

ATorchFPSCharacter* ATorchAICharacter::GetPlayerCharacter()
{
  return mPlayer;
}
void ATorchAICharacter::EnableControllerRotation()
{
  GetCharacterMovement()->bUseControllerDesiredRotation = true;
}
void ATorchAICharacter::DisableControllerRotation()
{
  GetCharacterMovement()->bUseControllerDesiredRotation = false;
}

ATorchAICharacter::ATorchAICharacter()
{
  // Setup actor
  PrimaryActorTick.bCanEverTick = true;
  AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

  // Setup movement
  GetCharacterMovement()->RotationRate.Yaw = 200.0f;
  GetCharacterMovement()->bUseControllerDesiredRotation = true;
  bUseControllerRotationPitch = false;
  bUseControllerRotationYaw = false;
  bUseControllerRotationRoll = false;

  // Setup physical animtion
  mPhysicalAnimationComponent = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("PhysicalAnimation"));
  mPhysicalAnimationComponent->SetStrengthMultiplyer(1.0f);

  // Setup perception
  mPerceptionSphereComponent = CreateDefaultSubobject<USphereComponent>("PerceptionSphere");
  mPerceptionSphereComponent->SetupAttachment(RootComponent);
  mPerceptionSphereComponent->SetSphereRadius(mPerceptionSphereRadius);
  mPerceptionSphereComponent->SetCollisionProfileName(TEXT("Custom"));
  mPerceptionSphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  mPerceptionSphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
  mPerceptionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ATorchAICharacter::OnBeginOverlap);
  mPerceptionSphereComponent->OnComponentEndOverlap.AddDynamic(this, &ATorchAICharacter::OnEndOverlap);

  // Setup capsule
  GetCapsuleComponent()->SetCollisionProfileName(TEXT("Custom"));
  GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);

  // Setup skeletal mesh
  GetMesh()->SetCollisionProfileName(TEXT("Custom"));
  GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
  GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
}

void ATorchAICharacter::BeginPlay()
{
  Super::BeginPlay();

  // Configure custom animation instance
  mAnimationInstance = Cast<UTorchAnimationInstance>(GetMesh()->GetAnimInstance());
}
void ATorchAICharacter::Tick(float deltaTime)
{
  Super::Tick(deltaTime);

  UpdateAnimationBlueprint();

  // Draw some debugs
  Debug();
}

void ATorchAICharacter::OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
  if (otherActor != nullptr && otherActor != this)
  {
    // Found enemy
    if (ATorchAICharacter* enemy = Cast<ATorchAICharacter>(otherActor))
    {
      mEnemies.Add(enemy);
    }
    // Found player
    if (!mPlayer)
    {
      if (ATorchFPSCharacter* player = Cast<ATorchFPSCharacter>(otherActor))
      {
        mPlayer = player;
      }
    }
  }
}
void ATorchAICharacter::OnEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
  if (otherActor != nullptr && otherActor != this)
  {
    // Lost enemy
    if (ATorchAICharacter* enemy = Cast<ATorchAICharacter>(otherActor))
    {
      mEnemies.Remove(enemy);
    }
    // Lost player
    if (mPlayer)
    {
      if (ATorchFPSCharacter* player = Cast<ATorchFPSCharacter>(otherActor))
      {
        mPlayer = nullptr;
      }
    }
  }
}

void ATorchAICharacter::Debug()
{
  if (mEnableDebug)
  {
    
  }
}