#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TorchBigDoor.generated.h"

class UStaticMeshComponent;
class UCurveFloat;
class USoundCue;
class UAudioComponent;
class UBoxComponent;
class USpotLightComponent;

UCLASS()
class TORCH_API ATorchBigDoor : public AActor
{
  GENERATED_BODY()

public:
  
  /*
  * Sub components
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  UStaticMeshComponent* mDoorBodyComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  UStaticMeshComponent* mDoorLeftComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  UStaticMeshComponent* mDoorRightComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  UBoxComponent* mBoxComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  UAudioComponent* mDoorLeftSoundComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  UAudioComponent* mDoorRightSoundComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  USpotLightComponent* mDoorLeftFrontAlarmSpotLightComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  USpotLightComponent* mDoorRightFrontAlarmSpotLightComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  USpotLightComponent* mDoorLeftBackAlarmSpotLightComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  USpotLightComponent* mDoorRightBackAlarmSpotLightComponent = nullptr;

  /*
  * Gate system
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchGate)
  FVector mGateLeftOffset = FVector{ -105.0f, 15.0f, 0.0f };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchGate)
  FVector mGateRightOffset = FVector{ 105.0f, 15.0f, 0.0f };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchGate)
  float mGateSpeedIntensity = 1.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchGate)
  float mGateSpotLightRotationIntensity = 1.0f;

public:

  ATorchBigDoor();

protected:

  virtual void BeginPlay() override;
  virtual void Tick(float deltaTime) override;

private:

  /*
  * Gate system
  */

  UCurveFloat* mGateCurveOpenClose = nullptr;

  USoundCue* mGateCueLeft = nullptr;
  USoundCue* mGateCueRight = nullptr;

  bool mOpen = false;
  bool mOverlap = false;
  bool mTransition = false;

  UFUNCTION()
  void OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
  UFUNCTION()
  void OnEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);
};