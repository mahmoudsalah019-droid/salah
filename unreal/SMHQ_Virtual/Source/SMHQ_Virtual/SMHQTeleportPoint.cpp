#include "SMHQTeleportPoint.h"

#include "Components/ArrowComponent.h"

ASMHQTeleportPoint::ASMHQTeleportPoint()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
#if WITH_EDITORONLY_DATA
	Arrow = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	if (Arrow)
	{
		Arrow->SetupAttachment(RootComponent);
		Arrow->SetRelativeLocation(FVector(0.f, 0.f, 160.f));
	}
#endif
}

void ASMHQTeleportPoint::ConfigureTeleport(FName InSpaceCode, const FText& InNameAR, const FText& InNameEN, int32 InSortOrder)
{
	SpaceCode = InSpaceCode;
	DisplayNameAR = InNameAR;
	DisplayNameEN = InNameEN;
	SortOrder = InSortOrder;
}

FText ASMHQTeleportPoint::GetMenuName() const
{
	if (!DisplayNameAR.IsEmpty())
	{
		return DisplayNameAR;
	}
	if (!DisplayNameEN.IsEmpty())
	{
		return DisplayNameEN;
	}
	return FText::FromName(SpaceCode);
}
