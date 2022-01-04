#include "TorchDungeonGenerator.h"
#include "TorchHUD.h"
#include "TorchAICharacter.h"
#include "TorchCore.h"
#include "TorchMath.h"
#include "TorchBSP.h"
#include "TorchScaredLight.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/Texture2D.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"

void FTileInstance::ClearInstances()
{
  mInstancedStaticMesh->ClearInstances();
}
void FTileInstance::Initialize(UObject* object, UStaticMesh* staticMesh, const FString& tileName)
{
  if (object && staticMesh)
  {
    mStaticMesh = staticMesh;
    if (!mInstancedStaticMesh)
    {
      mInstancedStaticMesh = object->CreateDefaultSubobject<UInstancedStaticMeshComponent>(&tileName[0]);
    }
    mInstancedStaticMesh->SetStaticMesh(staticMesh);
    mInstancedStaticMesh->SetMaterial(0, staticMesh->GetMaterial(0));
  }
}
void FTileInstance::AddRandomInstance(const FVector& location, const FRotator& rotation, const FVector& scale)
{
  mInstancedStaticMesh->AddInstance(FTransform{ rotation, location, scale });
}

ATorchDungeonGenerator::ATorchDungeonGenerator()
{
  // Setup actor
  PrimaryActorTick.bCanEverTick = true;

  // Setup tile instanced
  static ConstructorHelpers::FObjectFinder<UStaticMesh> ground1Mesh(TEXT("StaticMesh'/Game/Build/Environment/Meshes/Ground1.Ground1'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> wall1Mesh(TEXT("StaticMesh'/Game/Build/Environment/Meshes/Wall1.Wall1'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> wallCorner1Mesh(TEXT("StaticMesh'/Game/Build/Environment/Meshes/WallCorner1.WallCorner1'"));

  if (ground1Mesh.Succeeded()) mTileInstanceGround1.Initialize(this, ground1Mesh.Object, "Ground1");
  if (wall1Mesh.Succeeded()) mTileInstanceWall1.Initialize(this, wall1Mesh.Object, "Wall1");
  if (wallCorner1Mesh.Succeeded()) mTileInstanceWallCorner1.Initialize(this, wallCorner1Mesh.Object, "WallCorner1");
}

void ATorchDungeonGenerator::BeginPlay()
{
  Super::BeginPlay();

  // Get HUD
  mHud = Cast<ATorchHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

  // Setup pixel buffer
  for (int32 i = 0; i < mWidth * mHeight * 4; ++i)
  {
    mColorBuffer.Emplace(int8{ 0 });
  }
  
  // Setup cells
  for (int32 x = 0; x < mWidth; ++x)
  {
    for (int32 y = 0; y < mHeight; ++y)
    {
      mCurrCells.Emplace(FCell{ EBlockType::Air, false, FVector{ (float)x, (float)y, 0.0f }, FRotator{} });
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

  // Query rooms which can spawn things
  FTorchBSP::QueryChildren(mBSPTree, mLightSpawnRooms);
  FTorchBSP::QueryChildren(mBSPTree, mMobSpawnRooms);
  FTorchBSP::QueryChildren(mBSPTree, mPlayerSpawnRooms);

  // Copy room ids
  ChildRoomsToBlockType(mBSPTree);

  // Copy random path between rooms
  //RandomWalkToBlockType(mRandomWalkIterations);

  // Copy center connections
  HierarchicalRoomConnectionsToBlockType(mBSPTree);

  // Sequentially detect cells types
  mPrevCells = mCurrCells;
  DetectWalls();
  DetectCorners();

  // Rebuild instanced dungeon meshes
  ClearAllInstances();
  BuildAllInstances();

  // Spawn lights
  SpawnLights(mLightSpawnRooms);

  // Spawn mobs
  SpawnMobs(mMobSpawnRooms);

  // Spawn player
  SpawnPlayer(mPlayerSpawnRooms);

  // Convert cells to colors
  ConvertBlockTypeToColor();
}

void ATorchDungeonGenerator::RandomWalkToBlockType(int32 iterations)
{
  int32 startIndex = GetRandomNonSolidCellIndex();
  if (startIndex >= 0)
  {
    FVector location = mCurrCells[startIndex].Location;
    EDirection::Type direction = (EDirection::Type)(FMath::RandRange(0, 4));
    for (int32 i = 0; i < iterations;)
    {
      // Check current cell
      int32 index = (int32)location.X + (int32)location.Y * mWidth;
      if (!mCurrCells[index].IsSolid)
      {
        mCurrCells[index].BlockType = EBlockType::Ground;
        mCurrCells[index].IsSolid = true;
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
void ATorchDungeonGenerator::ChildRoomsToBlockType(FBSPNode* node)
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
          mCurrCells[index].BlockType = EBlockType::Ground;
          mCurrCells[index].IsSolid = true;
        }
      }
    }
    ChildRoomsToBlockType(node->LeafLeft);
    ChildRoomsToBlockType(node->LeafRight);
  }
}
void ATorchDungeonGenerator::HierarchicalRoomConnectionsToBlockType(FBSPNode* node)
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
            mCurrCells[x + y * mWidth].BlockType = EBlockType::Ground;
            mCurrCells[x + y * mWidth].IsSolid = true;
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
            mCurrCells[x + y * mWidth].BlockType = EBlockType::Ground;
            mCurrCells[x + y * mWidth].IsSolid = true;
          }
          break;
        }
      }
    }
    HierarchicalRoomConnectionsToBlockType(node->LeafLeft);
    HierarchicalRoomConnectionsToBlockType(node->LeafRight);
  }
}

