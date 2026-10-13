#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SMHQInteractable.h"
#include "SMHQLightSwitch.generated.h"

class ASMHQRoomLights;
class USoundBase;
class UStaticMesh;

/** Wall switch SW_<Floor>_<E##>_<NN>; toggles the room manager of the same space. */
UCLASS()
class SMHQ_VIRTUAL_API ASMHQLightSwitch : public AActor, public ISMHQInteractable
{
	GENERATED_BODY()

public:
	ASMHQLightSwitch();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Switch")
	TObjectPtr<UStaticMeshComponent> SwitchMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	TObjectPtr<ASMHQRoomLights> TargetRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Switch")
	TObjectPtr<USoundBase> ClickSound;

	UFUNCTION(BlueprintCallable, Category = "Switch")
	void ConfigureSwitch(UStaticMesh* Mesh, ASMHQRoomLights* Room);

	virtual void Interact_Implementation(AActor* InteractInstigator) override;
	virtual FText GetPromptText_Implementation() const override;
};
