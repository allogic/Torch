#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TorchAnimationInstance.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "TorchAICharacter.generated.h"

class UAnimInstance;
class UAnimMontage;
class USphereComponent;
class UPhysicalAnimationComponent;

class ATorchFPSCharacter;

UCLASS()
class TORCH_API UTorchAICharacterNotifyCombat : public UAnimNotifyState
{
  GENERATED_BODY()

public:

  virtual void NotifyBegin(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation, float totalDuration) override;
  virtual void NotifyTick(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation, float frameDeltaTime) override;
  virtual void NotifyEnd(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation) override;
};

UCLASS()
class TORCH_API ATorchAICharacter : public ACharacter
{
  GENERATED_BODY()

public:

  /*
  * Debugging
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchDebug)
  bool mEnableDebug = true;

  /*
  * Components
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  UPhysicalAnimationComponent* mPhysicalAnimationComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  USphereComponent* mPerceptionSphereComponent = nullptr;

  /*
  * Perception system
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPerception)
  float mPerceptionSphereRadius = 3000.0f;

public:

  /*
  * Controller access
  */

  ATorchFPSCharacter* GetPlayerCharacter();
  void EnableControllerRotation();
  void DisableControllerRotation();

public:

  ATorchAICharacter();

protected:

  virtual void BeginPlay() override;
  virtual void Tick(float deltaTime) override;

public:

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

  virtual void StopWalkAnimation() {};
  virtual void PlayWalkAnimation(float playRate, float movementSpeed) {};
  virtual void PlayRunAnimation(float playRate, float movementSpeed) {};
  virtual void PlayAttackAnimation(float playRate) {};

  virtual void UpdateAnimationBlueprint() {}

private:

  /*
  * Perception system
  */

  ATorchFPSCharacter* mPlayer = nullptr;
  TArray<ATorchAICharacter*> mEnemies = {};

  UFUNCTION()
  void OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
  UFUNCTION()
  void OnEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

public:

  /*
  * Combat system
  */

  bool mIsAttacking = false;

  FORCEINLINE void IsAttacking(bool value) { mIsAttacking = value; }
  FORCEINLINE bool IsAttacking() { return mIsAttacking; }

private:

  /*
  * Debugging
  */

  void Debug();
};