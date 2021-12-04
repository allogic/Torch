#include "TorchMath.h"

FVector FTorchMath::RandomInsideUnitCircle()
{
  return FMath::FRand() * FRotator(0.0f, FMath::FRandRange(0, 360), 0).Vector();
}
FVector FTorchMath::RandomInsideUnitSphere()
{
  return FMath::FRand() * FRotator(FMath::FRandRange(-90, 90), FMath::FRandRange(0, 360), 0).Vector();
}

FTransform FTorchMath::LookAt(const FVector& source, const FVector& target, const FVector& up)
{
  FVector newForward = target - source;
  FVector newRight = FVector::CrossProduct(target, up);
  FVector newUp = FVector::CrossProduct(newRight, newForward);
  return FTransform{ newForward, newRight, newUp, source };
}
FQuat FTorchMath::RelativeRotation(const FQuat& source, const FQuat& target)
{
  FQuat delta = target * source.Inverse();
  delta = delta * source;
  return delta;
}

FVector FTorchMath::ComputeOptimalSurfaceDirection(const FVector& target, const FVector& normal, const FVector& up)
{
  FVector surfaceDirection = FVector::CrossProduct(up, normal);
  surfaceDirection.Normalize();
  return surfaceDirection;
}
FVector FTorchMath::EvaluateCatRom(const FVector& p0, const FVector& p1, const FVector& p2, const FVector& p3, float time)
{
  float t01 = FMath::Pow(FVector::Distance(p0, p1), 1.0f);
  float t12 = FMath::Pow(FVector::Distance(p1, p2), 1.0f);
  float t23 = FMath::Pow(FVector::Distance(p2, p3), 1.0f);

  FVector m1 = (1.0f - 0.5f) * (p2 - p1 + t12 * ((p1 - p0) / t01 - (p2 - p0) / (t01 + t12)));
  FVector m2 = (1.0f - 0.5f) * (p2 - p1 + t12 * ((p3 - p2) / t23 - (p3 - p1) / (t12 + t23)));

  FVector s0 = 2.0f * (p1 - p2) + m1 + m2;
  FVector s1 = -3.0f * (p1 - p2) - m1 - m1 - m2;
  FVector s2 = m1;
  FVector s3 = p1;

  return s0 * time * time * time + s1 * time * time + s2 * time + s3;
}

float FTorchMath::RemapValue(float value, float low0, float high0, float low1, float high1)
{
  return low1 + (value - low0) * (high1 - low1) / (high0 - low0);
}