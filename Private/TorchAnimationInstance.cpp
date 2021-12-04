#include "TorchAnimationInstance.h"
#include "Animation/AnimInstance.h"

void UTorchAnimationInstance::Play(UAnimMontage* animationMontage, const FName& sectionName, float playRate, bool registerCallbacks)
{
  if (mAllowNextBlend)
  {
    if (!UAnimInstance::Montage_IsPlaying(animationMontage))
    {
      UAnimInstance::Montage_JumpToSection(sectionName, animationMontage);
      UAnimInstance::Montage_PlayWithBlendIn(animationMontage, FAlphaBlendArgs{}, playRate);

      if (registerCallbacks)
      {
        mAllowNextBlend = false;

        FOnMontageEnded blendOutDelegate;
        FOnMontageEnded endDelegate;

        blendOutDelegate.BindUObject(this, &UTorchAnimationInstance::OnMontageBlendOut);
        endDelegate.BindUObject(this, &UTorchAnimationInstance::OnMontageEnd);

        UAnimInstance::Montage_SetBlendingOutDelegate(blendOutDelegate, animationMontage);
        UAnimInstance::Montage_SetEndDelegate(endDelegate, animationMontage);
      }
    }
  }
}
void UTorchAnimationInstance::Stop(UAnimMontage* animationMontage, float blendOutTime)
{
  if (UAnimInstance::Montage_IsPlaying(animationMontage))
  {
    UAnimInstance::Montage_StopWithBlendOut(FAlphaBlend{ blendOutTime }, animationMontage);
  }
}

void UTorchAnimationInstance::OnMontageBlendOut(UAnimMontage* animationMontage, bool interrupted)
{
  mAllowNextBlend = true;
}
void UTorchAnimationInstance::OnMontageEnd(UAnimMontage* animationMontage, bool interrupted)
{

}