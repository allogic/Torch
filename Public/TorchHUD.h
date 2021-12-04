#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TorchHUD.generated.h"

class UTexture2D;

UCLASS()
class TORCH_API ATorchHUD : public AHUD
{
  GENERATED_BODY()

public:

  ATorchHUD();

public:

  /*
  * Debugging
  */

  void SetDebugTexture(uint8* source, int32 width, int32 height);

protected:

  virtual void BeginPlay() override;
  virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;
  virtual void DrawHUD() override;

private:

  /*
  * Debugging
  */

  UTexture2D* mDebugTexture = nullptr;
};