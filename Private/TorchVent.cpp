#include "TorchVent.h"
#include "TorchCore.h"
#include "Kismet/GameplayStatics.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/RectLightComponent.h"
#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Sound/SoundWave.h"

ATorchVent::ATorchVent()
{
  // Setup actor
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
  PrimaryActorTick.bCanEverTick = true;

  // Load object references
  static ConstructorHelpers::FObjectFinder<UStaticMesh> ventMesh(TEXT("StaticMesh'/Game/Build/Environment/Meshes/Props/Vent/Vent.Vent'"));
  static ConstructorHelpers::FObjectFinder<UStaticMesh> ventBladeMesh(TEXT("StaticMesh'/Game/Build/Environment/Meshes/Props/VentBlade/VentBlade.VentBlade'"));
  static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ventSystem(TEXT(""));
  static ConstructorHelpers::FObjectFinder<USoundWave> ventOpenWave(TEXT("SoundWave'/Game/Build/Audio/Props/Vent/VentOpen.VentOpen'"));
  static ConstructorHelpers::FObjectFinder<USoundWave> ventLoopWave(TEXT("SoundWave'/Game/Build/Audio/Props/Vent/VentLoop.VentLoop'"));
  static ConstructorHelpers::FObjectFinder<USoundWave> ventCloseWave(TEXT("SoundWave'/Game/Build/Audio/Props/Vent/VentClose.VentClose'"));

  // Setup vent static mesh
  mVentComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Vent"));
  mVentComponent->SetupAttachment(RootComponent);
  if (ventMesh.Succeeded()) mVentComponent->SetStaticMesh(ventMesh.Object);

  // Setup vent blades instanced static mesh
  mInstancedVentBladeComponents = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("VentBlades"));
  mInstancedVentBladeComponents->SetupAttachment(RootComponent);
  mInstancedVentBladeComponents->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  if (ventBladeMesh.Succeeded())
  {
    mInstancedVentBladeComponents->SetStaticMesh(ventBladeMesh.Object);
    mInstancedVentBladeComponents->SetMaterial(0, ventBladeMesh.Object->GetMaterial(0));
  }

  // Setup heat particles
  mHeatParticleComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HeatParticles"));
  mHeatParticleComponent->SetupAttachment(RootComponent);

  if (ventSystem.Succeeded()) mHeatParticleComponent->SetAsset(ventSystem.Object);

  // Setup audio
  mAudioOpenCloseComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioOpenClose"));
  mAudioOpenCloseComponent->SetupAttachment(RootComponent);
  mAudioOpenCloseComponent->bAutoActivate = false;
  mAudioOpenCloseComponent->SetRelativeLocation(mLightPosition);
  mAudioLoopComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioLoop"));
  mAudioLoopComponent->SetupAttachment(RootComponent);
  mAudioLoopComponent->bAutoActivate = false;
  mAudioLoopComponent->SetRelativeLocation(mLightPosition);
  if (ventOpenWave.Succeeded()) mWaveVentOpen = ventOpenWave.Object;
  if (ventLoopWave.Succeeded()) mWaveVentLoop = ventLoopWave.Object;
  if (ventCloseWave.Succeeded()) mWaveVentClose = ventCloseWave.Object;

  // Setup light
  mRectLightComponent = CreateDefaultSubobject<URectLightComponent>(TEXT("Light"));
  mRectLightComponent->SetupAttachment(RootComponent);

  // Setup timer delegates
  mTimerDelegateFlipExcaust.BindUFunction(this, TEXT("FlipExcaust"));
}

