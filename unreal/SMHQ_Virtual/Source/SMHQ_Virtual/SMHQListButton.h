#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "SMHQListButton.generated.h"

DECLARE_DELEGATE_OneParam(FSMHQIndexClicked, int32);

/** UButton that reports which entry was clicked (menu lists are built in code). */
UCLASS()
class SMHQ_VIRTUAL_API USMHQListButton : public UButton
{
	GENERATED_BODY()

public:
	void Setup(int32 InIndex);

	FSMHQIndexClicked OnIndexClicked;

private:
	UFUNCTION()
	void HandleClicked();

	int32 Index = INDEX_NONE;
};
