#pragma once

#include "CoreMinimal.h"

#define TORCH_LOG(FMT, ...) GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT(FMT), __VA_ARGS__));

#define TORCH_DO_ONCE(EXPR) \
{                           \
  static bool ctrl = true;  \
  if (ctrl)                 \
  {                         \
    EXPR;                   \
  }                         \
}