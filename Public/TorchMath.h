#pragma once

#include "CoreMinimal.h"

struct FTorchMath
{
  static FVector RandomInsideUnitCircle();
  static FVector RandomInsideUnitSphere();

  static FVector RandomInsideRect(const FVector& location, const FVector& size);

  static FVector CatmullRom(const FVector& p0, const FVector& p1, const FVector& p2, const FVector& p3, float t);
  static FVector CubicBezier(const FVector& p0, const FVector& c0, const FVector& c1, const FVector& p1, float t);

  static float RemapValue(float value, float low0, float high0, float low1, float high1);
};