#pragma once

#include "CoreMinimal.h"

struct FTorchMath
{
  static FVector RandomInsideUnitCircle();
  static FVector RandomInsideUnitSphere();

  static FTransform LookAt(const FVector& source, const FVector& target, const FVector& up = FVector::UpVector);
  static FQuat RelativeRotation(const FQuat& source, const FQuat& target);

  static FVector ComputeOptimalSurfaceDirection(const FVector& target, const FVector& normal, const FVector& up);
  static FVector EvaluateCatRom(const FVector& p0, const FVector& p1, const FVector& p2, const FVector& p3, float time);

  static float RemapValue(float value, float low0, float high0, float low1, float high1);
};