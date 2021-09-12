#include "TorchAIController.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"
#include "TorchAICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

ATorchAIController::ATorchAIController()
{

}

void ATorchAIController::BeginPlay()
{
  Super::BeginPlay();

  // Subscribe to custom physics
  mOnCalculateCustomPhysics.BindUObject(this, &ATorchAIController::PhysicsTick);
}
void ATorchAIController::Tick(float deltaTime)
{
  Super::Tick(deltaTime);

  // Get some samples
  ComputePath();

  mCurrentPath = mPathSamples[0].Path; // Find algortihm for me

  // Get flow-map nodes
  ComputeFlowMap();

  // Enable physics sub-stepping
  ACharacter* character = GetCharacter();
  if (character)
  {
    character->GetCapsuleComponent()->GetBodyInstance()->AddCustomPhysics(mOnCalculateCustomPhysics);
  }
}
void ATorchAIController::PhysicsTick(float deltaTime, FBodyInstance* bodyInstance)
{
  // Switch path following behavior
  switch (mPathFollowingState)
  {
    case EPathFollowingState::None:
    {
      mPathFollowingState = EPathFollowingState::Moving;
      break;
    }
    case EPathFollowingState::Idle:
    {
      break;
    }
    case EPathFollowingState::Moving:
    {
      // Follow path
      FollowPath(bodyInstance);
      break;
    }
  }
}

void ATorchAIController::ComputePath()
{
  ACharacter* character = GetCharacter();
  UWorld* world = GetWorld();
  if (character && world)
  {
    // Setup path finding
    FVector origin = character->GetActorLocation();
    FVector target = FVector{ 0.0f, 0.0f, 88.0f };
    FRotator rotation = character->GetActorRotation();
    FVector normal = character->GetActorUpVector();
    FVector right = character->GetActorRightVector();

    // Clear old samples
    mPathSamples.Empty();

    // Begin sampling
    for (int32 sample = 0; sample < mNumSamples; ++sample)
    {
      // Switch behavior based on different environmental properties
      switch (mSurfaceState)
      {
        case ESurfaceState::Grounded:
        {
          // Begin sub sampling
          for (int32 subSample = 0; subSample <= mNumSubSamples; ++subSample)
          {
            // Get current view angle
            float randomSteeringAngle = subSample * ((mViewAngle * 2.0f) / mNumSubSamples) - mViewAngle;

            // Sample path
            FPathFinding::SampleOptimalPathAlongSurfaces(
              world,
              origin,
              target,
              rotation.Quaternion(),
              mNumPathSegments,
              mTraceSphereRadius,
              randomSteeringAngle,
              mRandomRotationIntensity, mTargetRotationIntensity,
              mPathSamples[mPathSamples.Emplace()],
              normal,
              mEnableDebug,
              mDebugPersistence);
          }

          // Choose best sub-path to continue on
          mPathSamples.Sort();

          // Validate samples
          if (mPathSamples.Num() > 0)
          {
            int32 optimalIndex = 0;
            if (mPathSamples[optimalIndex].Path.Num() > 0)
            {
              // Promote new origin and rotation
              origin = mPathSamples[optimalIndex].Path[mPathSamples[optimalIndex].Path.Num() - 1].GetLocation();
              rotation = mPathSamples[optimalIndex].Path[mPathSamples[optimalIndex].Path.Num() - 1].GetRotation().Rotator();
            }
          }
        }
        case ESurfaceState::Airborne:
        {
          break;
        }
      }
    }
  }
}
void ATorchAIController::ComputeFlowMap()
{
  ACharacter* character = GetCharacter();
  UWorld* world = GetWorld();
  if (character && world)
  {
    FPathFinding::SampleFlowMap(
      world,
      character->GetActorLocation(),
      mFlowMapGridSize,
      mFlowMapCellSize,
      mCurrentPath,
      mFlowMapNodes,
      mEnableDebug,
      mDebugPersistence
    );
  }
}

void ATorchAIController::FollowPath(FBodyInstance* bodyInstance)
{
  // TODO: try spherical GPU depth map

  // Currently linearly interpolate between samples (also test physics based approach)
  //FVector sampleLocation = FMath::Lerp(
  //  mPathSamples[mCurrentPathIndex].Path[mCurrentSegmentIndex].GetLocation(),
  //  mPathSamples[mCurrentPathIndex].Path[mCurrentSegmentIndex + 1].GetLocation(),
  //  0.0f);
  //FQuat sampleRotation = FMath::Lerp(
  //  mPathSamples[mCurrentPathIndex].Path[mCurrentSegmentIndex].GetRotation(),
  //  mPathSamples[mCurrentPathIndex].Path[mCurrentSegmentIndex + 1].GetRotation(),
  //  0.0f);

  // Set new location and rotation
  //character->SetActorLocationAndRotation(sampleLocation, sampleRotation);

  // Apply direction force
  bodyInstance->AddForce(FVector::RightVector, false);
}

