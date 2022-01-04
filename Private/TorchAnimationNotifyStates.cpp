#include "TorchAnimationNotifyStates.h"
#include "TorchCore.h"
#include "TorchFPSCharacter.h"
#include "TorchAICharacter.h"
#include "TorchBlueprintUtils.h"

void UTorchFPSAnimationNotifyState::NotifyBegin(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation, float totalDuration)
{
  ATorchFPSCharacter* character = meshComp->GetOwner<ATorchFPSCharacter>();
  if (character)
  {
    character->DisableHandIK();
  }
}
void UTorchFPSAnimationNotifyState::NotifyTick(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation, float frameDeltaTime)
{

}
void UTorchFPSAnimationNotifyState::NotifyEnd(USkeletalMeshComponent* meshComp, UAnimSequenceBase* animation)
{
  ATorchFPSCharacter* character = meshComp->GetOwner<ATorchFPSCharacter>();
  if (character)
  {
    character->EnableHandIK();
  }
}