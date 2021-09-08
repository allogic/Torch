#include "TorchMath.h"

FVector FMafh::ComputeOptimalSurfaceDirection(FVector target, FVector normal, FVector up)
{
  FVector surfaceDirection = FVector::CrossProduct(up, normal);
  surfaceDirection.Normalize();
  return surfaceDirection;
}
FVector FMafh::EvaluateCatRom(FVector p0, FVector p1, FVector p2, FVector p3, float time)
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