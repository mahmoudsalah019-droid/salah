#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SMHQInteractable.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USMHQInteractable : public UInterface
{
	GENERATED_BODY()
};

/** Anything the visitor can use with the Interact key (E): doors, switches, info points, screens. */
class SMHQ_VIRTUAL_API ISMHQInteractable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SMHQ|Interact")
	void Interact(AActor* InteractInstigator);

	/** Text shown at the bottom of the screen while this actor is in focus. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SMHQ|Interact")
	FText GetPromptText() const;
};
