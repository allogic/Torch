#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "TorchTPSCameraManager.generated.h"

class ATorchTPSCharacter;

UCLASS()
class TORCH_API ATorchTPSCameraManager : public APlayerCameraManager
{
  GENERATED_BODY()

private:

  /*
  * Player reference
  */

  ATorchTPSCharacter* mTPSCharacter = nullptr;
};