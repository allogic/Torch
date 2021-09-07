#include "TorchAIController.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"
#include "TorchAICharacter.h"
#include "TorchMath.h"
#include "TorchPathFinding.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SplineComponent.h"

ATorchAIController::ATorchAIController()
{
  // Attach spline component
  mSplineComponent = CreateDefaultSubobject<USplineComponent>("Path");
  mSplineComponent->SetupAttachment(RootComponent);
}

void ATorchAIController::BeginPlay()
{
  Super::BeginPlay();

  // Set path prediction timer
  //GetWorldTimerManager().SetTimer(mPredictionTimeHdl, this, &ATorchAIController::PredictPath, 1.0f, true);
}
void ATorchAIController::Tick(float deltaTime)
{
  PredictPath();
}

void ATorchAIController::PredictPath()
{
  ACharacter* character = GetCharacter();
  UWorld* world = GetWorld();
  if (character && world)
  {
    // Setup path finding
    bool hit = false;
    FVector origin = character->GetActorLocation();
    FVector target = FVector{ 0.0f, 0.0f, 88.0f };
    FRotator rotation = character->GetActorRotation();
    EBehaviorState::Type behaviorState = EBehaviorState::Surface;

    // Begin sampling
    for (int32 sample = 0; sample < mNumPredictions; ++sample)
    {
      // Switch behavior based on different environment properties
      switch (behaviorState)
      {
        case EBehaviorState::Surface:
        {
          // Setup 1D view frustum
          FVector direction = rotation.Quaternion().GetForwardVector();
          FVector up = rotation.Quaternion().GetUpVector();

          // Setup path scoring array
          TArray<FPathSample> subPaths;

          // Begin sub sampling
          for (int32 subSample = 0; subSample <= mNumSubPredictions; ++subSample)
          {
            // Get current view angle
            float randomSteeringAngle = subSample * ((mViewAngle * 2.0f) / mNumSubPredictions) - mViewAngle;

            // Sample path
            int32 subIndex = subPaths.Emplace(FPathSample{});
            hit = FPathFinding::SamplePathSurface(world, origin, target, rotation.Quaternion(), mNumPathSegments, mTraceSphereRadius, randomSteeringAngle, mRandomRotationIntensity, mTargetRotationIntensity, subPaths[subIndex], mDebug);
          }

          // Choose best sub-path to continue on
          subPaths.Sort();

          // Promote new origin and rotation
          if (subPaths.Num() > 0)
          {
            TArray<FTransform>& subPath = subPaths[0].Path;
            if (subPath.Num() > 0)
            {
              origin = subPath[subPath.Num() - 1].GetLocation();
              rotation = subPath[subPath.Num() - 1].GetRotation().Rotator();
            }
          }

          // Flip behavior state
          if (hit)
          {
            
          }
          break;
        }
        case EBehaviorState::Airborne:
        {
          break;
        }
      }
    }

    // Update spline with new path
  }
}
void ATorchAIController::FollowPath()
{
  ACharacter* character = GetCharacter();
  if (character)
  {
    //character->GetCharacterMovement()->Add
    //character->SetActorLocationAndRotation(FVector{}, FQuat{});
  }
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