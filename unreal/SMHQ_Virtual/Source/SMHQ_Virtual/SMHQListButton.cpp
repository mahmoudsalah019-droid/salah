#include "SMHQListButton.h"

void USMHQListButton::Setup(int32 InIndex)
{
	Index = InIndex;
	OnClicked.AddUniqueDynamic(this, &USMHQListButton::HandleClicked);
}

void USMHQListButton::HandleClicked()
{
	OnIndexClicked.ExecuteIfBound(Index);
}
