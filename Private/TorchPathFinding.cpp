#include "TorchPathFinding.h"
#include "TorchCore.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"

bool FTorchPathFinding::RequestPathInsideSphere(UWorld* world, ACharacter* character, float radius, FNavPathSharedPtr& path, const FVector& relativeLocation)
{
  if (world && character)
  {
    UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(world);
    if (navSys)
    {
      ANavigationData* navData = navSys->GetNavDataForProps(character->GetNavAgentPropertiesRef());
      if (navData)
      {
        FNavLocation navLoc;
        if (navSys->GetRandomPointInNavigableRadius(character->GetActorLocation() + relativeLocation, radius, navLoc, nullptr))
        {
          FPathFindingQuery pathQuery{ character, *navData, character->GetNavAgentLocation(), navLoc.Location };
          FPathFindingResult pathResult = navSys->FindPathSync(pathQuery);
          if (pathResult.Result != ENavigationQueryResult::Error)
          {
            if (pathResult.Path.IsValid())
            {
              if (pathResult.Path->GetPathPoints().Num() > 1)
              {
                path = pathResult.Path;
                return true;
              }
            }
          }
        }
      }
    }
  }
  return false;
}
bool FTorchPathFinding::RequestPathToLocation(UWorld* world, ACharacter* character, const FVector& target, FNavPathSharedPtr& path)
{
  if (world && character)
  {
    UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(world);
    if (navSys)
    {
      ANavigationData* navData = navSys->GetNavDataForProps(character->GetNavAgentPropertiesRef());
      if (navData)
      {
        FPathFindingQuery pathQuery{ character, *navData, character->GetNavAgentLocation(), target };
        FPathFindingResult pathResult = navSys->FindPathSync(pathQuery);
        if (pathResult.Result != ENavigationQueryResult::Error)
        {
          if (pathResult.Path.IsValid())
          {
            if (pathResult.Path->GetPathPoints().Num() > 1)
            {
              path = pathResult.Path;
              return true;
            }
          }
        }
      }
    }
  }
  return false;
}