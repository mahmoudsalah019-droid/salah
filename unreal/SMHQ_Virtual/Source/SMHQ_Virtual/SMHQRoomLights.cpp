#include "SMHQRoomLights.h"

#include "Components/LightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "Engine/Light.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "SMHQ_Virtual.h"

ASMHQRoomLights::ASMHQRoomLights()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void ASMHQRoomLights::ConfigureRoom(FName InRoomCode)
{
	RoomCode = InRoomCode;
}

void ASMHQRoomLights::BeginPlay()
{
	Super::BeginPlay();
	CollectLights();
	SetLights(bStartOn);
}

void ASMHQRoomLights::CollectLights()
{
	LightComponents.Reset();
	FixtureMeshes.Reset();
	BaseIntensities.Reset();

	if (RoomCode.IsNone())
	{
		UE_LOG(LogSMHQ, Warning, TEXT("%s has no RoomCode"), *GetName());
		return;
	}

	TArray<AActor*> Tagged;
	UGameplayStatics::GetAllActorsWithTag(this, RoomCode, Tagged);
	for (AActor* Actor : Tagged)
	{
		const ALight* Light = Cast<ALight>(Actor);
		if (!Light || !Light->GetLightComponent())
		{
			continue;
		}
		ULightComponent* Comp = Light->GetLightComponent();
		LightComponents.Add(Comp);
		BaseIntensities.Add(Comp->Intensity);

		if (const AActor* Fixture = Light->GetAttachParentActor())
		{
			TArray<UStaticMeshComponent*> Meshes;
			Fixture->GetComponents<UStaticMeshComponent>(Meshes);
			for (UStaticMeshComponent* Mesh : Meshes)
			{
				FixtureMeshes.AddUnique(Mesh);
			}
		}
	}
	UE_LOG(LogSMHQ, Log, TEXT("Room %s: %d lights, %d fixture meshes"), *RoomCode.ToString(), LightComponents.Num(), FixtureMeshes.Num());
}

void ASMHQRoomLights::SetLights(bool bOn)
{
	bIsOn = bOn;
	Apply();
}

void ASMHQRoomLights::Toggle()
{
	SetLights(!bIsOn);
}

void ASMHQRoomLights::SetDimFactor(float Factor)
{
	DimFactor = FMath::Clamp(Factor, 0.f, 1.f);
	Apply();
}

void ASMHQRoomLights::Apply()
{
	for (int32 i = 0; i < LightComponents.Num(); ++i)
	{
		if (ULightComponent* Comp = LightComponents[i])
		{
			Comp->SetIntensity(BaseIntensities[i] * DimFactor);
			Comp->SetVisibility(bIsOn);
		}
	}
	const float Emissive = bIsOn ? DimFactor : 0.f;
	for (UStaticMeshComponent* Mesh : FixtureMeshes)
	{
		if (Mesh)
		{
			Mesh->SetScalarParameterValueOnMaterials(EmissiveParameter, Emissive);
		}
	}
}

void ASMHQRoomLights::SetAllRooms(const UObject* WorldContextObject, bool bOn)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		for (TActorIterator<ASMHQRoomLights> It(World); It; ++It)
		{
			It->SetLights(bOn);
		}
	}
}

void ASMHQRoomLights::RestoreAllRooms(const UObject* WorldContextObject)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		for (TActorIterator<ASMHQRoomLights> It(World); It; ++It)
		{
			It->SetLights(It->bStartOn);
		}
	}
}
