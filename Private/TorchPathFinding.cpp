#include "TorchPathFinding.h"
#include "DrawDebugHelpers.h"
#include "TorchMath.h"

bool FPathFinding::SamplePathAirborn(
  UWorld* world,
  FVector origin,
  FVector target,
  FQuat rotation,
  int32 numSegments,
  float sphereRadius,
  FPathSample& sample,
  bool debug)
{
  return false;
}
bool FPathFinding::SamplePathSurface(
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
  bool debug)
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
      //normal = hitResult.Normal;
      return true;
    }
    else
    {
      // Apply random rotation
      //float noiseRotation = FMath::PerlinNoise3D(origin * 0.1f);
      //float randomYaw = FMath::RandRange(-20.0f, 20.0f);
      FRotator r = rotation.Rotator();
      r.Yaw += randomSteeringAngle;
      FQuat noiseRotationNew = r.Quaternion() * rotation.Inverse();
      rotation += (noiseRotationNew * rotation) * randomRotationItensity;
      rotation.Normalize();

      // Apply target rotation
      FQuat targetRotationDiff = targetDirection.Rotation().Quaternion() * rotation.Inverse();
      rotation += (targetRotationDiff * rotation) * targetRotationItensity;
      rotation.Normalize();

      // March along forward
      origin += rotation.GetForwardVector() * sphereRadius;

      // Priority rules
      //sample.Length += FVector::Distance(origin, origin + rotation.GetForwardVector() * sphereRadius);
      sample.DistanceToTarget = FVector::Distance(origin, target);

      // Add transform to path
      sample.Path.Emplace(FTransform{ rotation, origin });

      // Draw some debugs
      if (debug)
      {
        if (segment % 2 == 0)
        {
          DrawDebugBox(world, origin, FVector{ 10.0f, 44.0f, 44.0f }, rotation, FColor::White, false);
        }
        else
        {
          DrawDebugLine(world, origin, origin + rotation.GetRightVector() * 200.0f, FColor::Red);
          DrawDebugLine(world, origin, origin + rotation.GetUpVector() * 200.0f, FColor::Green);
          DrawDebugLine(world, origin, origin + rotation.GetForwardVector() * 200.0f, FColor::Blue);
        }
      }
    }
  }
  return false;
}