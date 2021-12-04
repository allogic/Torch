#include "TorchBSP.h"

void FTorchBSP::Create(
  FBSPNode*& node,
  int32 depth,
  int32 minRoomArea,
  const FVector& location,
  const FVector& size,
  const FVector& splitCorrection,
  float selectionPercentage)
{
  int32 id = 1;
  node = (FBSPNode*)FMemory::Malloc(sizeof(FBSPNode));
  (*node) = FBSPNode{ id, EAxis::None, false, location, size, nullptr, nullptr, nullptr };
  FBSPNode* root = node;
  Create(nullptr, node, depth, minRoomArea, ++id, location, size, splitCorrection, selectionPercentage);
  node = root;
}

void FTorchBSP::Create(
  FBSPNode* parent,
  FBSPNode*& node,
  int32 depth,
  int32 minRoomArea,
  int32& id,
  const FVector& location,
  const FVector& size,
  const FVector& splitCorrection,
  float selectionPercentage)
{
  // Create new node
  if (!node)
  {
    node = (FBSPNode*)FMemory::Malloc(sizeof(FBSPNode));
    (*node) = FBSPNode{ id, EAxis::None, false, location, size, parent, nullptr, nullptr };
  }
  // Node is end exiting recursion
  int32 roomArea = size.X * size.Y * size.Z;
  if (depth <= 0 || roomArea < minRoomArea)
  {
    node->IsEnd = true;
    return;
  }
  // Find axis to split
  float longestAxis = 0.0f;
  if (size.X > longestAxis) { longestAxis = size.X; node->SplitAxis = EAxis::X; }
  if (size.Y > longestAxis) { longestAxis = size.Y; node->SplitAxis = EAxis::Y; }
  if (size.Z > longestAxis) { longestAxis = size.Z; node->SplitAxis = EAxis::Z; }
  // Split axis randomly
  int32 randomHalf = 0;
  switch (node->SplitAxis)
  {
    case EAxis::X: randomHalf = FMath::RandRange(0, (int32)size.X); break;
    case EAxis::Y: randomHalf = FMath::RandRange(0, (int32)size.Y); break;
    case EAxis::Z: randomHalf = FMath::RandRange(0, (int32)size.Z); break;
  }
  // Correct random location
  switch (node->SplitAxis)
  {
    case EAxis::X:
    {
      float threshold = (size.X / 100.0f) * splitCorrection.X * 100.0f;
      if (randomHalf < threshold) randomHalf = threshold;
      if (randomHalf > (size.X - threshold)) randomHalf = (size.X - threshold);
      break;
    }
    case EAxis::Y:
    {
      float threshold = (size.Y / 100.0f) * splitCorrection.Y * 100.0f;
      if (randomHalf < threshold) randomHalf = threshold;
      if (randomHalf > (size.Y - threshold)) randomHalf = (size.Y - threshold);
      break;
    }
    case EAxis::Z:
    {
      float threshold = (size.Z / 100.0f) * splitCorrection.Z * 100.0f;
      if (randomHalf < threshold) randomHalf = threshold;
      if (randomHalf > (size.Z - threshold)) randomHalf = (size.Z - threshold);
      break;
    }
  }
  // Compute new split locations
  FVector leftLocation{ location };
  FVector rightLocation{ location };
  switch (node->SplitAxis)
  {
    case EAxis::X: rightLocation.X += randomHalf; break;
    case EAxis::Y: rightLocation.Y += randomHalf; break;
    case EAxis::Z: rightLocation.Z += randomHalf; break;
  }
  // Compute new split sizes
  FVector leftSize{ size };
  FVector rightSize{ size };
  switch (node->SplitAxis)
  {
    case EAxis::X: leftSize.X = randomHalf; rightSize.X = (rightSize.X - randomHalf); break;
    case EAxis::Y: leftSize.Y = randomHalf; rightSize.Y = (rightSize.Y - randomHalf); break;
    case EAxis::Z: leftSize.Z = randomHalf; rightSize.Z = (rightSize.Z - randomHalf); break;
  }
  // Continue until depth is reached
  Create(node, node->LeafLeft, depth - 1, minRoomArea, ++id, leftLocation, leftSize, splitCorrection, selectionPercentage);
  Create(node, node->LeafRight, depth - 1, minRoomArea, ++id, rightLocation, rightSize, splitCorrection, selectionPercentage);
}

void FTorchBSP::Destroy(
  FBSPNode*& node)
{
  if (node)
  {
    if (node->LeafLeft && node->LeafRight)
    {
      Destroy(node->LeafLeft);
      Destroy(node->LeafRight);
    }
    FMemory::Free(node);
    node = nullptr;
  }
}