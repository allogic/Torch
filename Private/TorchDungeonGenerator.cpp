#include "TorchDungeonGenerator.h"
#include "TorchHUD.h"
#include "TorchBSP.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/Texture2D.h"

ATorchDungeonGenerator::ATorchDungeonGenerator()
{
  // Setup actor
  PrimaryActorTick.bCanEverTick = true;

  // Setup instanced floor mesh
  mInstancedFloorMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Floor");
  mInstancedFloorMesh->SetupAttachment(RootComponent);

  // Setup instanced wall mesh
  mInstancedWallMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>("Wall");
  mInstancedWallMesh->SetupAttachment(RootComponent);
}

void ATorchDungeonGenerator::BeginPlay()
{
  Super::BeginPlay();

  // Setup instanced floor mesh
  if (mFloorMesh)
  {
    if (mFloorMaterial)
    {
      mFloorMesh->SetMaterial(0, mFloorMaterial);
    }
    mInstancedFloorMesh->SetStaticMesh(mFloorMesh);
  }

  // Setup instanced wall mesh
  if (mWallMesh)
  {
    if (mWallMaterial)
    {
      mWallMesh->SetMaterial(0, mWallMaterial);
    }
    mInstancedWallMesh->SetStaticMesh(mWallMesh);
  }

  // Get HUD
  mHud = Cast<ATorchHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

  // Setup pixel buffer
  for (int32 i = 0; i < mWidth * mHeight * 4; ++i)
  {
    mColorBuffer.Emplace(int8{ 0 });
  }

  // Setup grid cells
  for (int32 x = 0; x < mWidth; ++x)
  {
    for (int32 y = 0; y < mHeight; ++y)
    {
      mGrid.Emplace(FCell{ 0, false, FVector{ (float)x, (float)y, 0.0f } });
    }
  }

  // Setup default colors
  mColors.Emplace(0, FColor::Black);
  mColors.Emplace(-1, FColor::Silver);

  // Generate a new dungeon
  GenerateDungeon();

  // Debug output
  //mHud->SetDebugTexture(mColorBuffer.GetData(), mWidth, mHeight);
}
void ATorchDungeonGenerator::EndPlay(const EEndPlayReason::Type endPlayReason)
{
  Super::EndPlay(endPlayReason);

  // Destroy KD tree
  FTorchBSP::Destroy(mBSPTree);
}
void ATorchDungeonGenerator::Tick(float deltaTime)
{
  Super::Tick(deltaTime);
}

void ATorchDungeonGenerator::GenerateDungeon()
{
  // Rebuild KD tree
  FTorchBSP::Destroy(mBSPTree);
  FTorchBSP::Create(mBSPTree, mBSPTreeDepth, mBSPTreeMinArea, FVector{ 0.0f }, FVector{ (float)mWidth, (float)mHeight, 1.0f }, mBSPTreeSplitCorrection, mBSPTreeRoomAmount);

  // Copy room ids
  CopyChildRoomIDToBlockID(mBSPTree);

  // Copy random path between rooms
  //CopyRandomWalkIDToBlockID(mRandomWalkIterations);

  // Copy center connections
  CopyHierarchicalRoomConnectionsToBlockID(mBSPTree);

  // Create instanced dungeon mesh
  RebuildMesh();

  // Convert grid to colors
  ConvertBlockIDToColor();
}

