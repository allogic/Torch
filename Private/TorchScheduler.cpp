#include "TorchScheduler.h"
#include "TorchCore.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"

ATorchScheduler::ATorchScheduler()
{
  // Setup actor
  PrimaryActorTick.bCanEverTick = true;

  // Setup material parameter collection
  static ConstructorHelpers::FObjectFinder<UMaterialParameterCollection> hellShaderParameters(TEXT("MaterialParameterCollection'/Game/Build/Global/MC_HellSphere.MC_HellSphere'"));
  if (hellShaderParameters.Succeeded()) mHellShaderParameters = hellShaderParameters.Object;
}

void ATorchScheduler::BeginPlay()
{
  Super::BeginPlay();

  // Configure material parameter collection
  if (mHellShaderParameters)
  {
    mHellShaderParametersInstance = GetWorld()->GetParameterCollectionInstance(mHellShaderParameters);
  }
  SyncHellSphereParameters();

  // Start hell sphere decrease timer
  StartTask(TEXT("HellSphereDecreaseBegin"), 4.0f);
}
void ATorchScheduler::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

  // Update hell sphere parameters
  if (mEnableHellSphereDecay)
  {
    if (mHellSphereRadius > 0.0f)
    {
      mHellSphereRadius -= mHellSphereRadiusDecreaseIntensity * GetWorld()->GetDeltaSeconds();
    }

    SyncHellSphereParameters();
  }
}

void ATorchScheduler::HellSphereDecreaseBegin()
{
  mEnableHellSphereDecay = true;

  // Start hell sphere stop timer
  StartTask(TEXT("HellSphereDecreaseEnd"), 1.0f);
}
void ATorchScheduler::HellSphereDecreaseEnd()
{
  mEnableHellSphereDecay = false;

  // Start hell sphere decrease timer
  if (mHellSphereRadius > 0.0f)
  {
    StartTask(TEXT("HellSphereDecreaseBegin"), 4.0f);
  }
}

void ATorchScheduler::SyncHellSphereParameters()
{
  // Sync to material instances
  if (mHellShaderParametersInstance)
  {
    mHellShaderParametersInstance->SetVectorParameterValue(TEXT("SphereLocation"), mHellSphereLocation);
    mHellShaderParametersInstance->SetScalarParameterValue(TEXT("SphereRadius"), mHellSphereRadius);
    mHellShaderParametersInstance->SetScalarParameterValue(TEXT("SphereHardness"), mHellSphereHardness);
  }
}

ATorchScheduler::FTorchTask& ATorchScheduler::FindOrAdd(const FName& taskName)
{
  FTorchTask& task = mTasks.FindOrAdd(taskName);
  if (!task.Delegate.IsBound())
  {
    task.Delegate.BindUFunction(this, taskName);
  }
  return task;
}
ATorchScheduler::FTorchTask& ATorchScheduler::StartTask(const FName& taskName, float delaySeconds)
{
  FTorchTask& task = FindOrAdd(taskName);
  GetWorldTimerManager().SetTimer(task.Handle, task.Delegate, delaySeconds, false);
  return task;
}