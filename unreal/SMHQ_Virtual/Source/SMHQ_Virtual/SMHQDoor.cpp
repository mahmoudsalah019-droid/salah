#include "SMHQDoor.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "SMHQStrings.h"

ASMHQDoor::ASMHQDoor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	DoorPivot = CreateDefaultSubobject<USceneComponent>(TEXT("DoorPivot"));
	RootComponent = DoorPivot;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetupAttachment(DoorPivot);
	DoorMesh->SetMobility(EComponentMobility::Movable);
	DoorMesh->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);

	ProximityBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ProximityBox"));
	ProximityBox->SetupAttachment(DoorPivot);
	ProximityBox->SetBoxExtent(FVector(150.f, 150.f, 110.f));
	ProximityBox->SetRelativeLocation(FVector(0.f, 0.f, 110.f));
	ProximityBox->SetCollisionProfileName(UCollisionProfile::CustomCollisionProfileName);
	ProximityBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ProximityBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	ProximityBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ProximityBox->SetGenerateOverlapEvents(true);
}

void ASMHQDoor::BeginPlay()
{
	Super::BeginPlay();
	ProximityBox->OnComponentBeginOverlap.AddDynamic(this, &ASMHQDoor::OnProximityBegin);
	ProximityBox->OnComponentEndOverlap.AddDynamic(this, &ASMHQDoor::OnProximityEnd);
	ApplyAlpha(0.f);
}

void ASMHQDoor::ConfigureDoor(UStaticMesh* LeafMesh, float InOpenAngle, bool bSliding, bool bInAutoOpen, bool bLocked)
{
	if (LeafMesh)
	{
		DoorMesh->SetStaticMesh(LeafMesh);
	}
	OpenAngle = InOpenAngle;
	bIsSliding = bSliding;
	bAutoOpen = bInAutoOpen;
	bIsLocked = bLocked;
	FitProximityBoxToMesh();
}

void ASMHQDoor::SetPairedLeaf(ASMHQDoor* Other)
{
	PairedLeaf = Other;
}

void ASMHQDoor::FitProximityBoxToMesh()
{
	const UStaticMesh* Mesh = DoorMesh->GetStaticMesh();
	if (!Mesh)
	{
		return;
	}
	const FBoxSphereBounds Bounds = Mesh->GetBounds();
	const float Reach = FMath::Max(Bounds.BoxExtent.X, Bounds.BoxExtent.Y) + 150.f;
	ProximityBox->SetRelativeLocation(Bounds.Origin);
	ProximityBox->SetBoxExtent(FVector(Reach, Reach, FMath::Max(Bounds.BoxExtent.Z, 100.f)));
}

void ASMHQDoor::SetOpen(bool bOpen, bool bAffectPair)
{
	if (bIsLocked)
	{
		PlayDoorSound(LockedSound);
		return;
	}
	if (bOpen == bIsOpen)
	{
		return;
	}

	bIsOpen = bOpen;
	TargetAlpha = bOpen ? 1.f : 0.f;
	SetActorTickEnabled(true);
	PlayDoorSound(bOpen ? OpenSound : CloseSound);

	if (bAffectPair && PairedLeaf && PairedLeaf != this)
	{
		PairedLeaf->SetOpen(bOpen, /*bAffectPair*/ false);
	}
}

void ASMHQDoor::ToggleOpen()
{
	SetOpen(!bIsOpen);
}

void ASMHQDoor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Alpha = FMath::FInterpConstantTo(Alpha, TargetAlpha, DeltaSeconds, 1.f / FMath::Max(OpenTime, 0.1f));
	ApplyAlpha(FMath::SmoothStep(0.f, 1.f, Alpha));

	if (FMath::IsNearlyEqual(Alpha, TargetAlpha, KINDA_SMALL_NUMBER))
	{
		Alpha = TargetAlpha;
		SetActorTickEnabled(false);
	}
}

void ASMHQDoor::ApplyAlpha(float Eased)
{
	if (bIsSliding)
	{
		DoorMesh->SetRelativeLocationAndRotation(FVector(SlideDistance * Eased, 0.f, 0.f), FRotator::ZeroRotator);
	}
	else
	{
		DoorMesh->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator(0.f, OpenAngle * Eased, 0.f));
	}
}

void ASMHQDoor::Interact_Implementation(AActor* InteractInstigator)
{
	ToggleOpen();
}

FText ASMHQDoor::GetPromptText_Implementation() const
{
	if (bIsLocked)
	{
		return SMHQStrings::DoorLocked();
	}
	return bIsOpen ? SMHQStrings::DoorClose() : SMHQStrings::DoorOpen();
}

void ASMHQDoor::OnProximityBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bAutoOpen && IsLocalPlayerPawn(OtherActor))
	{
		SetOpen(true);
	}
}

void ASMHQDoor::OnProximityEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (bAutoOpen && IsLocalPlayerPawn(OtherActor))
	{
		SetOpen(false);
	}
}

void ASMHQDoor::PlayDoorSound(USoundBase* Sound) const
{
	if (Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Sound, DoorMesh->GetComponentLocation());
	}
}

bool ASMHQDoor::IsLocalPlayerPawn(const AActor* Actor) const
{
	return Actor && Actor == UGameplayStatics::GetPlayerPawn(this, 0);
}
