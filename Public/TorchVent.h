#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TorchVent.generated.h"

class UStaticMeshComponent;
class UInstancedStaticMeshComponent;
class URectLightComponent;
class UNiagaraComponent;
class UAudioComponent;
class USoundWave;

UCLASS()
class TORCH_API ATorchVent : public AActor
{
  GENERATED_BODY()

public:

  /*
  * Components
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  UStaticMeshComponent* mVentComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  UInstancedStaticMeshComponent* mInstancedVentBladeComponents = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  UInstancedStaticMeshComponent* mInstancedHeatPlaneComponents = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  URectLightComponent* mRectLightComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  UNiagaraComponent* mHeatParticleComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  UAudioComponent* mAudioOpenCloseComponent = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchComponent)
  UAudioComponent* mAudioLoopComponent = nullptr;

  /*
  * Vent blades
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchVentBlades)
  int32 mNumVentBlades = 18;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchVentBlades)
  FVector mVentBladeBaseOffset = FVector{ -30.0f, 150.0f, 0.0f };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchVentBlades)
  FVector mVentBladeOffset = FVector{ 0.0f, 20.0f, 0.0f };
  
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchVentBlades)
  FRotator mVentBladeRotation = FRotator{ 0.0f, 0.0f, 0.0f };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchVentBlades)
  float mFlipOpenMinSeconds = 5.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchVentBlades)
  float mFlipOpenMaxSeconds = 15.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchVentBlades)
  float mFlipCloseMinSeconds = 2.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchVentBlades)
  float mFlipCloseMaxSeconds = 5.0f;

  /*
  * Vent light
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchVentLight)
  FVector mLightPosition = FVector{ -285.0f, 308.0f, 0.0f };

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchVentLight)
  FLinearColor mLightColor = FLinearColor{ 1.0f, 0.0f, 0.0f, 1.0f };  

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchVentLight)
  float mLightWidth = 260.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchVentLight)
  float mLightHeight = 210.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchVentLight)
  float mLightIntensity = 30.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchVentLight)
  float mLightScatterIntensity = 1000.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchVentLight)
  float mLightAttenuationRadius = 1000.0f;

  /*
  * FX
  */

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchReferences)
  TObjectPtr<class UMaterial> mHeatPlaneMaterial;

public:

		ATorchVent();

protected:

 virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

private:

  /*
  * Escaust timer
  */

  bool mOpen = false;

  FTimerHandle mTimerHandleFlipExcaust = {};
  FTimerDelegate mTimerDelegateFlipExcaust = {};

  UFUNCTION()
  void FlipExcaust();

private:

  /*
  * Audio files
  */

  USoundWave* mWaveVentOpen = nullptr;
  USoundWave* mWaveVentLoop = nullptr;
  USoundWave* mWaveVentClose = nullptr;
};