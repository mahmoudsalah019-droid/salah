#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SMHQCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/**
 * First-person visitor. Input actions and the mapping context are created in code at runtime
 * (no input assets needed): WASD/arrows walk, mouse looks, Shift runs, E interacts, M/Tab menu, Esc back.
 */
UCLASS()
class SMHQ_VIRTUAL_API ASMHQCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASMHQCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Visitor")
	TObjectPtr<UCameraComponent> Camera;

	/** Eye height above the floor, cm (capsule half height is 88). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visitor")
	float EyeHeight = 160.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visitor")
	float WalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visitor")
	float RunSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visitor")
	float LookSensitivity = 0.6f;

	/** How far the visitor can reach with E, cm. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visitor")
	float InteractDistance = 250.f;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PawnClientRestart() override;

protected:
	virtual void BeginPlay() override;

private:
	void BuildInput();
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void RunStart();
	void RunStop();
	void InteractPressed();
	void MenuPressed();
	void BackPressed();
	void UpdateFocus();
	AActor* FindFocusActor() const;
	class ASMHQPlayerController* GetSMHQController() const;

	UPROPERTY(Transient) TObjectPtr<UInputMappingContext> InputContext;
	UPROPERTY(Transient) TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(Transient) TObjectPtr<UInputAction> LookAction;
	UPROPERTY(Transient) TObjectPtr<UInputAction> RunAction;
	UPROPERTY(Transient) TObjectPtr<UInputAction> InteractAction;
	UPROPERTY(Transient) TObjectPtr<UInputAction> MenuAction;
	UPROPERTY(Transient) TObjectPtr<UInputAction> BackAction;

	UPROPERTY(Transient) TObjectPtr<AActor> FocusActor;

	FTimerHandle FocusTimer;
};
