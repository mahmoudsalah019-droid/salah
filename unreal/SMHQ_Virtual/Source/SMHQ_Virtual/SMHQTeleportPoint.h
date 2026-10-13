#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMHQTeleportPoint.generated.h"

class UArrowComponent;

/** Quick-travel destination, spawned from Spaces.csv rows with Teleport = Yes. Location = floor level. */
UCLASS()
class SMHQ_VIRTUAL_API ASMHQTeleportPoint : public AActor
{
	GENERATED_BODY()

public:
	ASMHQTeleportPoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport")
	FName SpaceCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport")
	FText DisplayNameAR;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport")
	FText DisplayNameEN;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport")
	int32 SortOrder = 0;

	UFUNCTION(BlueprintCallable, Category = "Teleport")
	void ConfigureTeleport(FName InSpaceCode, const FText& InNameAR, const FText& InNameEN, int32 InSortOrder);

	UFUNCTION(BlueprintPure, Category = "Teleport")
	FText GetMenuName() const;

#if WITH_EDITORONLY_DATA
private:
	UPROPERTY()
	TObjectPtr<UArrowComponent> Arrow;
#endif
};
