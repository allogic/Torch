#pragma once

#include "CoreMinimal.h"
#include "AI/Navigation/NavigationTypes.h"

class ACharacter;

struct FTorchSharedPath
{
  static bool GetOrCreate(UWorld* world, ACharacter* character, int32 key, FNavPathSharedPtr& path);
};