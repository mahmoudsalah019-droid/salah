#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SMHQHUDWidget.generated.h"

class ASMHQInfoPoint;
class UBorder;
class UCanvasPanel;
class UImage;
class UTextBlock;
class UVerticalBox;
class USMHQListButton;

/** Menu / info-panel commands. Plain enum (not reflected); button index = enum value. */
enum class ESMHQCommand : uint8
{
	DayNight,
	AllOn,
	AllOff,
	QualityLow,
	QualityMedium,
	QualityHigh,
	CloseMenu,
	Quit,
	InfoPrev,
	InfoNext,
	InfoVideo,
	InfoClose
};

DECLARE_DELEGATE_OneParam(FSMHQTeleportChosen, int32);
DECLARE_DELEGATE_OneParam(FSMHQCommandChosen, ESMHQCommand);

/**
 * Whole on-screen UI built in code (no Widget Blueprint needed): crosshair, prompt, menu, info panel.
 * A Blueprint subclass can still be made for styling; set it as HUDWidgetClass on the controller.
 */
UCLASS()
class SMHQ_VIRTUAL_API USMHQHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	USMHQHUDWidget(const FObjectInitializer& ObjectInitializer);

	/** Arabic-capable font (import e.g. Noto Naskh Arabic as /Game/SMHQ/UI/F_SMHQ_Arabic). Falls back to engine default. */
	UPROPERTY(EditDefaultsOnly, Category = "SMHQ|UI")
	TSoftObjectPtr<UObject> FontObject;

	void SetPrompt(const FText& Text);
	void SetMenuVisible(bool bVisible);
	void SetTeleportEntries(const TArray<FText>& Names);
	void ShowInfo(ASMHQInfoPoint* Info);
	void HideInfo();

	FSMHQTeleportChosen OnTeleportChosen;
	FSMHQCommandChosen OnCommandChosen;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeDestruct() override;

private:
	void BuildTree();
	UTextBlock* MakeText(const FText& Text, int32 Size) const;
	USMHQListButton* MakeButton(const FText& Label, int32 Index, bool bTeleport, int32 FontSize = 18);
	void HandleTeleportIndex(int32 Index);
	void HandleCommandIndex(int32 Index);
	void ShowImage(int32 Index);
	void StopInfoVideo();

	UPROPERTY(Transient) TObjectPtr<UTextBlock> PromptText;
	UPROPERTY(Transient) TObjectPtr<UBorder> MenuPanel;
	UPROPERTY(Transient) TObjectPtr<UVerticalBox> TeleportBox;
	UPROPERTY(Transient) TObjectPtr<UBorder> InfoPanel;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> InfoTitle;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> InfoProject;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> InfoDescription;
	UPROPERTY(Transient) TObjectPtr<UImage> InfoImage;
	UPROPERTY(Transient) TObjectPtr<USMHQListButton> VideoButton;
	UPROPERTY(Transient) TObjectPtr<UTextBlock> VideoButtonText;
	UPROPERTY(Transient) TObjectPtr<ASMHQInfoPoint> CurrentInfo;

	TArray<FText> PendingTeleportNames;
	int32 ImageIndex = 0;
	bool bVideoPlaying = false;
};
