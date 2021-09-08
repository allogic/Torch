#include "TorchPathFinding.h"
#include "DrawDebugHelpers.h"
#include "TorchMath.h"

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

      // Apply escape rotation
      FRotator r = rotation.Rotator();
      r.Roll += FMath::RadiansToDegrees(FVector::DotProduct(rotation.GetRightVector(), normal));
      FQuat targetRotation = r.Quaternion() * rotation.Inverse();
      rotation += (targetRotation * rotation);
      rotation.Normalize();
    }
    else
    {
      // Apply random rotation
      FRotator r = rotation.Rotator();
      r.Yaw += randomSteeringAngle;
      FQuat randomRotationNew = r.Quaternion() * rotation.Inverse();
      rotation += (randomRotationNew * rotation) * randomRotationItensity;
      rotation.Normalize();

      // Apply target rotation
      float targetDirectionDistIntensity = (1.0f / numSegments) * segment;
      FQuat targetRotationDiff = targetDirection.Rotation().Quaternion() * rotation.Inverse();
      rotation += (targetRotationDiff * rotation) * targetDirectionDistIntensity * targetRotationItensity;
      rotation.Normalize();

      // March along forward
      origin += rotation.GetForwardVector() * sphereRadius;

      // Priority rules
      //sample.Length += FVector::Distance(origin, origin + rotation.GetForwardVector() * sphereRadius);
      sample.DistanceToTarget = FVector::Distance(origin, target);

      // Add transform to path
      sample.Path.Emplace(FTransform{ rotation, origin });

      // Draw some debugs
      if (enableDebug)
      {
        if (segment % 2 == 0)
        {
          DrawDebugBox(world, origin, FVector{ 10.0f, 44.0f, 44.0f }, rotation, FColor{ 255, 255, 255, 64 }, debugPersistence);
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
}