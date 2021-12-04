#pragma once

#include "CoreMinimal.h"
#include "TorchCharacter.h"
#include "TorchCharacterPuker.generated.h"

class UAnimMontage;

UCLASS()
class TORCH_API ATorchCharacterPuker : public ATorchCharacter
{
  GENERATED_BODY()

public:

  ATorchCharacterPuker(const FObjectInitializer& initializer);

protected:

  virtual void BeginPlay() override;
};