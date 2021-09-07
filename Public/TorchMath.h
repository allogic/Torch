#pragma once

#include "CoreMinimal.h"

struct FMafh
{
  static FVector ComputeOptimalSurfaceDirection(FVector target, FVector normal, FVector up);

  static void EvaluateCatRom(const TArray<FVector>& points, uint32 numSamples, TArray<FVector>& samplePoints);
};