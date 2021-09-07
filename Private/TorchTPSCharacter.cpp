#include "TorchTPSCharacter.h"
#include "Kismet/GameplayStatics.h"

ATorchTPSCharacter::ATorchTPSCharacter()
{
  PrimaryActorTick.bCanEverTick = true;
}

void ATorchTPSCharacter::BeginPlay()
{
  Super::BeginPlay();  
}
void ATorchTPSCharacter::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}
void ATorchTPSCharacter::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
  Super::SetupPlayerInputComponent(playerInputComponent);

  playerInputComponent->BindAction(TEXT("AlignRotation"), EInputEvent::IE_Pressed, this, &ATorchTPSCharacter::AlignActorRotation);
}

void ATorchTPSCharacter::AlignActorRotation()
{
  GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, TEXT("Align"));
}