#include "TorchBigDoor.h"
#include "TorchCore.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Curves/CurveFloat.h"
#include "Components/BoxComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SpotLightComponent.h"

ATorchBigDoor::ATorchBigDoor()
{
  // Setup root
  PrimaryActorTick.bCanEverTick = true;
  RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

  // Setup box collider
  mBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
  mBoxComponent->SetupAttachment(RootComponent);
  mBoxComponent->SetRelativeTransform(FTransform{ FRotator{}, FVector{ 0.0f, 0.0f, 100.0f }, FVector{ 7.0f, 10.0f, 3.5f } });
  mBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATorchBigDoor::OnBeginOverlap);
  mBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ATorchBigDoor::OnEndOverlap);
  mBoxComponent->SetCollisionProfileName(TEXT("Custom"));
  mBoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  mBoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

  // Setup static meshes
  mDoorBodyComponent = CreateDefaultSubobject<UStaticMeshComponent>("DoorBody");
  mDoorLeftComponent = CreateDefaultSubobject<UStaticMeshComponent>("DoorLeft");
  mDoorRightComponent = CreateDefaultSubobject<UStaticMeshComponent>("DoorRight");

  mDoorLeftComponent->AddRelativeLocation(mGateLeftOffset);
  mDoorRightComponent->AddRelativeLocation(mGateRightOffset);

  mDoorLeftComponent->SetRelativeScale3D(FVector{ 1.0f, 2.0f, 1.0f });
  mDoorRightComponent->SetRelativeScale3D(FVector{ 1.0f, 2.0f, 1.0f });

  mDoorBodyComponent->SetupAttachment(RootComponent);
  mDoorLeftComponent->SetupAttachment(RootComponent);
  mDoorRightComponent->SetupAttachment(RootComponent);

  static ConstructorHelpers::FObjectFinder<UStaticMesh> doorBodyMesh(TEXT("StaticMesh'/Game/Sci_Fi_City/Meshes/S_door_2_C_low.S_door_2_C_low'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> doorMesh(TEXT("StaticMesh'/Game/Sci_Fi_City/Meshes/S_door_2_2_C_low.S_door_2_2_C_low'"));

  if (doorBodyMesh.Succeeded()) mDoorBodyComponent->SetStaticMesh(doorBodyMesh.Object);
  if (doorMesh.Succeeded()) mDoorLeftComponent->SetStaticMesh(doorMesh.Object);
  if (doorMesh.Succeeded()) mDoorRightComponent->SetStaticMesh(doorMesh.Object);

  // Setup animation curves
  static ConstructorHelpers::FObjectFinder<UCurveFloat> floatCurveGateOpenClose(TEXT("CurveFloat'/Game/Torch/Gates/BigDoor/CF_TorchBigDoorOpenClose.CF_TorchBigDoorOpenClose'"));

  static ConstructorHelpers::FObjectFinder<USoundCue> soundCueGateLeft(TEXT("SoundCue'/Game/Torch/Gates/BigDoor/SC_TorchBigDoorLeft.SC_TorchBigDoorLeft'"));
  static ConstructorHelpers::FObjectFinder<USoundCue> soundCueGateRight(TEXT("SoundCue'/Game/Torch/Gates/BigDoor/SC_TorchBigDoorRight.SC_TorchBigDoorRight'"));
  
  if (floatCurveGateOpenClose.Succeeded()) mGateCurveOpenClose = floatCurveGateOpenClose.Object;

  if (soundCueGateLeft.Succeeded()) mGateCueLeft = soundCueGateLeft.Object;
  if (soundCueGateRight.Succeeded()) mGateCueRight = soundCueGateRight.Object;

  // Setup audio sources
  mDoorLeftSoundComponent = CreateDefaultSubobject<UAudioComponent>("SoundEmitterLeft");
  mDoorRightSoundComponent = CreateDefaultSubobject<UAudioComponent>("SoundEmitterRight");

  mDoorLeftSoundComponent->SetupAttachment(mDoorLeftComponent);
  mDoorRightSoundComponent->SetupAttachment(mDoorRightComponent);

  // Setup alaram spot lights
  mDoorLeftFrontAlarmSpotLightComponent = CreateDefaultSubobject<USpotLightComponent>("AlarmSpotLightLeftFront");
  mDoorRightFrontAlarmSpotLightComponent = CreateDefaultSubobject<USpotLightComponent>("AlarmSpotLightRightFront");
  mDoorLeftBackAlarmSpotLightComponent = CreateDefaultSubobject<USpotLightComponent>("AlarmSpotLightLeftBack");
  mDoorRightBackAlarmSpotLightComponent = CreateDefaultSubobject<USpotLightComponent>("AlarmSpotLightRightBack");

  mDoorLeftFrontAlarmSpotLightComponent->SetupAttachment(RootComponent);
  mDoorRightFrontAlarmSpotLightComponent->SetupAttachment(RootComponent);
  mDoorLeftBackAlarmSpotLightComponent->SetupAttachment(RootComponent);
  mDoorRightBackAlarmSpotLightComponent->SetupAttachment(RootComponent);
}

void ATorchBigDoor::BeginPlay()
{
  Super::BeginPlay();
}
void ATorchBigDoor::Tick(float deltaTime)
{
  Super::Tick(deltaTime);

  static float transitionTime = 0.0f;
  static float overlapEndDecay = 0.0f;

  if (mTransition)
  {
    // Move doors
    float offset = mGateCurveOpenClose->GetFloatValue(mOpen ? transitionTime : (1.0f - transitionTime)) * GetActorScale().X;

    FVector directionOffsetLeft = GetActorForwardVector() * offset;
    FVector directionOffsetRight = -GetActorForwardVector() * offset;

    FVector leftOffset = mOpen ? -directionOffsetLeft : directionOffsetRight;
    FVector rightOffset = mOpen ? -directionOffsetRight : directionOffsetLeft;

    mDoorLeftComponent->SetRelativeLocation(mGateLeftOffset + leftOffset);
    mDoorRightComponent->SetRelativeLocation(mGateRightOffset + rightOffset);

    // Spin spot lights
    mDoorLeftFrontAlarmSpotLightComponent->AddRelativeRotation(FQuat{ FVector::UpVector, mGateSpotLightRotationIntensity * deltaTime });
    mDoorRightFrontAlarmSpotLightComponent->AddRelativeRotation(FQuat{ -FVector::UpVector, mGateSpotLightRotationIntensity * deltaTime });
    mDoorLeftBackAlarmSpotLightComponent->AddRelativeRotation(FQuat{ FVector::UpVector, mGateSpotLightRotationIntensity * deltaTime });
    mDoorRightBackAlarmSpotLightComponent->AddRelativeRotation(FQuat{ -FVector::UpVector, mGateSpotLightRotationIntensity * deltaTime });

    // Fade in/out
    transitionTime += mGateSpeedIntensity;
    if (transitionTime > 1.0f)
    {
      transitionTime = 0.0f;

      mTransition = false;
    }
  }
  else
  {
    if (mOverlap)
    {
      if (!mOpen)
      {
        mTransition = true;
        mOpen = true;

        mDoorLeftSoundComponent->SetSound(mGateCueLeft);
        mDoorRightSoundComponent->SetSound(mGateCueRight);
        mDoorLeftSoundComponent->Play();
        mDoorRightSoundComponent->Play();
      }
    }
    else
    {
      if (mOpen)
      {
        // Auto fade out
        overlapEndDecay += 0.01f;
        if (overlapEndDecay > 1.0f)
        {
          overlapEndDecay = 0.0f;

          mTransition = true;
          mOpen = false;

          mDoorLeftSoundComponent->SetSound(mGateCueLeft);
          mDoorRightSoundComponent->SetSound(mGateCueRight);
          mDoorLeftSoundComponent->Play();
          mDoorRightSoundComponent->Play();
        }
      }
    }
  }
}

void ATorchBigDoor::OnBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
  if (otherActor != this)
  {
    mOverlap = true;
  }
}
void ATorchBigDoor::OnEndOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
  if (otherActor != this)
  {
    mOverlap = false;
  }
}