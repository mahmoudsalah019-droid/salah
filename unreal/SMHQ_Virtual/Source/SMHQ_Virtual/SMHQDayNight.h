#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMHQDayNight.generated.h"

/**
 * Moves the Sun Position plugin's SunSky between a day and a night hour and switches room lights.
 * SunSky is a Blueprint actor, so its SolarTime variable and UpdateSun function are reached by name
 * (verify both names in UE 5.8's SunSky Blueprint on day 1).
 */
UCLASS()
class SMHQ_VIRTUAL_API ASMHQDayNight : public AActor
{
	GENERATED_BODY()

public:
	ASMHQDayNight();

	/** Leave empty to auto-find the first actor whose class name contains "SunSky". */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight")
	TObjectPtr<AActor> SunSky;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight")
	FName SolarTimeProperty = TEXT("SolarTime");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight")
	FName UpdateSunFunction = TEXT("UpdateSun");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight", meta = (ClampMin = "0", ClampMax = "24"))
	float DayHour = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight", meta = (ClampMin = "0", ClampMax = "24"))
	float NightHour = 20.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight", meta = (ClampMin = "0.1"))
	float TransitionSeconds = 3.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DayNight")
	bool bStartAtNight = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "DayNight")
	bool bIsNight = false;

	UFUNCTION(BlueprintCallable, Category = "DayNight")
	void SetNight(bool bNight);

	UFUNCTION(BlueprintCallable, Category = "DayNight")
	void ToggleDayNight();

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

private:
	bool ApplySolarTime(float Hour);
	void FindSunSky();

	float FromHour = 10.f;
	float ToHour = 10.f;
	float Elapsed = 0.f;
};
