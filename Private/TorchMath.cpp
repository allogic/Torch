#include "TorchMath.h"

FVector FTorchMath::RandomInsideUnitCircle()
{
  return FMath::FRand() * FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f).Vector();
}
FVector FTorchMath::RandomInsideUnitSphere()
{
  return FMath::FRand() * FRotator(FMath::RandRange(-90.0f, 90.0f), FMath::FRandRange(0.0f, 360.0f), 0.0f).Vector();
}

FVector FTorchMath::RandomInsideRect(const FVector& location, const FVector& size)
{
  return FVector{ FMath::RandRange(location.X, location.X + size.X), FMath::RandRange(location.Y, location.Y + size.Y), FMath::RandRange(location.Z, location.Z + size.Z) };
}

FVector FTorchMath::CatmullRom(const FVector& p0, const FVector& p1, const FVector& p2, const FVector& p3, float t)
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
  return s0 * t * t * t + s1 * t * t + s2 * t + s3;
}
FVector FTorchMath::CubicBezier(const FVector& p0, const FVector& c0, const FVector& c1, const FVector& p1, float t)
{
  return p0 * FMath::Pow(1.0f - t, 3.0f) + 3.0f * c0 * FMath::Pow(1.0f - t, 2.0f) * t + 3.0f * c1 * (1.0f - t) * FMath::Pow(t, 2) + p1 * FMath::Pow(t, 3);
}

float FTorchMath::RemapValue(float value, float low0, float high0, float low1, float high1)
{
  return low1 + (value - low0) * (high1 - low1) / (high0 - low0);
}