#include "TorchHUD.h"
#include "TorchCore.h"

ATorchHUD::ATorchHUD()
{

}

void ATorchHUD::SetDebugTexture(uint8* source, int32 width, int32 height)
{
  // Dispose current debug texture
  if (mDebugTexture)
  {
    mDebugTexture->RemoveFromRoot();
    mDebugTexture->ConditionalBeginDestroy();
    mDebugTexture = nullptr;
  }
  // Create new debug texture
  mDebugTexture = UTexture2D::CreateTransient(width, height);
  mDebugTexture->Filter = TextureFilter::TF_Nearest;
  mDebugTexture->bNoTiling = true;
  mDebugTexture->SetFlags(RF_Public);
  // Copy buffer
  FTexture2DMipMap& mip = mDebugTexture->PlatformData->Mips[0];
  uint8* target = (uint8*)mip.BulkData.Lock(LOCK_READ_WRITE);
  FMemory::Memcpy(target, source, width * height * 4);
  mip.BulkData.Unlock();
  mDebugTexture->UpdateResource();
}

void ATorchHUD::BeginPlay()
{
  Super::BeginPlay();
}
void ATorchHUD::EndPlay(const EEndPlayReason::Type endPlayReason)
{
  Super::EndPlay(endPlayReason);

  // Dispose debug texture
  if (mDebugTexture)
  {
    mDebugTexture->RemoveFromRoot();
    mDebugTexture->ConditionalBeginDestroy();
  }
}
void ATorchHUD::DrawHUD()
{
  Super::DrawHUD();

  if (mDebugTexture)
  {
    DrawTexture(
      mDebugTexture,
      0.0f, 0.0f,
      650.0f, 650.0f,
      0.0f, 0.0f,
      1.0f, 1.0f,
      FLinearColor::White,
      EBlendMode::BLEND_Translucent,
      1.0f,
      false,
      0.0f,
      FVector2D{});
  }
}