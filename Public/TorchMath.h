#pragma once

#include "CoreMinimal.h"

struct FMafh
{
  static FVector ComputeOptimalSurfaceDirection(FVector target, FVector normal, FVector up);
  static FVector EvaluateCatRom(FVector p0, FVector p1, FVector p2, FVector p3, float time);
};