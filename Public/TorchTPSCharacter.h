#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TorchTPSCharacter.generated.h"

UCLASS()
class TORCH_API ATorchTPSCharacter : public ACharacter
{
  GENERATED_BODY()

public:
  ATorchTPSCharacter();

protected:
  virtual void BeginPlay() override;
  virtual void Tick(float deltaTime) override;
  virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;

private:

  UFUNCTION()
  void AlignActorRotation();
};