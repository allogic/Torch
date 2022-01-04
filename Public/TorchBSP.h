#pragma once

#include "CoreMinimal.h"

struct FBSPNode
{
  int32 ID;
  EAxis::Type SplitAxis;
  bool IsEnd;
  FVector Location;
  FVector Size;
  FBSPNode* Parent;
  FBSPNode* LeafLeft;
  FBSPNode* LeafRight;
};

struct FTorchBSP
{
  static void Create(
    FBSPNode*& node,
    int32 depth,
    int32 minRoomArea,
    const FVector& location,
    const FVector& size,
    const FVector& splitCorrection = FVector{ 0.2f },
    float selectionPercentage = 0.0f);

  static void Create(
    FBSPNode* parent,
    FBSPNode*& node,
    int32 depth,
    int32 minRoomArea,
    int32& id,
    const FVector& location,
    const FVector& size,
    const FVector& splitCorrection = FVector{ 0.2f },
    float selectionPercentage = 0.0f);

  static void Destroy(
    FBSPNode*& node);

  static void QueryChildren(
    FBSPNode* node,
    TArray<FBSPNode*>& children);
};