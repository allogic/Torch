#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "TorchAnimationNotifyStates.generated.h"

UCLASS()
class TORCH_API UTorchAnimationNotifyStatesA : public UAnimNotifyState
{
  GENERATED_BODY()

public:
  virtual void NotifyBegin(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation, float totalDuration) override;
  virtual void NotifyTick(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation, float frameDeltaTime) override;
  virtual void NotifyEnd(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation) override;
};

UCLASS()
class TORCH_API UTorchAnimationNotifyStatesB : public UAnimNotifyState
{
  GENERATED_BODY()

public:
  virtual void NotifyBegin(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation, float totalDuration) override;
  virtual void NotifyTick(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation, float frameDeltaTime) override;
  virtual void NotifyEnd(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation) override;
};