void ATorchVent::BeginPlay()
{
	 Super::BeginPlay();

  // Setup vent blades static mesh
  for (int32 i = 0; i < mNumVentBlades; ++i)
  {
    FVector ventBladeLocation = mVentBladeBaseOffset + mVentBladeOffset * i;
    mInstancedVentBladeComponents->AddInstance(FTransform{ mVentBladeRotation, ventBladeLocation, FVector::OneVector });
  }

  // Setup heat particles
  mHeatParticleComponent->SetRelativeLocation(mLightPosition);
  mHeatParticleComponent->SetRelativeRotation(FRotator{ 90.0f, 90.0f, -90.0f });

  // Setup audio
  FSoundAttenuationSettings soundSettings{};
  soundSettings.bAttenuate = true;
  soundSettings.FocusDistanceScale = 1.5f;
  soundSettings.bEnableReverbSend = true;
  soundSettings.ReverbDistanceMin = 100.0f;
  soundSettings.ReverbDistanceMax = 1000.0f;
  mAudioOpenCloseComponent->bAutoActivate = false;
  mAudioOpenCloseComponent->SetRelativeLocation(mLightPosition);
  mAudioOpenCloseComponent->AdjustAttenuation(soundSettings);
  mAudioLoopComponent->bAutoActivate = false;
  mAudioLoopComponent->SetRelativeLocation(mLightPosition);
  mAudioLoopComponent->AdjustAttenuation(soundSettings);

  // Setup light
  mRectLightComponent->SetRelativeLocation(mLightPosition);
  mRectLightComponent->SetLightColor(mLightColor);
  mRectLightComponent->SetSourceWidth(mLightWidth);
  mRectLightComponent->SetSourceHeight(mLightHeight);
  mRectLightComponent->SetIntensity(mLightIntensity);
  mRectLightComponent->SetVolumetricScatteringIntensity(mLightScatterIntensity);
  mRectLightComponent->SetAttenuationRadius(mLightAttenuationRadius);

  // Start excaust timer
  GetWorldTimerManager().SetTimer(mTimerHandleFlipExcaust, mTimerDelegateFlipExcaust, FMath::RandRange(mFlipOpenMinSeconds, mFlipOpenMaxSeconds), false);
}
void ATorchVent::Tick(float deltaTime)
{
  Super::Tick(deltaTime);

  // Procedural lerp to target rotation
  FRotator targetRotation = mOpen ? FRotator{ 0.0f, 0.0f, 0.0f } : FRotator{ 0.0f, -75.0f, 0.0f };
  FTransform ventBladeTransform;
  for (int32 i = 0; i < mInstancedVentBladeComponents->GetNumRenderInstances(); ++i)
  {
    mInstancedVentBladeComponents->GetInstanceTransform(i, ventBladeTransform);
    ventBladeTransform.SetRotation(FMath::RInterpTo(ventBladeTransform.GetRotation().Rotator(), targetRotation, deltaTime, 2.5f + FMath::Abs(FMath::PerlinNoise1D(float(30 + i) / 15.0f)) * 10.0f).Quaternion());
    mInstancedVentBladeComponents->UpdateInstanceTransform(i, ventBladeTransform);
  }
  mInstancedVentBladeComponents->MarkRenderStateDirty();

  // Update heat particles
  mHeatParticleComponent->SetFloatParameter(TEXT("SpawnRate"), mOpen ? 150.0f : 0.0f);
}

void ATorchVent::FlipExcaust()
{
  // Flip state
  mOpen = !mOpen;

  // Restart excaust timer
  GetWorldTimerManager().ClearTimer(mTimerHandleFlipExcaust);
  float randomDuration = mOpen ? FMath::RandRange(mFlipCloseMinSeconds, mFlipCloseMaxSeconds) : FMath::RandRange(mFlipOpenMinSeconds, mFlipOpenMaxSeconds);
  GetWorldTimerManager().SetTimer(mTimerHandleFlipExcaust, mTimerDelegateFlipExcaust, randomDuration, false);

  // Play sound
  if (mOpen)
  {
    mAudioOpenCloseComponent->SetSound(mWaveVentOpen);
    mAudioLoopComponent->SetSound(mWaveVentLoop);
    mAudioLoopComponent->FadeIn(0.2f, 0.6f);
  }
  else
  {
    mAudioOpenCloseComponent->SetSound(mWaveVentClose);
    mAudioLoopComponent->FadeOut(0.5f, 0.0f);
  }
  mAudioOpenCloseComponent->FadeIn(0.1f, 0.1f);
}