#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TorchDungeonGenerator.generated.h"

class UStaticMesh;
class UInstancedStaticMeshComponent;
class UTexture2D;
class ATorchHUD;

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

struct FCell
{
  int32 BlockID;
  bool IsSolid;
  FVector Location;
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

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchBuilder)
  FVector mTileSpacing = FVector{ 100.0f, 100.0f, 1.0f };

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
  * Dungeon builder
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchBuilder)
  TObjectPtr<UStaticMesh> mFloorMesh;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchBuilder)
  TObjectPtr<UStaticMesh> mWallMesh;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchBuilder)
  TObjectPtr<UMaterial> mFloorMaterial;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchBuilder)
  TObjectPtr<UMaterial> mWallMaterial;

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

  TArray<FCell> mGrid = {};
  FBSPNode* mBSPTree = nullptr;
  TMap<int32, FColor> mColors = {};

  void GenerateDungeon();

  void CopyRandomWalkIDToBlockID(int32 iterations);
  void CopyChildRoomIDToBlockID(FBSPNode* node);
  void CopyHierarchicalRoomConnectionsToBlockID(FBSPNode* node);

  int32 GetRandomNonSolidCellIndex();

private:

  /*
  * Mesh instancing
  */

  UInstancedStaticMeshComponent* mInstancedFloorMesh = nullptr;
  UInstancedStaticMeshComponent* mInstancedWallMesh = nullptr;

  void RebuildMesh();
  void BuildLayer(const FVector& location, const FVector& size, const FVector& layerScale = FVector{ 0.0f });

  /*
  * Debugging
  */

  ATorchHUD* mHud = nullptr;
  TArray<uint8> mColorBuffer = {};
  UTexture2D* mDebugTexture = nullptr;

  void SetColor(int32 x, int32 y, const FColor& color);
  void FillColor(FVector location, FVector size, const FColor& color);

  void ConvertBlockIDToColor();
};