#include "SMHQGameMode.h"

#include "SMHQCharacter.h"
#include "SMHQPlayerController.h"

ASMHQGameMode::ASMHQGameMode()
{
	DefaultPawnClass = ASMHQCharacter::StaticClass();
	PlayerControllerClass = ASMHQPlayerController::StaticClass();
}