int32 ATorchDungeonGenerator::GetRandomNonSolidCellIndex()
{
  while (true)
  {
    FVector location = FVector{ FMath::RandRange(1.0f, (float)mWidth - 1), FMath::RandRange(1.0f, (float)mHeight - 1), 0.0f };
    int32 index = (int32)location.X + (int32)location.Y * mWidth;
    if (!mCurrCells[index].IsSolid)
    {
      return index;
    }
  }
  return -1;
}
bool ATorchDungeonGenerator::CompareMask(const FVector& location, const TArray<int32>& mask)
{
  if (location.X > 0 && location.X <= mWidth - 2)
  {
    if (location.Y > 0 && location.Y <= mHeight - 2)
    {
      for (int32 x = 0; x <= 2; ++x)
      {
        for (int32 y = 0; y <= 2; ++y)
        {
          int32 indexCells = ((x - 1) + location.X) + ((y - 1) + location.Y) * mWidth;
          int32 indexMask = x + y * 3;
          if (mPrevCells[indexCells].BlockType != (EBlockType::Type)mask[indexMask])
          {
            // Mask does not match surroundings
            return false;
          }
        }
      }
      return true;
    }
  }
  return false;
}

void ATorchDungeonGenerator::DetectWalls()
{
  for (int32 x = 0; x < mWidth; ++x)
  {
    for (int32 y = 0; y < mHeight; ++y)
    {
      FCell& cell = mCurrCells[x + y * mWidth];
      // Front
      {
        if (CompareMask(FVector{ (float)x, (float)y, 0.0f },
          {
            1, 1, 1,
            0, 0, 0,
            0, 0, 0,
          }))
        {
          cell.BlockType = EBlockType::Wall;
          cell.Rotation.Yaw = -90.0f;
        }
        if (CompareMask(FVector{ (float)x, (float)y, 0.0f },
          {
            1, 1, 0,
            0, 0, 0,
            0, 0, 0,
          }))
        {
          cell.BlockType = EBlockType::Wall;
          cell.Rotation.Yaw = -90.0f;
        }
        if (CompareMask(FVector{ (float)x, (float)y, 0.0f },
          {
            0, 1, 1,
            0, 0, 0,
            0, 0, 0,
          }))
        {
          cell.BlockType = EBlockType::Wall;
          cell.Rotation.Yaw = -90.0f;
        }
      }
      // Back
      {
        if (CompareMask(FVector{ (float)x, (float)y, 0.0f },
        {
          0, 0, 0,
          0, 0, 0,
          1, 1, 1,
        }))
        {
          cell.BlockType = EBlockType::Wall;
          cell.Rotation.Yaw = 90.0f;
        }
        if (CompareMask(FVector{ (float)x, (float)y, 0.0f },
        {
          0, 0, 0,
          0, 0, 0,
          1, 1, 0,
        }))
        {
          cell.BlockType = EBlockType::Wall;
          cell.Rotation.Yaw = 90.0f;
        }
        if (CompareMask(FVector{ (float)x, (float)y, 0.0f },
        {
          0, 0, 0,
          0, 0, 0,
          0, 1, 1,
        }))
        {
          cell.BlockType = EBlockType::Wall;
          cell.Rotation.Yaw = 90.0f;
        }
      }
      // Left
      {
        if (CompareMask(FVector{ (float)x, (float)y, 0.0f },
        {
          1, 0, 0,
          1, 0, 0,
          1, 0, 0,
        }))
        {
          cell.BlockType = EBlockType::Wall;
          cell.Rotation.Yaw = 180.0f;
        }
        if (CompareMask(FVector{ (float)x, (float)y, 0.0f },
        {
          1, 0, 0,
          1, 0, 0,
          0, 0, 0,
        }))
        {
          cell.BlockType = EBlockType::Wall;
          cell.Rotation.Yaw = 180.0f;
        }
        if (CompareMask(FVector{ (float)x, (float)y, 0.0f },
        {
          0, 0, 0,
          1, 0, 0,
          1, 0, 0,
        }))
        {
          cell.BlockType = EBlockType::Wall;
          cell.Rotation.Yaw = 180.0f;
        }
      }
      // Right
      {
        if (CompareMask(FVector{ (float)x, (float)y, 0.0f },
        {
          0, 0, 1,
          0, 0, 1,
          0, 0, 1,
        }))
        {
          cell.BlockType = EBlockType::Wall;
          cell.Rotation.Yaw = 0.0f;
        }
        if (CompareMask(FVector{ (float)x, (float)y, 0.0f },
        {
          0, 0, 1,
          0, 0, 1,
          0, 0, 0,
        }))
        {
          cell.BlockType = EBlockType::Wall;
          cell.Rotation.Yaw = 0.0f;
        }
        if (CompareMask(FVector{ (float)x, (float)y, 0.0f },
        {
          0, 0, 0,
          0, 0, 1,
          0, 0, 1,
        }))
        {
          cell.BlockType = EBlockType::Wall;
          cell.Rotation.Yaw = 0.0f;
        }
      }
    }
  }
}
void ATorchDungeonGenerator::DetectCorners()
{
  for (int32 x = 0; x < mWidth; ++x)
  {
    for (int32 y = 0; y < mHeight; ++y)
    {
      FCell& cell = mCurrCells[x + y * mWidth];
      if (CompareMask(FVector{ (float)x, (float)y, 0.0f },
        {
          1, 0, 0,
          0, 0, 0,
          0, 0, 0,
        }))
      {
        cell.BlockType = EBlockType::WallCorner;
        cell.Rotation.Yaw = 180.0f;
      }
      if (CompareMask(FVector{ (float)x, (float)y, 0.0f },
        {
          0, 0, 1,
          0, 0, 0,
          0, 0, 0,
        }))
      {
        cell.BlockType = EBlockType::WallCorner;
        cell.Rotation.Yaw = -90.0f;
      }
      if (CompareMask(FVector{ (float)x, (float)y, 0.0f },
        {
          0, 0, 0,
          0, 0, 0,
          1, 0, 0,
        }))
      {
        cell.BlockType = EBlockType::WallCorner;
        cell.Rotation.Yaw = 90.0f;
      }
      if (CompareMask(FVector{ (float)x, (float)y, 0.0f },
        {
          0, 0, 0,
          0, 0, 0,
          0, 0, 1,
        }))
      {
        cell.BlockType = EBlockType::WallCorner;
        cell.Rotation.Yaw = 0.0f;
      }
    }
  }
}

