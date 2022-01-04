#pragma once

#include "CoreMinimal.h"
#include "TorchAICharacter.h"
#include "TorchAICharacterPuker.generated.h"

UCLASS()
class TORCH_API ATorchAICharacterPuker : public ATorchAICharacter
{
  GENERATED_BODY()

public:

  /*
  * Root IK
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchRootIK)
  float mLeanIntensity = 1.0f;

  /*
  * Foot IK
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchFootIK)
  float mFootUpdateTime = 0.03f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchFootIK)
  float mFootBezierBeginScale = 60.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchFootIK)
  float mFootBezierEndScale = 120.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchFootIK)
  float mFootBezierVelocityScale = 0.01f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchFootIK)
  float mFootLeftRayVelocityScale = 1.2f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchFootIK)
  float mFootRightRayVelocityScale = 0.8f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchFootIK)
  float mFootVelocityKillAngle = 10.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchFootIK)
  float mFootLeftCorrectionDistance = 30.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchFootIK)
  float mFootRightCorrectionDistance = 30.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchFootIK)
  FVector mFootBezierBeginDirection = FVector{ 1.0f, 0.0f, 1.0f };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchFootIK)
  FVector mFootBezierEndDirection = FVector{ 0.0f, 0.0f, 1.0f };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchFootIK)
  FVector mFootLeftLocationOffset = FVector{ 0.0f, -35.0f, 0.0f };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchFootIK)
  FVector mFootRightLocationOffset = FVector{ 0.0f, 50.0f, 0.0f };


public:

  ATorchAICharacterPuker();

protected:

  virtual void BeginPlay() override;
  virtual void Tick(float deltaTime) override;

private:

  /*
  * Animation system
  */

  TArray<FString> mAnimationMontageWalkSections = { "Normal", "Blind" };
  TArray<FString> mAnimationMontageWalkAggressiveSections = { "Normal", "Limping" };
  TArray<FString> mAnimationMontageAttackSections = { "Normal", "Double", "Special" };

  void StopWalkAnimation() override;
  void PlayWalkAnimation(float playRate, float movementSpeed) override;
  void PlayRunAnimation(float playRate, float movementSpeed) override;
  void PlayAttackAnimation(float playRate) override;

private:

  /*
  * Root IK
  */

  FVector mRootLocation = FVector{};
  FVector mRootVelocity = FVector{};

  void UpdateRootLocation();

private:

  /*
  * Foot IK
  */

  FVector mFootLeftStart = FVector{};
  FVector mFootRightStart = FVector{};

  FVector mFootLeftLocation = FVector{};
  FVector mFootRightLocation = FVector{};
  FVector mFootLeftPrevLocation = FVector{};
  FVector mFootRightPrevLocation = FVector{};
  FVector mFootLeftCurrLocation = FVector{};
  FVector mFootRightCurrLocation = FVector{};

  TArray<FVector> mLeftPath = {};
  TArray<FVector> mRightPath = {};
  float mCurrentLeftTime = 1.0f;
  float mCurrentRightTime = 1.0f;

  int32 mNumPathSegments = 32;

  float mRotationDistance = 0.0f;
  float mVelocityDistance = 0.0f;

  void TraceFootRays();
  void ComputePathSegments(TArray<FVector>& pathSegments, const FVector& previous, const FVector& current);
  void FollowPathSegment();

private:

  /*
  * Animation system
  */

  void UpdateAnimationBlueprint() override;

private:

  /*
  * Debugging
  */

  void Debug();
};