#include "SMHQHUDWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Engine/Texture2D.h"
#include "MediaTexture.h"
#include "SMHQInfoPoint.h"
#include "SMHQListButton.h"
#include "SMHQStrings.h"

USMHQHUDWidget::USMHQHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FontObject = TSoftObjectPtr<UObject>(FSoftObjectPath(TEXT("/Game/SMHQ/UI/F_SMHQ_Arabic.F_SMHQ_Arabic")));
}

TSharedRef<SWidget> USMHQHUDWidget::RebuildWidget()
{
	if (WidgetTree && !WidgetTree->RootWidget)
	{
		BuildTree();
	}
	return Super::RebuildWidget();
}

void USMHQHUDWidget::NativeDestruct()
{
	StopInfoVideo();
	Super::NativeDestruct();
}

UTextBlock* USMHQHUDWidget::MakeText(const FText& Text, int32 Size) const
{
	UTextBlock* Block = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
	Block->SetText(Text);
	FSlateFontInfo Font = Block->GetFont();
	if (UObject* FontAsset = FontObject.LoadSynchronous())
	{
		Font = FSlateFontInfo(FontAsset, Size);
	}
	else
	{
		Font.Size = Size;
	}
	Block->SetFont(Font);
	Block->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	return Block;
}

USMHQListButton* USMHQHUDWidget::MakeButton(const FText& Label, int32 Index, bool bTeleport, int32 FontSize)
{
	USMHQListButton* Button = WidgetTree->ConstructWidget<USMHQListButton>(USMHQListButton::StaticClass());
	Button->Setup(Index);
	if (bTeleport)
	{
		Button->OnIndexClicked.BindUObject(this, &USMHQHUDWidget::HandleTeleportIndex);
	}
	else
	{
		Button->OnIndexClicked.BindUObject(this, &USMHQHUDWidget::HandleCommandIndex);
	}
	Button->SetBackgroundColor(FLinearColor(0.92f, 0.92f, 0.92f, 0.95f));
	UTextBlock* Text = MakeText(Label, FontSize);
	Text->SetColorAndOpacity(FSlateColor(FLinearColor::Black));
	Button->AddChild(Text);
	return Button;
}

