#include "TorchAICharacter.h"
#include "DrawDebugHelpers.h"
#include "TorchAIController.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ATorchAICharacter::ATorchAICharacter()
{
  // Setup sight system
  mHeadLocation = CreateDefaultSubobject<USceneComponent>(TEXT("HeadLocation"));
  mHeadLocation->SetupAttachment(RootComponent);

  // Setup perception system
  mPerceptionSphere = CreateDefaultSubobject<USphereComponent>("PerceptionSphere");
  mPerceptionSphere->SetupAttachment(RootComponent);
  mPerceptionSphere->OnComponentBeginOverlap.AddDynamic(this, &ATorchAICharacter::OnBeginOverlap);
  mPerceptionSphere->OnComponentEndOverlap.AddDynamic(this, &ATorchAICharacter::OnEndOverlap);

  // Setup AI chararcter
  PrimaryActorTick.bCanEverTick = true;
}

void ATorchAICharacter::BeginPlay()
{
  Super::BeginPlay();

  // Set collision channels
  GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
  mPerceptionSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
  GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);

  // Setup animation instance
  mAnimationInstance = GetMesh()->GetAnimInstance();
}
void ATorchAICharacter::Tick(float deltaTime)
{
  Super::Tick(deltaTime);

  // Sync animations to physics
  SyncAnimation();

  // Draw some gizmos
  if (mDrawDebugGizmo)
  {
    DrawDebugGizmo();
  }
}

void ATorchAICharacter::OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
  GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, TEXT("BeginOverlap"));
  //if (otherActor != this)
  //{
  //  mPerceivingActors.Emplace(otherActor);
  //}
  GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::FromInt(mPerceivingActors.Num()));
}
void ATorchAICharacter::OnEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
  GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, TEXT("EndOverlap"));
  //if (otherActor != this)
  //{
  //  mPerceivingActors.Remove(otherActor);
  //}
  GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::FromInt(mPerceivingActors.Num()));
}

void ATorchAICharacter::SyncAnimation()
{
  // Update animation blueprint variables
  if (mAnimationInstance)
  {
    FFloatProperty* direction = FindFProperty<FFloatProperty>(mAnimationInstance->GetClass(), TEXT("Direction"));
    FFloatProperty* speed = FindFProperty<FFloatProperty>(mAnimationInstance->GetClass(), TEXT("Speed"));

    if (direction)
    {
      direction->SetPropertyValue_InContainer(mAnimationInstance, GetCharacterMovement()->Velocity.X);
    }
    if (speed)
    {
      speed->SetPropertyValue_InContainer(mAnimationInstance, GetCharacterMovement()->Velocity.Y);
    }
  }
}
void ATorchAICharacter::DrawDebugGizmo()
{
  AController* controller = GetController();
  if (controller)
  {
    // Draw local forward location
    //DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetActorForwardVector(), FColor::Blue, false);

    // Draw movement velocity
    //DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + GetMovementComponent()->Velocity, FColor::Green, false);
  }
}