#pragma once

#include "CoreMinimal.h"

struct FPathSample
{
  float DistanceToTarget;
  TArray<FTransform> Path;

  inline bool operator < (const FPathSample& other) const { return DistanceToTarget < other.DistanceToTarget; }
};

struct FPathFinding
{
  static bool SamplePathAirborn(
    UWorld* world,
    FVector origin,
    FVector target,
    FQuat rotation,
    int32 numSegments,
    float sphereRadius,
    FPathSample& sample,
    bool debug);
  static bool SamplePathSurface(
    UWorld* world,
    FVector origin,
    FVector target,
    FQuat rotation,
    int32 numSegments,
    float sphereRadius,
    float randomSteeringAngle,
    float randomRotationItensity,
    float targetRotationItensity,
    FPathSample& sample,
    bool debug);
};