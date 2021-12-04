#pragma once

#include "CoreMinimal.h"

struct FTorchDebug
{
  static void DrawDebugAxisCross(UWorld* world, const FVector& location, const FVector& right, const FVector& up, const FVector& forward, float scale);
};