void USMHQHUDWidget::BuildTree()
{
	UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("Root"));
	WidgetTree->RootWidget = Root;

	// Crosshair
	UBorder* Dot = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
	Dot->SetBrushColor(FLinearColor(1.f, 1.f, 1.f, 0.8f));
	UCanvasPanelSlot* DotSlot = Root->AddChildToCanvas(Dot);
	DotSlot->SetAnchors(FAnchors(0.5f, 0.5f));
	DotSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	DotSlot->SetSize(FVector2D(6.f, 6.f));
	DotSlot->SetPosition(FVector2D::ZeroVector);

	// Prompt
	PromptText = MakeText(FText::GetEmpty(), 22);
	PromptText->SetJustification(ETextJustify::Center);
	UCanvasPanelSlot* PromptSlot = Root->AddChildToCanvas(PromptText);
	PromptSlot->SetAnchors(FAnchors(0.5f, 1.f));
	PromptSlot->SetAlignment(FVector2D(0.5f, 1.f));
	PromptSlot->SetPosition(FVector2D(0.f, -60.f));
	PromptSlot->SetAutoSize(true);

	// Menu
	MenuPanel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
	MenuPanel->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.75f));
	MenuPanel->SetPadding(FMargin(20.f));
	UVerticalBox* MenuBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
	MenuPanel->SetContent(MenuBox);

	auto AddRow = [](UVerticalBox* Box, UWidget* Widget, float Top = 4.f)
	{
		if (UVerticalBoxSlot* RowSlot = Box->AddChildToVerticalBox(Widget))
		{
			RowSlot->SetPadding(FMargin(0.f, Top, 0.f, 0.f));
		}
	};

	AddRow(MenuBox, MakeText(SMHQStrings::MenuTitle(), 24), 0.f);
	AddRow(MenuBox, MakeText(SMHQStrings::MenuPlaces(), 18), 16.f);
	TeleportBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
	AddRow(MenuBox, TeleportBox);

	const TPair<FText, ESMHQCommand> Commands[] = {
		{ SMHQStrings::DayNight(), ESMHQCommand::DayNight },
		{ SMHQStrings::AllLightsOn(), ESMHQCommand::AllOn },
		{ SMHQStrings::AllLightsOff(), ESMHQCommand::AllOff },
		{ SMHQStrings::QualityLow(), ESMHQCommand::QualityLow },
		{ SMHQStrings::QualityMedium(), ESMHQCommand::QualityMedium },
		{ SMHQStrings::QualityHigh(), ESMHQCommand::QualityHigh },
		{ SMHQStrings::CloseMenu(), ESMHQCommand::CloseMenu },
		{ SMHQStrings::Quit(), ESMHQCommand::Quit },
	};
	bool bFirst = true;
	for (const TPair<FText, ESMHQCommand>& Command : Commands)
	{
		AddRow(MenuBox, MakeButton(Command.Key, static_cast<int32>(Command.Value), false), bFirst ? 16.f : 4.f);
		bFirst = false;
	}

	UCanvasPanelSlot* MenuSlot = Root->AddChildToCanvas(MenuPanel);
	MenuSlot->SetAnchors(FAnchors(0.f, 0.5f));
	MenuSlot->SetAlignment(FVector2D(0.f, 0.5f));
	MenuSlot->SetPosition(FVector2D(40.f, 0.f));
	MenuSlot->SetAutoSize(true);
	MenuPanel->SetVisibility(ESlateVisibility::Collapsed);

	// Info panel
	InfoPanel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
	InfoPanel->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.85f));
	InfoPanel->SetPadding(FMargin(24.f));
	UVerticalBox* InfoBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass());
	InfoPanel->SetContent(InfoBox);

	InfoTitle = MakeText(FText::GetEmpty(), 28);
	InfoProject = MakeText(FText::GetEmpty(), 18);
	InfoImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass());
	InfoImage->SetDesiredSizeOverride(FVector2D(960.f, 540.f));
	InfoDescription = MakeText(FText::GetEmpty(), 18);
	InfoDescription->SetAutoWrapText(true);

	UHorizontalBox* InfoButtons = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
	auto AddButton = [InfoButtons](UWidget* Widget)
	{
		if (UHorizontalBoxSlot* ButtonSlot = InfoButtons->AddChildToHorizontalBox(Widget))
		{
			ButtonSlot->SetPadding(FMargin(0.f, 0.f, 8.f, 0.f));
		}
	};
	AddButton(MakeButton(SMHQStrings::Previous(), static_cast<int32>(ESMHQCommand::InfoPrev), false));
	AddButton(MakeButton(SMHQStrings::Next(), static_cast<int32>(ESMHQCommand::InfoNext), false));
	VideoButton = MakeButton(SMHQStrings::PlayVideo(), static_cast<int32>(ESMHQCommand::InfoVideo), false);
	VideoButtonText = Cast<UTextBlock>(VideoButton->GetChildAt(0));
	AddButton(VideoButton);
	AddButton(MakeButton(SMHQStrings::Close(), static_cast<int32>(ESMHQCommand::InfoClose), false));

	AddRow(InfoBox, InfoTitle, 0.f);
	AddRow(InfoBox, InfoProject);
	AddRow(InfoBox, InfoImage, 12.f);
	AddRow(InfoBox, InfoButtons, 12.f);
	AddRow(InfoBox, InfoDescription, 12.f);

	UCanvasPanelSlot* InfoSlot = Root->AddChildToCanvas(InfoPanel);
	InfoSlot->SetAnchors(FAnchors(0.5f, 0.5f));
	InfoSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	InfoSlot->SetPosition(FVector2D::ZeroVector);
	InfoSlot->SetAutoSize(true);
	InfoPanel->SetVisibility(ESlateVisibility::Collapsed);

	if (PendingTeleportNames.Num() > 0)
	{
		SetTeleportEntries(PendingTeleportNames);
	}
}