void ATorchDungeonGenerator::SpawnLights(const TArray<FBSPNode*>& rooms)
{
  if (mDefaultLight)
  {
    for (FBSPNode* room : rooms)
    {
      FVector location{ (room->Location + room->Size / 2.0f) * mTileScale };
      location.Z = -500.0f;
      FLinearColor color{ FMath::RandRange(0.5f, 1.0f), FMath::RandRange(0.5f, 1.0f), FMath::RandRange(0.5f, 1.0f), 1.0f };
      ATorchScaredLight* scaredLight = GetWorld()->SpawnActor<ATorchScaredLight>(location, FRotator{}, FActorSpawnParameters{});
      scaredLight->mFlickerIntensity = 1.0f;
      scaredLight->mLightColor = color;
      scaredLight->mPointLightComponent->SetIntensity(20000.0f);
      scaredLight->mPointLightComponent->SetAttenuationRadius(2500.0f);
      scaredLight->mPointLightComponent->SetTemperature(true);
      scaredLight->mPointLightComponent->SetLightColor(color);
    }
  }
}

void ATorchDungeonGenerator::SpawnMobs(const TArray<FBSPNode*>& rooms)
{
  if (mDefaultMob)
  {
    for (FBSPNode* room : rooms)
    {
      for (int32 i = 0; i < mMobCount; ++i)
      {
        int32 offset = 2;
        int32 x = FMath::RandRange((room->Location.X + offset), (room->Location.X + (room->Size.X - offset)));
        int32 y = FMath::RandRange((room->Location.Y + offset), (room->Location.Y + (room->Size.Y - offset)));
        FVector location = FVector{ (float)x, (float)y, 300.0f } * mTileScale;
        UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), mDefaultMob, nullptr, location, FRotator{});
        //GetWorld()->SpawnActor<ATorchCharacterPuker>(location, FRotator{}, FActorSpawnParameters{});
      }
    }
  }
}

