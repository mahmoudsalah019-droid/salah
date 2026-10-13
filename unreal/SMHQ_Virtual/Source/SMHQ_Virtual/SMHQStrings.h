#pragma once

#include "CoreMinimal.h"

/** All visitor-facing text in one place (SMHQStrings.cpp is saved as UTF-8 with BOM for the Arabic literals). */
namespace SMHQStrings
{
	FText DoorOpen();
	FText DoorClose();
	FText DoorLocked();
	FText LightSwitch();
	FText InfoPoint();
	FText ScreenPlayPause();
	FText MenuTitle();
	FText MenuPlaces();
	FText DayNight();
	FText AllLightsOn();
	FText AllLightsOff();
	FText QualityLow();
	FText QualityMedium();
	FText QualityHigh();
	FText CloseMenu();
	FText Quit();
	FText Previous();
	FText Next();
	FText PlayVideo();
	FText StopVideo();
	FText Close();
}
