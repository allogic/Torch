#include "TorchTrace.h"

void FTorchTrace::Test()
{
  // Check if blocked
  //TArray<FHitResult> hitResults;
  //FCollisionQueryParams queryParams;
  //queryParams.AddIgnoredActor(this);
  //queryParams.AddIgnoredComponent(GetCapsuleComponent());
  //queryParams.AddIgnoredComponent(GetMesh());
  //queryParams.AddIgnoredComponent(mPerceptionSphere);
  //FCollisionResponseParams responseParams;
  //responseParams.CollisionResponse.SetAllChannels(ECollisionResponse::ECR_Ignore);
  //responseParams.CollisionResponse.SetResponse(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
  //FCollisionShape collisionShape;
  //collisionShape.ShapeType = ECollisionShape::Capsule;
  //collisionShape.SetCapsule(GetCapsuleComponent()->GetScaledCapsuleRadius(), GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
  //DrawDebugCapsule(GetWorld(), GetActorLocation() + GetActorForwardVector() * 300.0f, GetCapsuleComponent()->GetScaledCapsuleHalfHeight(), GetCapsuleComponent()->GetScaledCapsuleRadius(), GetActorRotation().Quaternion(), FColor::Red, false, -1.0f, 0, 2.0f);
  //if (GetWorld()->SweepMultiByChannel(hitResults, GetActorLocation(), GetActorForwardVector() * 300.0f, FQuat{}, ECollisionChannel::ECC_GameTraceChannel1, collisionShape))
  //{
  //  TORCH_LOG("hit");
  //}
}