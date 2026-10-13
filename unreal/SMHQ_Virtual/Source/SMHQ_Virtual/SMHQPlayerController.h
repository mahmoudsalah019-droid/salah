#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SMHQPlayerController.generated.h"

class ASMHQInfoPoint;
class ASMHQTeleportPoint;
class USMHQHUDWidget;
enum class ESMHQCommand : uint8;

/** Owns the on-screen UI: prompt, menu (places, day/night, lights, quality, quit) and the info panel. */
UCLASS()
class SMHQ_VIRTUAL_API ASMHQPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "SMHQ|UI")
	TSubclassOf<USMHQHUDWidget> HUDWidgetClass;

	UFUNCTION(BlueprintCallable, Category = "SMHQ|UI")
	void ToggleMenu();

	UFUNCTION(BlueprintCallable, Category = "SMHQ|UI")
	void CloseOverlays();

	void SetPrompt(const FText& Text);
	void SetNearbyInfoPoint(ASMHQInfoPoint* Info, bool bInside);
	ASMHQInfoPoint* GetNearbyInfoPoint() const { return NearbyInfoPoint; }

	void OpenInfoPanel(ASMHQInfoPoint* Info);
	void CloseInfoPanel();

	bool IsMenuOpen() const { return bMenuOpen; }
	bool IsInfoOpen() const { return OpenInfo != nullptr; }
	bool IsUIBlockingMovement() const { return bMenuOpen || OpenInfo != nullptr; }

	UFUNCTION(BlueprintCallable, Category = "SMHQ")
	void ApplyQuality(int32 Level);

protected:
	virtual void BeginPlay() override;

private:
	void CollectTeleportPoints();
	void HandleTeleport(int32 Index);
	void FinishTeleport();
	void HandleCommand(ESMHQCommand Command);
	void UpdateInputMode();

	UPROPERTY(Transient) TObjectPtr<USMHQHUDWidget> HUD;
	UPROPERTY(Transient) TArray<TObjectPtr<ASMHQTeleportPoint>> TeleportPoints;
	UPROPERTY(Transient) TObjectPtr<ASMHQInfoPoint> NearbyInfoPoint;
	UPROPERTY(Transient) TObjectPtr<ASMHQInfoPoint> OpenInfo;

	TWeakObjectPtr<ASMHQTeleportPoint> PendingTeleport;
	FTimerHandle TeleportTimer;
	bool bMenuOpen = false;
};
