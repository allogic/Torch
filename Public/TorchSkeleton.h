#pragma once

#include "CoreMinimal.h"

struct FTorchSkeleton
{
  static void GetCharacterBoneRotation(const FRotator& actorRotation, const FRotator& boneRotation, FVector& right, FVector& up, FVector& forward);
  static void GetInverseBonePitch(const FRotator& boneRotationSource, const FRotator& boneRotationTarget);
};