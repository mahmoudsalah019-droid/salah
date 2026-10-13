#include "SMHQDayNight.h"

#include "EngineUtils.h"
#include "SMHQRoomLights.h"
#include "SMHQ_Virtual.h"
#include "UObject/UnrealType.h"

ASMHQDayNight::ASMHQDayNight()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void ASMHQDayNight::BeginPlay()
{
	Super::BeginPlay();
	FindSunSky();
	bIsNight = bStartAtNight;
	ApplySolarTime(bIsNight ? NightHour : DayHour);
	if (bIsNight)
	{
		ASMHQRoomLights::SetAllRooms(this, true);
	}
}

void ASMHQDayNight::FindSunSky()
{
	if (SunSky)
	{
		return;
	}
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (It->GetClass()->GetName().Contains(TEXT("SunSky")))
		{
			SunSky = *It;
			return;
		}
	}
	UE_LOG(LogSMHQ, Warning, TEXT("DayNight: no SunSky actor found in the level"));
}

void ASMHQDayNight::SetNight(bool bNight)
{
	if (bNight == bIsNight && !IsActorTickEnabled())
	{
		return;
	}
	bIsNight = bNight;
	FromHour = bNight ? DayHour : NightHour;
	ToHour = bNight ? NightHour : DayHour;
	Elapsed = 0.f;
	SetActorTickEnabled(true);
}

void ASMHQDayNight::ToggleDayNight()
{
	SetNight(!bIsNight);
}

void ASMHQDayNight::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Elapsed += DeltaSeconds;
	const float T = FMath::Clamp(Elapsed / FMath::Max(TransitionSeconds, 0.1f), 0.f, 1.f);
	ApplySolarTime(FMath::Lerp(FromHour, ToHour, FMath::SmoothStep(0.f, 1.f, T)));

	if (T >= 1.f)
	{
		SetActorTickEnabled(false);
		if (bIsNight)
		{
			ASMHQRoomLights::SetAllRooms(this, true);
		}
		else
		{
			ASMHQRoomLights::RestoreAllRooms(this);
		}
	}
}

bool ASMHQDayNight::ApplySolarTime(float Hour)
{
	if (!SunSky)
	{
		return false;
	}

	FProperty* Prop = SunSky->GetClass()->FindPropertyByName(SolarTimeProperty);
	if (FDoubleProperty* DoubleProp = CastField<FDoubleProperty>(Prop))
	{
		DoubleProp->SetPropertyValue_InContainer(SunSky, Hour);
	}
	else if (FFloatProperty* FloatProp = CastField<FFloatProperty>(Prop))
	{
		FloatProp->SetPropertyValue_InContainer(SunSky, Hour);
	}
	else
	{
		UE_LOG(LogSMHQ, Warning, TEXT("DayNight: %s has no float/double property %s"), *SunSky->GetName(), *SolarTimeProperty.ToString());
		return false;
	}

	if (UFunction* Update = SunSky->FindFunction(UpdateSunFunction))
	{
		if (Update->ParmsSize == 0)
		{
			SunSky->ProcessEvent(Update, nullptr);
		}
	}
	return true;
}
