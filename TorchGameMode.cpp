#include "TorchGameMode.h"
#include "TorchFPSCharacter.h"
//#include "TorchTPSController.h"
#include "TorchGameState.h"
#include "TorchHUD.h"

ATorchGameMode::ATorchGameMode()
{
  DefaultPawnClass = ATorchFPSCharacter::StaticClass();
  //PlayerControllerClass = ATorchTPSController::StaticClass();
  GameStateClass = ATorchGameState::StaticClass();
  HUDClass = ATorchHUD::StaticClass();
}