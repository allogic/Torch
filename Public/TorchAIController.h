#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TorchAIController.generated.h"

class ATorchCharacter;

UCLASS()
class TORCH_API ATorchAIController : public AAIController
{
  GENERATED_BODY()

public:

  ATorchAIController();

protected:

  virtual void BeginPlay() override;
  virtual void Tick(float deltaTime) override;
  virtual void OnPossess(APawn* pawn) override;
  virtual void OnUnPossess() override;

private:

  /*
  * Possessed character
  */

  ATorchCharacter* mCharacter = nullptr;

private:

  /*
  * Locomotion system
  */

  void Stop();
  void WalkForward();

private:

  /*
  * Path finding
  */

  FNavPathSharedPtr mPath = {};

private:

  /*
  * Path following
  */

  int32 mCurrentPathIndex = 0;
  float mAcceptanceRadius = 100.0f;
};