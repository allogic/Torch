#include "TorchBehaviorTasks.h"
#include "NavigationData.h"
#include "NavigationSystem.h"
#include "TorchAIController.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"

bool FTorchBehaviorTask::MoveToRandomLocationInsideSphere(ATorchAIController* AIController, float randomRadius, float acceptanceRadius)
{
  if (AIController)
  {
    UWorld* world = AIController->GetWorld();
    ACharacter* AIcharacter = AIController->GetCharacter();
    if (world && AIcharacter)
    {
      FNavLocation navLocation;
      UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(world);
      if (navSystem)
      {
        if (navSystem->GetRandomPointInNavigableRadius(AIcharacter->GetActorLocation(), randomRadius, navLocation, nullptr))
        {
          EPathFollowingRequestResult::Type requestResult = AIController->MoveToLocation(navLocation.Location, acceptanceRadius);
          if (requestResult == EPathFollowingRequestResult::RequestSuccessful)
          {
            return true;
          }
        }
      }
    }
  }
  return false;
}

bool FTorchBehaviorTask::MoveToPlayerLocation(ATorchAIController* AIController, float acceptanceRadius)
{
  if (AIController)
  {
    UWorld* world = AIController->GetWorld();
    ACharacter* character = UGameplayStatics::GetPlayerCharacter(world, 0);
    if (world && character)
    {
      EPathFollowingRequestResult::Type requestResult = AIController->MoveToLocation(character->GetActorLocation(), acceptanceRadius);
      if (requestResult == EPathFollowingRequestResult::RequestSuccessful)
      {
        return true;
      }
    }
  }
  return false;
}