#pragma once

#include "CoreMinimal.h"

class ATorchAIController;

struct FTorchBehaviorTask
{
  static bool MoveToRandomLocationInsideSphere(ATorchAIController* AIController, float randomRadius, float acceptanceRadius);
  static bool MoveToPlayerLocation(ATorchAIController* AIController, float acceptanceRadius);
};