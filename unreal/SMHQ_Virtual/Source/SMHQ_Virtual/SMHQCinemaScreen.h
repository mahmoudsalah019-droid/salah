#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMHQInteractable.h"
#include "SMHQCinemaScreen.generated.h"

class ASMHQRoomLights;
class UBoxComponent;
class UMaterialInstanceDynamic;
class UMaterialInterface;
class UMediaPlayer;
class UMediaSoundComponent;
class UMediaSource;
class UMediaTexture;
class UStaticMesh;

/**
 * Auditorium screen. Plays a video while the visitor is inside HallTrigger and dims the hall lights.
 * Uses M_SMHQ_Screen (created by smhq_setup.py) with texture parameter "Video" and scalar "Brightness".
 */
UCLASS()
class SMHQ_VIRTUAL_API ASMHQCinemaScreen : public AActor, public ISMHQInteractable
{
	GENERATED_BODY()

public:
	ASMHQCinemaScreen();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Screen")
	TObjectPtr<UStaticMeshComponent> ScreenMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Screen")
	TObjectPtr<UBoxComponent> HallTrigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Screen")
	TObjectPtr<UMediaSoundComponent> Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen")
	TObjectPtr<UMediaSource> MediaSource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen")
	TSoftObjectPtr<UMaterialInterface> ScreenMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen")
	float Brightness = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen")
	bool bAutoPlay = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen")
	bool bLoop = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen")
	TObjectPtr<ASMHQRoomLights> DimRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Screen", meta = (ClampMin = "0", ClampMax = "1"))
	float DimFactor = 0.1f;

	/** HallExtent and HallOffset are in centimetres; offset is in world axes from the screen origin. */
	UFUNCTION(BlueprintCallable, Category = "Screen")
	void ConfigureScreen(UStaticMesh* Mesh, UMediaSource* InSource, bool bInAutoPlay, bool bInLoop,
		ASMHQRoomLights* InDimRoom, FVector HallExtent, FVector HallOffset);

	UFUNCTION(BlueprintCallable, Category = "Screen")
	void Play();

	UFUNCTION(BlueprintCallable, Category = "Screen")
	void Pause();

	UFUNCTION(BlueprintCallable, Category = "Screen")
	void TogglePlay();

	virtual void Interact_Implementation(AActor* InteractInstigator) override;
	virtual FText GetPromptText_Implementation() const override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHallBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnHallEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:
	bool IsLocalPlayerPawn(const AActor* Actor) const;

	UPROPERTY(Transient)
	TObjectPtr<UMediaPlayer> Player;

	UPROPERTY(Transient)
	TObjectPtr<UMediaTexture> VideoTexture;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> ScreenMID;

	bool bOpened = false;
};
