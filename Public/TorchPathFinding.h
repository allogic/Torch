#pragma once

#include "CoreMinimal.h"

struct FPathSample
{
  float DistanceToTarget;
  TArray<FTransform> Path;

  inline bool operator < (const FPathSample& other) const { return DistanceToTarget < other.DistanceToTarget; }
};

struct FFlowMapNode
{
  FVector Origin;
  FVector Direction;
  float Force;
};

struct FPathFinding
{
  static void SampleOptimalPathAlongSurfaces(
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
    FVector& normal,
    bool enableDebug,
    bool debugPersistence);

  static void SampleFlowMap(
    UWorld* world,
    FVector origin,
    int32 gridSize,
    float cellSize,
    const TArray<FTransform>& currentPath,
    TArray<FFlowMapNode>& flowMapNodes,
    bool enableDebug,
    bool debugPersistence);
};