void USMHQHUDWidget::SetPrompt(const FText& Text)
{
	if (PromptText)
	{
		PromptText->SetText(Text);
	}
}

void USMHQHUDWidget::SetMenuVisible(bool bVisible)
{
	if (MenuPanel)
	{
		MenuPanel->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void USMHQHUDWidget::SetTeleportEntries(const TArray<FText>& Names)
{
	PendingTeleportNames = Names;
	if (!TeleportBox)
	{
		return; // applied when the tree is built
	}
	TeleportBox->ClearChildren();
	for (int32 i = 0; i < Names.Num(); ++i)
	{
		if (UVerticalBoxSlot* RowSlot = TeleportBox->AddChildToVerticalBox(MakeButton(Names[i], i, true)))
		{
			RowSlot->SetPadding(FMargin(0.f, 4.f, 0.f, 0.f));
		}
	}
}

void USMHQHUDWidget::ShowInfo(ASMHQInfoPoint* Info)
{
	if (!Info || !InfoPanel)
	{
		return;
	}
	StopInfoVideo();
	CurrentInfo = Info;
	InfoTitle->SetText(Info->Title);
	InfoProject->SetText(FText::FromString(Info->ProjectCode));
	InfoDescription->SetText(Info->Description);
	VideoButton->SetVisibility(Info->Video ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	ShowImage(0);
	InfoPanel->SetVisibility(ESlateVisibility::Visible);
}

void USMHQHUDWidget::HideInfo()
{
	StopInfoVideo();
	CurrentInfo = nullptr;
	if (InfoPanel)
	{
		InfoPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USMHQHUDWidget::ShowImage(int32 Index)
{
	if (!CurrentInfo || CurrentInfo->Images.Num() == 0)
	{
		InfoImage->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	ImageIndex = (Index % CurrentInfo->Images.Num() + CurrentInfo->Images.Num()) % CurrentInfo->Images.Num();
	InfoImage->SetBrushFromTexture(CurrentInfo->Images[ImageIndex]);
	InfoImage->SetDesiredSizeOverride(FVector2D(960.f, 540.f));
	InfoImage->SetVisibility(ESlateVisibility::Visible);
}

void USMHQHUDWidget::StopInfoVideo()
{
	if (bVideoPlaying && CurrentInfo)
	{
		CurrentInfo->StopVideo();
	}
	bVideoPlaying = false;
	if (VideoButtonText)
	{
		VideoButtonText->SetText(SMHQStrings::PlayVideo());
	}
}

void USMHQHUDWidget::HandleTeleportIndex(int32 Index)
{
	OnTeleportChosen.ExecuteIfBound(Index);
}

void USMHQHUDWidget::HandleCommandIndex(int32 Index)
{
	const ESMHQCommand Command = static_cast<ESMHQCommand>(Index);
	switch (Command)
	{
	case ESMHQCommand::InfoPrev:
		StopInfoVideo();
		ShowImage(ImageIndex - 1);
		return;
	case ESMHQCommand::InfoNext:
		StopInfoVideo();
		ShowImage(ImageIndex + 1);
		return;
	case ESMHQCommand::InfoVideo:
		if (bVideoPlaying)
		{
			StopInfoVideo();
			ShowImage(ImageIndex);
		}
		else if (CurrentInfo)
		{
			if (UMediaTexture* VideoTexture = CurrentInfo->StartVideo())
			{
				InfoImage->SetBrushResourceObject(VideoTexture);
				InfoImage->SetDesiredSizeOverride(FVector2D(960.f, 540.f));
				InfoImage->SetVisibility(ESlateVisibility::Visible);
				bVideoPlaying = true;
				if (VideoButtonText)
				{
					VideoButtonText->SetText(SMHQStrings::StopVideo());
				}
			}
		}
		return;
	default:
		OnCommandChosen.ExecuteIfBound(Command);
		return;
	}
}
