#include "TorchPathSharing.h"
#include "TorchPathFinding.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"

static TMap<int32, FNavPathSharedPtr> mSharedPaths = {};

bool FTorchSharedPath::GetOrCreate(UWorld* world, ACharacter* character, int32 key, FNavPathSharedPtr& path)
{
  if (mSharedPaths.Contains(key))
  {
    // Create copies for shared access
    path = FNavPathSharedPtr{ mSharedPaths[key] };
    return true;
  }
  else
  {
    if (FTorchPathFinding::RequestPathInsideSphere(world, character, 10000.0f, path))
    {
      mSharedPaths.Emplace(key, path);
      return true;
    }
  }
  return false;
}