void ATorchDungeonGenerator::SpawnPlayer(const TArray<FBSPNode*>& rooms)
{
  FVector location = FVector{ rooms[0]->Location.X + rooms[0]->Size.X / 2.0f, rooms[0]->Location.Y + rooms[0]->Size.Y / 2.0f, 100.0f } * mTileScale;
  APlayerController* controller = GetWorld()->GetFirstPlayerController();
  if (controller)
  {
    APawn* pawn = controller->GetPawn();
    if (pawn)
    {
      pawn->SetActorLocation(location);
    }
  }
}

void ATorchDungeonGenerator::ClearAllInstances()
{
  mTileInstanceGround1.ClearInstances();
  mTileInstanceWall1.ClearInstances();
  mTileInstanceWallCorner1.ClearInstances();
}

void ATorchDungeonGenerator::BuildAllInstances()
{
  for (int32 x = 0; x < mWidth; ++x)
  {
    for (int32 y = 0; y < mHeight; ++y)
    {
      FCell const& cell = mCurrCells[x + y * mWidth];
      switch (cell.BlockType)
      {
        case EBlockType::Air:
        {
          break;
        }
        case EBlockType::Ground:
        {
          // Add floor tile
          FVector location = FVector{ (float)x, (float)y, 0.0f } * mTileScale;
          FRotator rotation = FRotator{ cell.Rotation.Pitch, cell.Rotation.Yaw, -90.0f };
          FVector scale = FVector{ 1.0f };
          mTileInstanceGround1.AddRandomInstance(location, rotation, scale);
          mTileInstanceGround1.AddRandomInstance(location + FVector{ 0.0f, 0.0f, 800.0f }, rotation, scale);
          break;
        }
        case EBlockType::Wall:
        {
          // Add floor tile
          FVector location = FVector{ (float)x, (float)y, 0.0f } * mTileScale;
          FRotator rotation = FRotator{ cell.Rotation.Pitch, cell.Rotation.Yaw, -90.0f };
          FVector scale = FVector{ 1.0f, 2.0f, 1.0f };
          mTileInstanceWall1.AddRandomInstance(location, rotation, scale);
          break;
        }
        case EBlockType::WallCorner:
        {
          // Add floor tile
          FVector location = FVector{ (float)x, (float)y, 0.0f } * mTileScale;
          FRotator rotation = FRotator{ cell.Rotation.Pitch, cell.Rotation.Yaw, -90.0f };
          FVector scale = FVector{ 1.0f, 2.0f, 1.0f };
          mTileInstanceWallCorner1.AddRandomInstance(location, rotation, scale);
          break;
        }
      }
    }
  }

  // Rebuild navigation mesh
  UNavigationSystemV1* navSys = UNavigationSystemV1::GetCurrent(GetWorld());
  if (navSys)
  {
    navSys->Build();
  }
}
void ATorchDungeonGenerator::BuildAllCandles()
{

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

void ATorchDungeonGenerator::ConvertBlockTypeToColor()
{
  for (int32 x = 0; x < mWidth; ++x)
  {
    for (int32 y = 0; y < mHeight; ++y)
    {
      FCell const& cell = mCurrCells[x + y * mWidth];
      switch (cell.BlockType)
      {
        case EBlockType::Air:
        {
          SetColor(x, y, FColor::Black);
          break;
        }
        case EBlockType::Ground:
        {
          SetColor(x, y, FColor::Green);
          break;
        }
        case EBlockType::Wall:
        {
          SetColor(x, y, FColor::Purple);
          break;
        }
        case EBlockType::WallCorner:
        {
          SetColor(x, y, FColor::Orange);
          break;
        }
      }
    }
  }
}