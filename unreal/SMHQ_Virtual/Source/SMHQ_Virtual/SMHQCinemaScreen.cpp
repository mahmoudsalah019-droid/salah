#include "SMHQCinemaScreen.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"
#include "MediaPlayer.h"
#include "MediaSoundComponent.h"
#include "MediaSource.h"
#include "MediaTexture.h"
#include "SMHQRoomLights.h"
#include "SMHQStrings.h"
#include "SMHQ_Virtual.h"

ASMHQCinemaScreen::ASMHQCinemaScreen()
{
	PrimaryActorTick.bCanEverTick = false;

	ScreenMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ScreenMesh"));
	RootComponent = ScreenMesh;

	HallTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("HallTrigger"));
	HallTrigger->SetupAttachment(ScreenMesh);
	HallTrigger->SetBoxExtent(FVector(1000.f, 1000.f, 300.f));
	HallTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HallTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	HallTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HallTrigger->SetGenerateOverlapEvents(true);

	Sound = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("Sound"));
	Sound->SetupAttachment(ScreenMesh);

	ScreenMaterial = TSoftObjectPtr<UMaterialInterface>(FSoftObjectPath(TEXT("/Game/SMHQ/Materials/M_SMHQ_Screen.M_SMHQ_Screen")));
}

void ASMHQCinemaScreen::ConfigureScreen(UStaticMesh* Mesh, UMediaSource* InSource, bool bInAutoPlay, bool bInLoop,
	ASMHQRoomLights* InDimRoom, FVector HallExtent, FVector HallOffset)
{
	if (Mesh)
	{
		ScreenMesh->SetStaticMesh(Mesh);
	}
	MediaSource = InSource;
	bAutoPlay = bInAutoPlay;
	bLoop = bInLoop;
	DimRoom = InDimRoom;
	HallTrigger->SetBoxExtent(HallExtent);
	HallTrigger->SetWorldLocation(GetActorLocation() + HallOffset);
}

void ASMHQCinemaScreen::BeginPlay()
{
	Super::BeginPlay();

	Player = NewObject<UMediaPlayer>(this);
	Player->SetLooping(bLoop);
	Player->PlayOnOpen = true;

	VideoTexture = NewObject<UMediaTexture>(this);
	VideoTexture->SetMediaPlayer(Player);
	VideoTexture->UpdateResource();

	Sound->SetMediaPlayer(Player);

	if (UMaterialInterface* Base = ScreenMaterial.LoadSynchronous())
	{
		ScreenMID = UMaterialInstanceDynamic::Create(Base, this);
		ScreenMID->SetTextureParameterValue(TEXT("Video"), VideoTexture);
		ScreenMID->SetScalarParameterValue(TEXT("Brightness"), Brightness);
		for (int32 i = 0; i < ScreenMesh->GetNumMaterials(); ++i)
		{
			ScreenMesh->SetMaterial(i, ScreenMID);
		}
	}
	else
	{
		UE_LOG(LogSMHQ, Warning, TEXT("%s: screen material missing - run smhq_setup.py"), *GetName());
	}

	HallTrigger->OnComponentBeginOverlap.AddDynamic(this, &ASMHQCinemaScreen::OnHallBegin);
	HallTrigger->OnComponentEndOverlap.AddDynamic(this, &ASMHQCinemaScreen::OnHallEnd);
}

void ASMHQCinemaScreen::Play()
{
	if (!Player || !MediaSource)
	{
		return;
	}
	if (!bOpened)
	{
		bOpened = Player->OpenSource(MediaSource);
	}
	else
	{
		Player->Play();
	}
	if (!Sound->IsPlaying())
	{
		Sound->Start();
	}
	if (DimRoom)
	{
		DimRoom->SetDimFactor(DimFactor);
	}
}

void ASMHQCinemaScreen::Pause()
{
	if (Player)
	{
		Player->Pause();
	}
	if (DimRoom)
	{
		DimRoom->SetDimFactor(1.f);
	}
}

void ASMHQCinemaScreen::TogglePlay()
{
	if (Player && Player->IsPlaying())
	{
		Pause();
	}
	else
	{
		Play();
	}
}

void ASMHQCinemaScreen::Interact_Implementation(AActor* InteractInstigator)
{
	TogglePlay();
}

FText ASMHQCinemaScreen::GetPromptText_Implementation() const
{
	return SMHQStrings::ScreenPlayPause();
}

void ASMHQCinemaScreen::OnHallBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bAutoPlay && IsLocalPlayerPawn(OtherActor))
	{
		Play();
	}
}

void ASMHQCinemaScreen::OnHallEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (IsLocalPlayerPawn(OtherActor))
	{
		Pause();
	}
}

bool ASMHQCinemaScreen::IsLocalPlayerPawn(const AActor* Actor) const
{
	return Actor && Actor == UGameplayStatics::GetPlayerPawn(this, 0);
}
