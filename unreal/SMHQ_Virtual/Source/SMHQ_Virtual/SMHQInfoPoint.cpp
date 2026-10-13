#include "SMHQInfoPoint.h"

#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MediaPlayer.h"
#include "MediaSoundComponent.h"
#include "MediaSource.h"
#include "MediaTexture.h"
#include "SMHQPlayerController.h"
#include "SMHQStrings.h"

ASMHQInfoPoint::ASMHQInfoPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	Trigger->InitSphereRadius(200.f);
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Trigger->SetGenerateOverlapEvents(true);
	RootComponent = Trigger;

	Marker = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Marker"));
	Marker->SetupAttachment(Trigger);
	Marker->SetText(FText::FromString(TEXT("i")));
	Marker->SetWorldSize(40.f);
	Marker->SetHorizontalAlignment(EHTA_Center);
	Marker->SetVerticalAlignment(EVRTA_TextCenter);
	Marker->SetTextRenderColor(FColor::White);
	Marker->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	VideoSound = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("VideoSound"));
	VideoSound->SetupAttachment(Trigger);
}

void ASMHQInfoPoint::BeginPlay()
{
	Super::BeginPlay();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASMHQInfoPoint::OnTriggerBegin);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ASMHQInfoPoint::OnTriggerEnd);
	MarkerBase = Marker->GetRelativeLocation();
}

void ASMHQInfoPoint::ConfigureInfo(FName InMaquetteCode, const FString& InProjectCode, const FText& InTitle,
	const FText& InDescription, const TArray<UTexture2D*>& InImages, UMediaSource* InVideo)
{
	MaquetteCode = InMaquetteCode;
	ProjectCode = InProjectCode;
	Title = InTitle;
	Description = InDescription;
	Images.Reset();
	for (UTexture2D* Image : InImages)
	{
		if (Image)
		{
			Images.Add(Image);
		}
	}
	Video = InVideo;
}

void ASMHQInfoPoint::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	MarkerTime += DeltaSeconds;
	Marker->SetRelativeLocation(MarkerBase + FVector(0.f, 0.f, 5.f * FMath::Sin(MarkerTime * 2.f)));

	if (const APlayerCameraManager* Camera = UGameplayStatics::GetPlayerCameraManager(this, 0))
	{
		const FVector ToCamera = Camera->GetCameraLocation() - Marker->GetComponentLocation();
		Marker->SetWorldRotation(FRotator(0.f, ToCamera.Rotation().Yaw, 0.f));
	}
}

UMediaTexture* ASMHQInfoPoint::StartVideo()
{
	if (!Video)
	{
		return nullptr;
	}
	if (!VideoPlayer)
	{
		VideoPlayer = NewObject<UMediaPlayer>(this);
		VideoTexture = NewObject<UMediaTexture>(this);
		VideoTexture->SetMediaPlayer(VideoPlayer);
		VideoTexture->UpdateResource();
		VideoSound->SetMediaPlayer(VideoPlayer);
	}
	VideoPlayer->PlayOnOpen = true;
	VideoPlayer->OpenSource(Video);
	if (!VideoSound->IsPlaying())
	{
		VideoSound->Start();
	}
	return VideoTexture;
}

void ASMHQInfoPoint::StopVideo()
{
	if (VideoPlayer)
	{
		VideoPlayer->Close();
	}
	if (VideoSound && VideoSound->IsPlaying())
	{
		VideoSound->Stop();
	}
}

void ASMHQInfoPoint::Interact_Implementation(AActor* InteractInstigator)
{
	if (ASMHQPlayerController* PC = Cast<ASMHQPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		PC->OpenInfoPanel(this);
	}
}

FText ASMHQInfoPoint::GetPromptText_Implementation() const
{
	return SMHQStrings::InfoPoint();
}

void ASMHQInfoPoint::OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor == UGameplayStatics::GetPlayerPawn(this, 0))
	{
		if (ASMHQPlayerController* PC = Cast<ASMHQPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
		{
			PC->SetNearbyInfoPoint(this, true);
		}
	}
}

void ASMHQInfoPoint::OnTriggerEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor == UGameplayStatics::GetPlayerPawn(this, 0))
	{
		if (ASMHQPlayerController* PC = Cast<ASMHQPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
		{
			PC->SetNearbyInfoPoint(this, false);
		}
	}
}
