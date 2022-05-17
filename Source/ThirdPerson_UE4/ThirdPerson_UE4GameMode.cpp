// Copyright Ready Player Me 2022

#include "ThirdPerson_UE4GameMode.h"
#include "ThirdPerson_UE4Character.h"
#include "UObject/ConstructorHelpers.h"

AThirdPerson_UE4GameMode::AThirdPerson_UE4GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ReadyPlayerme/Blueprints/RPM_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
