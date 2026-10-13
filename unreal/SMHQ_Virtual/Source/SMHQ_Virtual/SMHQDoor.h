#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMHQInteractable.h"
#include "SMHQDoor.generated.h"

class UBoxComponent;
class USoundBase;
class UStaticMesh;

/**
 * One door leaf. The actor origin is the hinge (taken from the SketchUp component axes);
 * the leaf rotates (swing) or translates along local X (sliding).
 * Spawned by smhq_rebuild.py for every DR_<Floor>_<E##>_<D##>[_L|_R] in the model.
 */
UCLASS()
class SMHQ_VIRTUAL_API ASMHQDoor : public AActor, public ISMHQInteractable
{
	GENERATED_BODY()

public:
	ASMHQDoor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	TObjectPtr<USceneComponent> DoorPivot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	TObjectPtr<UStaticMeshComponent> DoorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Door")
	TObjectPtr<UBoxComponent> ProximityBox;

	/** Degrees around the hinge. Negative opens the other way. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float OpenAngle = 90.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bIsSliding = false;

	/** Centimetres along the leaf's local X axis. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	float SlideDistance = 100.f;

	/** Opens when the visitor walks into ProximityBox, closes when they leave. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bAutoOpen = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	bool bIsLocked = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door", meta = (ClampMin = "0.1"))
	float OpenTime = 1.2f;

	/** The other leaf of a double door (_L / _R); both move together. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door")
	TObjectPtr<ASMHQDoor> PairedLeaf;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Sound")
	TObjectPtr<USoundBase> OpenSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Sound")
	TObjectPtr<USoundBase> CloseSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Sound")
	TObjectPtr<USoundBase> LockedSound;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Door")
	bool bIsOpen = false;

	/** Called by the rebuild script after spawning. */
	UFUNCTION(BlueprintCallable, Category = "Door")
	void ConfigureDoor(UStaticMesh* LeafMesh, float InOpenAngle, bool bSliding, bool bInAutoOpen, bool bLocked);

	UFUNCTION(BlueprintCallable, Category = "Door")
	void SetPairedLeaf(ASMHQDoor* Other);

	UFUNCTION(BlueprintCallable, Category = "Door")
	void SetOpen(bool bOpen, bool bAffectPair = true);

	UFUNCTION(BlueprintCallable, Category = "Door")
	void ToggleOpen();

	virtual void Interact_Implementation(AActor* InteractInstigator) override;
	virtual FText GetPromptText_Implementation() const override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnProximityBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnProximityEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:
	void FitProximityBoxToMesh();
	void ApplyAlpha(float Eased);
	void PlayDoorSound(USoundBase* Sound) const;
	bool IsLocalPlayerPawn(const AActor* Actor) const;

	float Alpha = 0.f;
	float TargetAlpha = 0.f;
};
