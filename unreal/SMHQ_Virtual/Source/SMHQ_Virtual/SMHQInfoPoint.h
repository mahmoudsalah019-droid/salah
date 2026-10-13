#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMHQInteractable.h"
#include "SMHQInfoPoint.generated.h"

class UMediaPlayer;
class UMediaSoundComponent;
class UMediaSource;
class UMediaTexture;
class USphereComponent;
class UTextRenderComponent;
class UTexture2D;

/**
 * Info point above a maquette MQ-xx. Data (project code, title, images, video) comes from
 * Maquettes.csv through the rebuild script. Walking into the sphere shows the prompt; E opens the panel.
 */
UCLASS()
class SMHQ_VIRTUAL_API ASMHQInfoPoint : public AActor, public ISMHQInteractable
{
	GENERATED_BODY()

public:
	ASMHQInfoPoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	TObjectPtr<USphereComponent> Trigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	TObjectPtr<UTextRenderComponent> Marker;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	TObjectPtr<UMediaSoundComponent> VideoSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	FName MaquetteCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	FString ProjectCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	TArray<TObjectPtr<UTexture2D>> Images;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	TObjectPtr<UMediaSource> Video;

	UFUNCTION(BlueprintCallable, Category = "Info")
	void ConfigureInfo(FName InMaquetteCode, const FString& InProjectCode, const FText& InTitle, const FText& InDescription,
		const TArray<UTexture2D*>& InImages, UMediaSource* InVideo);

	/** Opens the video and returns the texture the info panel should show (nullptr if no video). */
	UMediaTexture* StartVideo();
	void StopVideo();

	virtual void Interact_Implementation(AActor* InteractInstigator) override;
	virtual FText GetPromptText_Implementation() const override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:
	UPROPERTY(Transient)
	TObjectPtr<UMediaPlayer> VideoPlayer;

	UPROPERTY(Transient)
	TObjectPtr<UMediaTexture> VideoTexture;

	float MarkerTime = 0.f;
	FVector MarkerBase = FVector::ZeroVector;
};
