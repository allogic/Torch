#include "TorchDebug.h"
#include "DrawDebugHelpers.h"

void FTorchDebug::DrawDebugAxisCross(UWorld* world, const FVector& location, const FVector& right, const FVector& up, const FVector& forward, float scale)
{
  DrawDebugLine(world, location, location + right * scale, FColor::Red, false, -1.0f, 0, 1.0f);
  DrawDebugLine(world, location, location + up * scale, FColor::Green, false, -1.0f, 0, 1.0f);
  DrawDebugLine(world, location, location +forward * scale, FColor::Blue, false, -1.0f, 0, 1.0f);
}