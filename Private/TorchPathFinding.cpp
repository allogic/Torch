#include "TorchPathFinding.h"
#include "DrawDebugHelpers.h"

void FPathFinding::SampleOptimalPathAlongSurfaces(
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
  bool debugPersistence)
{
  // Setup trace params
  FHitResult hitResult;
  ECollisionChannel traceChannel = ECollisionChannel::ECC_GameTraceChannel1;
  FCollisionShape collShape;
  collShape.ShapeType = ECollisionShape::Sphere;
  collShape.SetSphere(sphereRadius);

  // Begin sampling
  for (int32 segment = 0; segment < numSegments; ++segment)
  {
    // Get target direction
    FVector targetDirection = target - origin;
    targetDirection.Normalize();

    // Sweep sphere along direction
    bool hit = world->SweepSingleByChannel(hitResult, origin, origin + rotation.GetForwardVector() * sphereRadius, rotation, traceChannel, collShape);
    if (hit)
    {
      // Set hit normal
      normal = hitResult.Normal;
      normal.Normalize();

      // Compute orthogonal directions from normal & current forward
      FVector right = FVector::CrossProduct(rotation.GetForwardVector(), normal);
      FVector forward = FVector::CrossProduct(right, normal);

      // Apply escape rotation
      FRotator escapeRotation = rotation.Rotator();
      escapeRotation.Pitch += FMath::RadiansToDegrees(-FVector::DotProduct(rotation.GetUpVector(), forward));
      escapeRotation.Yaw += 0.0f;
      rotation = escapeRotation.Quaternion();
      rotation.Normalize();

      // Adjust distance to surface
      origin += rotation.GetUpVector() * sphereRadius + 22.0f;

      DrawDebugLine(world, origin, origin + right * 300.0f, FColor::Red, debugPersistence, -1.0f, 0, 2);
      DrawDebugLine(world, origin, origin + normal * 300.0f, FColor::Green, debugPersistence, -1.0f, 0, 2);
      DrawDebugLine(world, origin, origin + forward * 300.0f, FColor::Blue, debugPersistence, -1.0f, 0, 2);

      DrawDebugLine(world, origin, origin + (-forward) * 300.0f, FColor::Yellow, debugPersistence, -1.0f, 0, 3);
    }
    else
    {
      // Apply random rotation
      FRotator randomRotation = rotation.Rotator();
      randomRotation.Yaw += randomSteeringAngle * randomRotationItensity;
      rotation = randomRotation.Quaternion();
      rotation.Normalize();

      // Apply target rotation
      float targetRotationDistanceIntensity = (1.0f / numSegments) * segment;
      FRotator targetRotation = rotation.Rotator();
      targetRotation.Yaw += FMath::RadiansToDegrees(FVector::DotProduct(rotation.GetRightVector(), targetDirection)) * targetRotationItensity * targetRotationDistanceIntensity;
      rotation = targetRotation.Quaternion();
      rotation.Normalize();

      // March along forward
      origin += rotation.GetForwardVector() * sphereRadius;

      // Priority rules
      //sample.Length += FVector::Distance(origin, origin + rotation.GetForwardVector() * sphereRadius);
      sample.DistanceToTarget = FVector::Distance(origin, target);

      // Add transform to path
      sample.Path.Emplace(FTransform{ rotation, origin });
    }

    // Draw some debugs
    if (enableDebug)
    {
      if (segment % 2 == 0)
      {
        DrawDebugBox(world, origin, FVector{ 10.0f, sphereRadius, sphereRadius }, rotation, FColor::White, debugPersistence);
      }
      else
      {
        DrawDebugLine(world, origin, origin + rotation.GetRightVector() * 200.0f, FColor::Red, debugPersistence);
        DrawDebugLine(world, origin, origin + rotation.GetUpVector() * 200.0f, FColor::Green, debugPersistence);
        DrawDebugLine(world, origin, origin + rotation.GetForwardVector() * 200.0f, FColor::Blue, debugPersistence);
      }
    }
  }
}

void FPathFinding::SampleFlowMap(
  UWorld* world,
  FVector origin,
  int32 gridSize,
  float cellSize,
  const TArray<FTransform>& currentPath,
  TArray<FFlowMapNode>& flowMapNodes,
  bool enableDebug,
  bool debugPersistence)
{
  // Clear old flow map
  flowMapNodes.Empty();

  // Begin sampling new flow map
  for (int32 i = 0; i < gridSize; ++i)
  {
    for (int32 j = 0; j < gridSize; ++j)
    {
      // Get integer grid vectors
      FVector gridHalf = FVector{ (float)gridSize / 2, (float)gridSize / 2, 0.0f };
      FVector cellHalf = FVector{ (float)cellSize / 2, (float)cellSize / 2, 0.0f };
      FVector gridOffset = FVector{ (float)i, (float)j, 0.0f } - gridHalf;

      // Get real grid vectors
      FVector offset = gridOffset * cellSize;
      FVector location = origin + offset;

      // Find nearest path segment to current cell
      int32 index = 0;
      float distance = NAN;
      for (int32 k = 0; k < currentPath.Num(); ++k)
      {
        float currentDistance = FVector::Distance(currentPath[k].GetLocation(), location);
        if (currentDistance < distance)
        {
          index = k;
          distance = currentDistance;
        }
      }

      // Compute flow direction
      FVector direction = currentPath[index].GetLocation() - location;
      direction.Normalize();

      // Insert new flow map nodes
      flowMapNodes.Emplace(FFlowMapNode{ location, direction, 1.0f });

      // Draw some debugs
      if (enableDebug)
      {
        DrawDebugBox(world, origin + offset, FVector{ cellHalf.X, cellHalf.Y, 10.0f }, FColor::White, debugPersistence);
        DrawDebugLine(world, origin + offset, origin + offset + direction * (cellSize / 2.0f), FColor::Yellow, debugPersistence);
      }
    }
  }
}