#pragma once

#include "CoreMinimal.h"
#include "TorchAnimationInstance.generated.h"

class UAnimInstance;
class UAnimMontage;

UCLASS()
class TORCH_API UTorchAnimationInstance : public UAnimInstance
{
  GENERATED_BODY()

public:

  void Play(UAnimMontage* animationMontage, const FName& sectionName = TEXT(""), float playRate = 1.0f, bool registerCallbacks = false);
  void Stop(UAnimMontage* animationMontage, float blendOutTime = 0.2f);

private:

  UFUNCTION()
  void OnMontageBlendOut(UAnimMontage* animationMontage, bool interrupted);
  UFUNCTION()
  void OnMontageEnd(UAnimMontage* animationMontage, bool interrupted);

private:

  /*
  * Internal state
  */

  bool mAllowNextBlend = true;
};
