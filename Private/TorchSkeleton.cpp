#include "TorchSkeleton.h"

void FTorchSkeleton::GetCharacterBoneRotation(const FRotator& actorRotation, const FRotator& boneRotation, FVector& right, FVector& up, FVector& forward)
{
  right = boneRotation.RotateVector(right);
  up = boneRotation.RotateVector(up);
  forward = boneRotation.RotateVector(forward);

  right = actorRotation.RotateVector(right);
  up = actorRotation.RotateVector(up);
  forward = actorRotation.RotateVector(forward);
}
void FTorchSkeleton::GetInverseBonePitch(const FRotator& boneRotationSource, const FRotator& boneRotationTarget)
{
  
}