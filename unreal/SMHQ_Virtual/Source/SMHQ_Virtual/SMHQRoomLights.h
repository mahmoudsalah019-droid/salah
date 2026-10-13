#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMHQRoomLights.generated.h"

class ALight;
class ULightComponent;
class UStaticMeshComponent;

/**
 * Light manager for one space (e.g. GF_E01). Collects every light tagged with RoomCode
 * (the rebuild script tags lights from LT_<Floor>_<E##>_<KIND>_<NN>) and switches them together,
 * including the emissive glow of the fixture meshes they are attached to.
 */
UCLASS()
class SMHQ_VIRTUAL_API ASMHQRoomLights : public AActor
{
	GENERATED_BODY()

public:
	ASMHQRoomLights();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lights")
	FName RoomCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lights")
	bool bStartOn = true;

	/** Scalar parameter driven on fixture materials (0 = off). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lights")
	FName EmissiveParameter = TEXT("EmissiveStrength");

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Lights")
	bool bIsOn = false;

	UFUNCTION(BlueprintCallable, Category = "Lights")
	void ConfigureRoom(FName InRoomCode);

	UFUNCTION(BlueprintCallable, Category = "Lights")
	void SetLights(bool bOn);

	UFUNCTION(BlueprintCallable, Category = "Lights")
	void Toggle();

	/** 1 = normal, 0.1 = cinema dim. Keeps the on/off state. */
	UFUNCTION(BlueprintCallable, Category = "Lights")
	void SetDimFactor(float Factor);

	UFUNCTION(BlueprintCallable, Category = "Lights", meta = (WorldContext = "WorldContextObject"))
	static void SetAllRooms(const UObject* WorldContextObject, bool bOn);

	/** Back to each room's bStartOn (used when switching back to day). */
	UFUNCTION(BlueprintCallable, Category = "Lights", meta = (WorldContext = "WorldContextObject"))
	static void RestoreAllRooms(const UObject* WorldContextObject);

protected:
	virtual void BeginPlay() override;

private:
	void CollectLights();
	void Apply();

	UPROPERTY(Transient)
	TArray<TObjectPtr<ULightComponent>> LightComponents;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UStaticMeshComponent>> FixtureMeshes;

	TArray<float> BaseIntensities;
	float DimFactor = 1.f;
};