void ATorchDungeonGenerator::CopyRandomWalkIDToBlockID(int32 iterations)
{
  int32 startIndex = GetRandomNonSolidCellIndex();
  if (startIndex >= 0)
  {
    FVector location = mGrid[startIndex].Location;
    EDirection::Type direction = (EDirection::Type)(FMath::RandRange(0, 4));
    for (int32 i = 0; i < iterations;)
    {
      // Check current cell
      int32 index = (int32)location.X + (int32)location.Y * mWidth;
      if (!mGrid[index].IsSolid)
      {
        mGrid[index].BlockID = -1;
        mGrid[index].IsSolid = true;
        ++i;
      }
      // Randomly switch direction
      if (FMath::FRandRange(0.0f, 1.0f) > 0.5f)
      {
        direction = (EDirection::Type)(FMath::RandRange(0, 4));
      }
      // Walk along direction
      switch (direction)
      {
        case EDirection::North: location.Y += 1; break;
        case EDirection::South: location.Y -= 1; break;
        case EDirection::West: location.X += 1; break;
        case EDirection::East: location.X -= 1; break;
      }
      // Keep walker in bounds
      if (location.X <= 1) location.X = 1;
      if (location.Y <= 1) location.Y = 1;
      if (location.X >= mWidth - 2) location.X = mWidth - 2;
      if (location.Y >= mHeight - 2) location.Y = mHeight - 2;
    }
  }
}
void ATorchDungeonGenerator::CopyChildRoomIDToBlockID(FBSPNode* node)
{
  if (node)
  {
    if (node->IsEnd)
    {
      FVector location = FVector{ node->Location.X + mBSPTreeRoomBorder, node->Location.Y + mBSPTreeRoomBorder, 0 };
      FVector size = FVector{ node->Size.X - (mBSPTreeRoomBorder * 2), node->Size.Y - (mBSPTreeRoomBorder * 2), 1 };
      for (int32 x = 0; x < (int32)size.X; ++x)
      {
        for (int32 y = 0; y < (int32)size.Y; ++y)
        {
          int32 index = ((int32)location.X + x) + ((int32)location.Y + y) * mWidth;
          mGrid[index].BlockID = node->ID;
          mGrid[index].IsSolid = true;
        }
      }
    }
    CopyChildRoomIDToBlockID(node->LeafLeft);
    CopyChildRoomIDToBlockID(node->LeafRight);
  }
}
void ATorchDungeonGenerator::CopyHierarchicalRoomConnectionsToBlockID(FBSPNode* node)
{
  if (node)
  {
    if (node->Parent)
    {
      switch (node->Parent->SplitAxis)
      {
        case EAxis::X:
        {
          float roomXCenter = node->Location.X + FMath::Floor(node->Size.X / 2.0f);
          float parentXCenter = node->Parent->Location.X + FMath::Floor(node->Parent->Size.X / 2.0f);
          float minX = FMath::Min(roomXCenter, parentXCenter);
          float maxX = FMath::Max(roomXCenter, parentXCenter);
          float y = node->Parent->Location.Y + FMath::Floor(node->Parent->Size.Y / 2.0f);
          for (int32 x = (int32)minX; x < (int32)maxX; ++x)
          {
            mGrid[x + y * mWidth].BlockID = -2;
            mGrid[x + y * mWidth].IsSolid = true;
          }
          break;
        }
        case EAxis::Y:
        {
          float roomYCenter = node->Location.Y + FMath::Floor(node->Size.Y / 2.0f);
          float parentYCenter = node->Parent->Location.Y + FMath::Floor(node->Parent->Size.Y / 2.0f);
          float minY = FMath::Min(roomYCenter, parentYCenter);
          float maxY = FMath::Max(roomYCenter, parentYCenter);
          float x = node->Parent->Location.X + FMath::Floor(node->Parent->Size.X / 2.0f);
          for (int32 y = (int32)minY; y < (int32)maxY; ++y)
          {
            mGrid[x + y * mWidth].BlockID = -2;
            mGrid[x + y * mWidth].IsSolid = true;
          }
          break;
        }
      }
    }
    CopyHierarchicalRoomConnectionsToBlockID(node->LeafLeft);
    CopyHierarchicalRoomConnectionsToBlockID(node->LeafRight);
  }
}

int32 ATorchDungeonGenerator::GetRandomNonSolidCellIndex()
{
  while (true)
  {
    FVector location = FVector{ FMath::RandRange(1.0f, (float)mWidth - 1), FMath::RandRange(1.0f, (float)mHeight - 1), 0.0f };
    int32 index = (int32)location.X + (int32)location.Y * mWidth;
    if (!mGrid[index].IsSolid)
    {
      return index;
    }
  }
  return -1;
}

