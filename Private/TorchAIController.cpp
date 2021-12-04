#include "TorchAIController.h"
#include "TorchCore.h"
#include "TorchMath.h"
#include "TorchPathFinding.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"
#include "TorchCharacter.h"
#include "TorchBehaviorTasks.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

ATorchAIController::ATorchAIController()
{

}

void ATorchAIController::BeginPlay()
{
  Super::BeginPlay();
}
void ATorchAIController::Tick(float deltaTime)
{
  Super::Tick(deltaTime);
}
void ATorchAIController::OnPossess(APawn* pawn)
{
  mCharacter = Cast<ATorchCharacter>(pawn);

  if (mCharacter)
  {
    //FTorchPathFinding::RequestNewPath(GetWorld(), mCharacter, mCharacter->GetActorLocation(), FVector{}, mPath);

    WalkForward();
  }
}
void ATorchAIController::OnUnPossess()
{
  mCharacter = nullptr;
}

void ATorchAIController::Stop()
{
  if (mCharacter)
  {
    mCharacter->GetMesh()->GetAnimInstance()->StopAllMontages(0.25f);
  }
}
void ATorchAIController::WalkForward()
{
  if (mCharacter)
  {
    //mCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection(TEXT("Walk"), mCharacter->mMontageWalkForward);
    //mCharacter->GetMesh()->GetAnimInstance()->Montage_Play(mCharacter->mMontageWalkForward, 1.0f);
  }
}