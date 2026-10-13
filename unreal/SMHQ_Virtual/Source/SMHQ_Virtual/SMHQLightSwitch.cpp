#include "SMHQLightSwitch.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Kismet/GameplayStatics.h"
#include "SMHQRoomLights.h"
#include "SMHQStrings.h"

ASMHQLightSwitch::ASMHQLightSwitch()
{
	PrimaryActorTick.bCanEverTick = false;
	SwitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchMesh"));
	SwitchMesh->SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
	RootComponent = SwitchMesh;
}

void ASMHQLightSwitch::ConfigureSwitch(UStaticMesh* Mesh, ASMHQRoomLights* Room)
{
	if (Mesh)
	{
		SwitchMesh->SetStaticMesh(Mesh);
	}
	TargetRoom = Room;
}

void ASMHQLightSwitch::Interact_Implementation(AActor* InteractInstigator)
{
	if (TargetRoom)
	{
		TargetRoom->Toggle();
	}
	if (ClickSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ClickSound, GetActorLocation());
	}
}

FText ASMHQLightSwitch::GetPromptText_Implementation() const
{
	return SMHQStrings::LightSwitch();
}