void ATorchDungeonGenerator::RebuildMesh()
{
  // Clear instances
  mInstancedFloorMesh->ClearInstances();
  mInstancedWallMesh->ClearInstances();

  // Create floor
  //for (int32 x = 0; x < mWidth; ++x)
  //{
  //  for (int32 y = 0; y < mHeight; ++y)
  //  {
  //    if (mGrid[x + y * mWidth].IsSolid)
  //    {
  //      FVector location = FVector{ (float)x, (float)y, 50.0f } * mTileSpacing;
  //      FRotator rotation = FRotator{ 0.0f };
  //      FVector scale = FVector{ 1.0f };
  //      mInstancedFloorMesh->AddInstance(FTransform{ rotation, location, scale });
  //    }
  //  }
  //}

  FVector roomLocation{ 1000.0f, 1000.0f, 0.0f };
  FVector roomSize{ 20.0f, 20.0f, 1.0f };

  BuildLayer(roomLocation, roomSize, FVector{ 0.0f, 0.0f, 0.0f });
  BuildLayer(roomLocation, roomSize, FVector{ 300.0f, 300.0f, 0.0f });
  BuildLayer(roomLocation, roomSize, FVector{ 600.0f, 600.0f, 0.0f });
}
void ATorchDungeonGenerator::BuildLayer(const FVector& location, const FVector& size, const FVector& layerScale)
{
  for (int32 x = 0; x < (float)size.X; ++x)
  {
    for (float i = 0; i < 1.0f; i += FMath::RandRange(0.1f, 0.5f))
    {
      // Front
        mInstancedWallMesh->AddInstance(FTransform{
            FRotator{ 0.0f, FMath::RandRange(-180.0f, 180.0f), 90.0f },
            location + FVector{ (float)x + i, 0.0f, FMath::RandRange(50.0f, 200.0f) } * mTileSpacing - layerScale,
            FVector{ FMath::RandRange(1.0f, 3.0f) },
          });
      // Back
        mInstancedWallMesh->AddInstance(FTransform{
            FRotator{ 0.0f, FMath::RandRange(-180.0f, 180.0f), 90.0f },
            location + FVector{ (float)x + i, (float)size.Y, FMath::RandRange(50.0f, 200.0f) } * mTileSpacing - layerScale,
            FVector{ FMath::RandRange(1.0f, 3.0f) },
          });
    }
  }
  for (int32 y = 0; y < (float)size.Y; ++y)
  {
    for (float i = 0; i < 1.0f; i += FMath::RandRange(0.1f, 0.5f))
    {
      // Left
      mInstancedWallMesh->AddInstance(FTransform{
          FRotator{ 0.0f, FMath::RandRange(-180.0f, 180.0f), 90.0f },
          location + FVector{ 0.0f, (float)y + i, FMath::RandRange(50.0f, 200.0f) } * mTileSpacing - layerScale,
          FVector{ FMath::RandRange(1.0f, 3.0f) },
        });
      // Right
      mInstancedWallMesh->AddInstance(FTransform{
          FRotator{ 0.0f, FMath::RandRange(-180.0f, 180.0f), 90.0f },
          location + FVector{ (float)size.X, (float)y + i, FMath::RandRange(50.0f, 200.0f) } * mTileSpacing - layerScale,
          FVector{ FMath::RandRange(1.0f, 3.0f) },
        });
    }
  }
}

void ATorchDungeonGenerator::SetColor(int32 x, int32 y, const FColor& color)
{
  int32 index = (x + y * mWidth) * 4;
  mColorBuffer[index + 0] = color.B;
  mColorBuffer[index + 1] = color.G;
  mColorBuffer[index + 2] = color.R;
  mColorBuffer[index + 3] = color.A;
}
void ATorchDungeonGenerator::FillColor(FVector location, FVector size, const FColor& color)
{
  for (int32 x = 0; x < (int32)size.X; ++x)
  {
    for (int32 y = 0; y < (int32)size.Y; ++y)
    {
      int32 index = (((int32)location.X + x) + ((int32)location.Y + y) * mWidth) * 4;
      mColorBuffer[index + 0] = color.B;
      mColorBuffer[index + 1] = color.G;
      mColorBuffer[index + 2] = color.R;
      mColorBuffer[index + 3] = color.A;
    }
  }
}

void ATorchDungeonGenerator::ConvertBlockIDToColor()
{
  FillColor(FVector{ 0.0f }, FVector{ (float)mWidth, (float)mHeight, 1.0f }, FColor::Black);
  for (int32 x = 0; x < mWidth; ++x)
  {
    for (int32 y = 0; y < mHeight; ++y)
    {
      if (mGrid[x + y * mWidth].IsSolid)
      {
        SetColor(x, y, FColor::White);
      }
      //int32 blockID = mGrid[x + y * mWidth].BlockID;
      //FColor* color = mColors.Find(blockID);
      //if (color)
      //{
      //  SetColor(x, y, *color);
      //}
      //else
      //{
      //  SetColor(x, y, mColors.Emplace(blockID, FColor::MakeRandomColor()));
      //}
    }
  }
}