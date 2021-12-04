#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TorchTypes.h"
#include "TorchAnimationInstance.h"
#include "TorchCharacter.generated.h"

class UAnimInstance;
class UAnimMontage;
class USphereComponent;
class ATorchTPSCharacter;
class UPhysicalAnimationComponent;

UCLASS()
class TORCH_API ATorchCharacter : public ACharacter
{
  GENERATED_BODY()

public:

  /*
  * Debugging
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchDebug)
  bool mEnableDebug = true;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchDebug)
  bool mDrawPerceptionSphere = false;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchDebug)
  bool mDrawSharedPath = true;

  /*
  * Perception system
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPerception)
  float mPerceptionSphereRadius = 2000.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPerception)
  USphereComponent* mPerceptionSphere;

  /*
  * Physical animation
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPhysicalAnimation)
  UPhysicalAnimationComponent* mPhysicalAnimationComponent = nullptr;

  /*
  * Movement system
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMovement)
  float mRotationSpeed = 2.0f;

  /*
  * Path finding
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  bool mFollowPath = true;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  bool mEnableSharedPath = false;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPathFinding)
  float mAcceptanceRadius = 50.0f;

  /*
  * Combat system
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchCombat)
  float mAttackRadius = 250.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchCombat)
  float mEvadePropability = 0.995f;

public:

  ATorchCharacter(const FObjectInitializer& initializer);

protected:

  virtual void BeginPlay() override;
  virtual void Tick(float deltaTime) override;

private:

  /*
  * Path finding
  */

  FNavPathSharedPtr mPath = nullptr;

  bool mUpdateTarget = true;
  bool mIsSharedPath = false;

  FVector mRelativeLocation = FVector::ZeroVector;
  FVector mTargetLocation = FVector::ZeroVector;

  FTimerDelegate mTimerDelegateFindPath;
  FTimerHandle mTimerHandleFindPath;

  UFUNCTION()
  void UpdatePath();

private:

  /*
  * Path following
  */

  int32 mSharedPathKey = 0;
  int32 mCurrentPathIndex = 0;

  bool IsRestPathEven(float maxAngle = 10.0f);
  EHemisphere::Type ComputeHemisphere(const FVector& direction);
  void FollowPath();

protected:

  /*
  * Animation system
  */

  UTorchAnimationInstance* mAnimationInstance = nullptr;

  UAnimMontage* mMontageWalk = nullptr;
  UAnimMontage* mMontageWalkAggressive = nullptr;
  UAnimMontage* mMontageRun = nullptr;

  UAnimMontage* mMontageTurnLeft90 = nullptr;
  UAnimMontage* mMontageTurnLeft180 = nullptr;
  UAnimMontage* mMontageTurnRight90 = nullptr;
  UAnimMontage* mMontageTurnRight180 = nullptr;

  UAnimMontage* mMontageEvadeLeft = nullptr;
  UAnimMontage* mMontageEvadeRight = nullptr;

  UAnimMontage* mMontageAttack = nullptr;

private:

  /*
  * Perception system
  */

  ATorchTPSCharacter* mPlayer = nullptr;
  TArray<ATorchCharacter*> mEnemies = {};

  bool mCanSeePlayer = false;

  UFUNCTION()
  void OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
  UFUNCTION()
  void OnEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

private:

  /*
  * Combat system
  */

  bool mEvading = false;

private:

  /*
  * Debugging
  */

  void Debug();
};