void ATorchAIController::DrawDebugGizmo()
{
  if (false)
  {
    // Draw target location
    //DrawDebugLine(GetWorld(), mAICharacter->GetActorLocation(), mTargetLocation, FColor::White, false);

    // Draw segment target location
    //if (GetPathFollowingComponent()->HasValidPath())
    //{
    //  FNavPathSharedPtr path = GetPathFollowingComponent()->GetPath();
    //  int32 pathIndex = GetPathFollowingComponent()->GetNextPathIndex();
    //  DrawDebugLine(GetWorld(), mAICharacter->GetActorLocation(), path->GetPathPointLocation(pathIndex).Position, FColor::White, false);
    //}

    // Draw perception radius
    //DrawDebugSphere(GetWorld(), mAICharacter->GetActorLocation(), mSearchRadius, 32, FColor::White, false);

    // Draw optimal path
    //if (GetPathFollowingComponent()->HasValidPath())
    //{
    //  FNavPathSharedPtr optimalPath = GetPathFollowingComponent()->GetPath();
    //  int32 pathSize = optimalPath->GetPathPoints().Num();
    //  FVector floorOffset = FVector{ 0.0f, 0.0f, 88.0f };
    //  for (int32 i = 0; i < pathSize - 1; ++i)
    //  {
    //    FVector currLocation = optimalPath->GetPathPointLocation(i).Position;
    //    FVector nextLocation = optimalPath->GetPathPointLocation(i + 1).Position;
    //    DrawDebugLine(GetWorld(), currLocation, currLocation + floorOffset, FColor::White, false);
    //    if (i < pathSize - 1)
    //    {
    //      DrawDebugLine(GetWorld(), currLocation, nextLocation, FColor::White, false);
    //    }
    //  }
    //}

    // Draw desired path
    //if (GetPathFollowingComponent()->HasValidPath())
    //{
    //  FNavPathSharedPtr optimalPath = GetPathFollowingComponent()->GetPath();
    //  TArray<FVector> desiredPath;
    //  GetDesiredPath(optimalPath, desiredPath, 16);
    //  int32 pathSize = desiredPath.Num();
    //  FVector floorOffset = FVector{ 0.0f, 0.0f, 88.0f };
    //  for (int32 i = 0; i < pathSize - 1; ++i)
    //  {
    //    FVector currLocation = desiredPath[i];
    //    FVector nextLocation = desiredPath[i + 1];
    //    DrawDebugLine(GetWorld(), currLocation + floorOffset, nextLocation + floorOffset, FColor::White, false);
    //  }
    //}

    // Draw alternative path based on current rotation
    //if (mTargetActor)
    //{
    //  TArray<TPathNode> path;
    //  FVector origin = mAICharacter->GetActorLocation();
    //  FVector right = mAICharacter->GetActorRightVector();
    //  FVector up = mAICharacter->GetActorUpVector();
    //  FVector forward = mAICharacter->GetActorForwardVector();
    //  FRotator rotation = mAICharacter->GetActorRotation();
    //  FVector target = mTargetActor->GetActorLocation();
    //  TPathFinding::SamplePathAlongSurface(
    //    GetWorld(),
    //    origin,
    //    right, up, forward,
    //    rotation.Quaternion(),
    //    target,
    //    100,
    //    mTraceSphereRadius,
    //    mAirSampleDistance, mAirDeltaCorrection,
    //    mHitSampleDistance, mHitDeltaCorrection,
    //    path);
    //  int32 pathSize = path.Num();
    //  if (pathSize > 1)
    //  {
    //    for (int32 i = 0; i < pathSize - 1; ++i)
    //    {
    //      //DrawDebugLine(GetWorld(), path[i].Location, path[i + 1].Location, FColor{ 0, 0, 255, 64 }, false);
    //      if (i % 2 == 0)
    //      {
    //        //DrawDebugSphere(GetWorld(), path[i].Location, 44.0f, 16, FColor{ 255, 128, 255, 64 }, false);
    //        //FVector transform = FVector::ZeroVector;
    //        //transform.RotateAngleAxis(FVector::DotProduct(right, path[i].Tangent), right);
    //        //transform.RotateAngleAxis(FVector::DotProduct(up, path[i].Normal), up);
    //        DrawDebugLine(GetWorld(), path[i].Location, path[i].Location + path[i].Normal * mTraceSphereRadius, FColor::Blue, false);
    //        DrawDebugLine(GetWorld(), path[i].Location, path[i].Location + path[i].Tangent * mTraceSphereRadius, FColor::Green, false);
    //        DrawDebugLine(GetWorld(), path[i].Location, path[i].Location + path[i].BiTangent * mTraceSphereRadius, FColor::Red, false);
    //        DrawDebugBox(GetWorld(), path[i].Location, FVector{ mTraceSphereRadius }, rotation.Quaternion(), FColor::Blue, false);
    //      }
    //    }
    //  }
    //  else
    //  {
    //    DrawDebugLine(GetWorld(), path[0].Location, path[0].Location + path[0].Normal * mTraceSphereRadius, FColor::Blue, false);
    //    DrawDebugLine(GetWorld(), path[0].Location, path[0].Location + path[0].Tangent * mTraceSphereRadius, FColor::Green, false);
    //    DrawDebugLine(GetWorld(), path[0].Location, path[0].Location + path[0].BiTangent * mTraceSphereRadius, FColor::Red, false);
    //  }
    //  DrawDebugLine(GetWorld(), origin, target, FColor::Yellow, false);
    //}
  }
}