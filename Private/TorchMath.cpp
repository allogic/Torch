#include "TorchMath.h"

FVector FMafh::ComputeOptimalSurfaceDirection(FVector target, FVector normal, FVector up)
{
  FVector surfaceDirection = FVector::CrossProduct(up, normal);
  surfaceDirection.Normalize();
  return surfaceDirection;
}

void FMafh::EvaluateCatRom(const TArray<FVector>& points, uint32 numSamples, TArray<FVector>& samplePoints)
{
  uint32 numPoints = FMath::Floor(points.Num() / 4);
  for (uint32 i = 0; i < numPoints; i += 4)
  {
    const FVector& p0 = points[i + 0];
    const FVector& p1 = points[i + 1];
    const FVector& p2 = points[i + 2];
    const FVector& p3 = points[i + 3];

    float t01 = FMath::Pow(FVector::Distance(p0, p1), 1.0f);
    float t12 = FMath::Pow(FVector::Distance(p1, p2), 1.0f);
    float t23 = FMath::Pow(FVector::Distance(p2, p3), 1.0f);

    FVector m1 = (1.0f - 0.5f) * (p2 - p1 + t12 * ((p1 - p0) / t01 - (p2 - p0) / (t01 + t12)));
    FVector m2 = (1.0f - 0.5f) * (p2 - p1 + t12 * ((p3 - p2) / t23 - (p3 - p1) / (t12 + t23)));

    FVector s0 = 2.0f * (p1 - p2) + m1 + m2;
    FVector s1 = -3.0f * (p1 - p2) - m1 - m1 - m2;
    FVector s2 = m1;
    FVector s3 = p1;

    float q = 1.0f / (float)(numSamples - 1);

    for (float t = 0; t < 1.0f; t += q)
    {
      FVector samplePoint = s0*t*t*t + s1*t*t + s2*t + s3;
      samplePoints.Emplace(samplePoint);
    }
  }
}