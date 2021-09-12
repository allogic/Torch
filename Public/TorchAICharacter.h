#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TorchAICharacter.generated.h"

class USkeletalMeshComponent;
class UAnimInstance;
class USceneComponent;
class USphereComponent;
class ATorchAIController;

UCLASS()
class TORCH_API ATorchAICharacter : public ACharacter
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchDebug)
  bool mDrawDebugGizmo = true;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPerception)
  USceneComponent* mHeadLocation;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TorchPerception)
  USphereComponent* mPerceptionSphere;

  ATorchAICharacter();

protected:
  virtual void BeginPlay() override;
  virtual void Tick(float deltaTime) override;

  UFUNCTION()
  void OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
  UFUNCTION()
  void OnEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

private:
  TArray<AActor*> mPerceivingActors;
  UAnimInstance* mAnimationInstance;

  void SyncAnimation();
  void DrawDebugGizmo();
};