#include "TorchGameMode.h"
#include "TorchTPSCharacter.h"
#include "TorchTPSController.h"
#include "TorchGameState.h"
#include "TorchHUD.h"

ATorchGameMode::ATorchGameMode()
{
  DefaultPawnClass = ATorchTPSCharacter::StaticClass();
  PlayerControllerClass = ATorchTPSController::StaticClass();
  GameStateClass = ATorchGameState::StaticClass();
  HUDClass = ATorchHUD::StaticClass();
}