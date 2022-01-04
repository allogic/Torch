#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TorchDungeonGenerator.generated.h"

class UStaticMesh;
class UInstancedStaticMeshComponent;
class UTexture2D;

class ATorchHUD;
class ATorchAICharacter;
class ATorchScaredLight;

struct FBSPNode;

namespace EDirection
{
  enum Type : int32
  {
    North,
    South,
    West,
    East,
  };
}
namespace EBlockType
{
  enum Type : int32
  {
    Air,
    Ground,
    Wall,
    WallCorner,
  };
}

struct FCell
{
  EBlockType::Type BlockType;
  bool IsSolid;
  FVector Location;
  FRotator Rotation;
};
struct FTileInstance
{
  UStaticMesh* mStaticMesh;
  UInstancedStaticMeshComponent* mInstancedStaticMesh;

  void ClearInstances();
  void Initialize(UObject* object, UStaticMesh* staticMesh, const FString& tileName);
  void AddRandomInstance(const FVector& location, const FRotator& rotation, const FVector& scale);
};

UCLASS()
class TORCH_API ATorchDungeonGenerator : public AActor
{
  GENERATED_BODY()

public:

  /*
  * Generator settings
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchGenerator)
  int32 mWidth = 128;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchGenerator)
  int32 mHeight = 128;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchGenerator)
  FVector mTileScale = FVector{ 100.0f, 100.0f, 1.0f };

  /*
  * Random walker
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchRandom)
  int32 mRandomWalkIterations = 1024;

  /*
  * BSP Tree
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchBSP)
  int32 mBSPTreeMinArea = 256;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchBSP)
  FVector mBSPTreeSplitCorrection = FVector{ 0.2f, 0.2f, 0.2f };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchBSP)
  int32 mBSPTreeRoomBorder = 3;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchBSP)
  float mBSPTreeRoomAmount = 0.2f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchBSP)
  int32 mBSPTreeDepth = 10;

  /*
  * Light spawner
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchLightSpawner)
  TSubclassOf<ATorchScaredLight> mDefaultLight;

  /*
  * Mob spawner
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMobSpawner)
  int32 mMobCount = 3;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMobSpawner)
  float mRoomSelectionChance = 1.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchMobSpawner)
  TSubclassOf<ATorchAICharacter> mDefaultMob;

public:

  ATorchDungeonGenerator();

protected:

  virtual void BeginPlay() override;
  virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;
  virtual void Tick(float deltaTime) override;

private:

  /*
  * Dungeon generator
  */

  TArray<FCell> mPrevCells = {};
  TArray<FCell> mCurrCells = {};
  FBSPNode* mBSPTree = nullptr;
  TArray<FBSPNode*> mLightSpawnRooms = {};
  TArray<FBSPNode*> mMobSpawnRooms = {};
  TArray<FBSPNode*> mPlayerSpawnRooms = {};
  TMap<int32, FColor> mColors = {};

  void GenerateDungeon();

  void RandomWalkToBlockType(int32 iterations);
  void ChildRoomsToBlockType(FBSPNode* node);
  void HierarchicalRoomConnectionsToBlockType(FBSPNode* node);

  int32 GetRandomNonSolidCellIndex();
  bool CompareMask(const FVector& location, const TArray<int32>& mask);

  void DetectWalls();
  void DetectCorners();

private:
  
  /*
  * Light spawner
  */

  void SpawnLights(const TArray<FBSPNode*>& rooms);

private:

  /*
  * Mob spawner
  */

  void SpawnMobs(const TArray<FBSPNode*>& rooms);

private:

  /*
  * Spawn player
  */

  void SpawnPlayer(const TArray<FBSPNode*>& rooms);

private:

  /*
  * Tile instances
  */

  FTileInstance mTileInstanceGround1 = {};
  FTileInstance mTileInstanceWall1 = {};
  FTileInstance mTileInstanceWallCorner1 = {};
  
  void ClearAllInstances();

  void BuildAllInstances();
  void BuildAllCandles();

private:

  /*
  * Debugging
  */

  ATorchHUD* mHud = nullptr;
  TArray<uint8> mColorBuffer = {};
  UTexture2D* mDebugTexture = nullptr;

  void SetColor(int32 x, int32 y, const FColor& color);
  void FillColor(FVector location, FVector size, const FColor& color);

  void ConvertBlockTypeToColor();
};