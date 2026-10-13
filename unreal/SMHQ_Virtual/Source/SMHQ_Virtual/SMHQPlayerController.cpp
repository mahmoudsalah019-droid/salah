#include "SMHQPlayerController.h"

#include "Algo/Sort.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SMHQDayNight.h"
#include "SMHQHUDWidget.h"
#include "SMHQInfoPoint.h"
#include "SMHQRoomLights.h"
#include "SMHQTeleportPoint.h"
#include "TimerManager.h"

void ASMHQPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (!IsLocalController())
	{
		return;
	}

	const TSubclassOf<USMHQHUDWidget> WidgetClass = HUDWidgetClass ? HUDWidgetClass : TSubclassOf<USMHQHUDWidget>(USMHQHUDWidget::StaticClass());
	HUD = CreateWidget<USMHQHUDWidget>(this, WidgetClass);
	if (HUD)
	{
		HUD->AddToViewport();
		HUD->OnTeleportChosen.BindUObject(this, &ASMHQPlayerController::HandleTeleport);
		HUD->OnCommandChosen.BindUObject(this, &ASMHQPlayerController::HandleCommand);
	}

	CollectTeleportPoints();

	if (!UGameplayStatics::GetActorOfClass(this, ASMHQDayNight::StaticClass()))
	{
		GetWorld()->SpawnActor<ASMHQDayNight>();
	}

	UpdateInputMode();
}

void ASMHQPlayerController::CollectTeleportPoints()
{
	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(this, ASMHQTeleportPoint::StaticClass(), Found);

	TArray<ASMHQTeleportPoint*> Sorted;
	for (AActor* Actor : Found)
	{
		Sorted.Add(CastChecked<ASMHQTeleportPoint>(Actor));
	}
	Algo::Sort(Sorted, [](const ASMHQTeleportPoint* A, const ASMHQTeleportPoint* B)
	{
		return A->SortOrder != B->SortOrder ? A->SortOrder < B->SortOrder : A->SpaceCode.LexicalLess(B->SpaceCode);
	});
	TeleportPoints.Reset();
	for (ASMHQTeleportPoint* Point : Sorted)
	{
		TeleportPoints.Add(Point);
	}

	TArray<FText> Names;
	for (const ASMHQTeleportPoint* Point : TeleportPoints)
	{
		Names.Add(Point->GetMenuName());
	}
	if (HUD)
	{
		HUD->SetTeleportEntries(Names);
	}
}

void ASMHQPlayerController::SetPrompt(const FText& Text)
{
	if (HUD)
	{
		HUD->SetPrompt(Text);
	}
}

void ASMHQPlayerController::ToggleMenu()
{
	if (IsInfoOpen())
	{
		CloseInfoPanel();
	}
	bMenuOpen = !bMenuOpen;
	if (HUD)
	{
		HUD->SetMenuVisible(bMenuOpen);
	}
	UpdateInputMode();
}

void ASMHQPlayerController::CloseOverlays()
{
	if (IsInfoOpen())
	{
		CloseInfoPanel();
	}
	if (bMenuOpen)
	{
		ToggleMenu();
	}
}

void ASMHQPlayerController::SetNearbyInfoPoint(ASMHQInfoPoint* Info, bool bInside)
{
	if (bInside)
	{
		NearbyInfoPoint = Info;
	}
	else if (NearbyInfoPoint == Info)
	{
		NearbyInfoPoint = nullptr;
		if (OpenInfo == Info)
		{
			CloseInfoPanel();
		}
	}
}

void ASMHQPlayerController::OpenInfoPanel(ASMHQInfoPoint* Info)
{
	if (!Info || !HUD)
	{
		return;
	}
	if (bMenuOpen)
	{
		ToggleMenu();
	}
	OpenInfo = Info;
	HUD->ShowInfo(Info);
	UpdateInputMode();
}

void ASMHQPlayerController::CloseInfoPanel()
{
	if (HUD)
	{
		HUD->HideInfo();
	}
	OpenInfo = nullptr;
	UpdateInputMode();
}

void ASMHQPlayerController::UpdateInputMode()
{
	if (IsUIBlockingMovement())
	{
		FInputModeGameAndUI Mode;
		Mode.SetHideCursorDuringCapture(false);
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(Mode);
		bShowMouseCursor = true;
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
}

void ASMHQPlayerController::HandleTeleport(int32 Index)
{
	if (!TeleportPoints.IsValidIndex(Index))
	{
		return;
	}
	PendingTeleport = TeleportPoints[Index];
	if (bMenuOpen)
	{
		ToggleMenu();
	}
	if (PlayerCameraManager)
	{
		PlayerCameraManager->StartCameraFade(0.f, 1.f, 0.25f, FLinearColor::Black, false, true);
	}
	GetWorldTimerManager().SetTimer(TeleportTimer, this, &ASMHQPlayerController::FinishTeleport, 0.25f, false);
}

void ASMHQPlayerController::FinishTeleport()
{
	APawn* MyPawn = GetPawn();
	const ASMHQTeleportPoint* Target = PendingTeleport.Get();
	if (MyPawn && Target)
	{
		float HalfHeight = 0.f;
		if (const ACharacter* TargetCharacter = Cast<ACharacter>(MyPawn))
		{
			HalfHeight = TargetCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		}
		const FRotator Facing(0.f, Target->GetActorRotation().Yaw, 0.f);
		MyPawn->TeleportTo(Target->GetActorLocation() + FVector(0.f, 0.f, HalfHeight + 2.f), Facing);
		SetControlRotation(Facing);
	}
	if (PlayerCameraManager)
	{
		PlayerCameraManager->StartCameraFade(1.f, 0.f, 0.25f, FLinearColor::Black, false, false);
	}
}

void ASMHQPlayerController::HandleCommand(ESMHQCommand Command)
{
	switch (Command)
	{
	case ESMHQCommand::DayNight:
		if (ASMHQDayNight* DayNight = Cast<ASMHQDayNight>(UGameplayStatics::GetActorOfClass(this, ASMHQDayNight::StaticClass())))
		{
			DayNight->ToggleDayNight();
		}
		break;
	case ESMHQCommand::AllOn:
		ASMHQRoomLights::SetAllRooms(this, true);
		break;
	case ESMHQCommand::AllOff:
		ASMHQRoomLights::SetAllRooms(this, false);
		break;
	case ESMHQCommand::QualityLow:
		ApplyQuality(0);
		break;
	case ESMHQCommand::QualityMedium:
		ApplyQuality(1);
		break;
	case ESMHQCommand::QualityHigh:
		ApplyQuality(2);
		break;
	case ESMHQCommand::CloseMenu:
		if (bMenuOpen)
		{
			ToggleMenu();
		}
		break;
	case ESMHQCommand::Quit:
		UKismetSystemLibrary::QuitGame(this, this, EQuitPreference::Quit, false);
		break;
	case ESMHQCommand::InfoClose:
		CloseInfoPanel();
		break;
	default:
		break;
	}
}

void ASMHQPlayerController::ApplyQuality(int32 Level)
{
	if (UGameUserSettings* Settings = GEngine ? GEngine->GetGameUserSettings() : nullptr)
	{
		Settings->SetOverallScalabilityLevel(FMath::Clamp(Level, 0, 3));
		Settings->ApplySettings(false);
	}
}
