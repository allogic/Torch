#pragma once

#include "CoreMinimal.h"

struct FTorchPathFinding
{
  static bool RequestPathInsideSphere(UWorld* world, ACharacter* character, float radius, FNavPathSharedPtr& path, const FVector& relativeLocation = FVector::ZeroVector);
  static bool RequestPathToLocation(UWorld* world, ACharacter* character, const FVector& target, FNavPathSharedPtr& path);
};