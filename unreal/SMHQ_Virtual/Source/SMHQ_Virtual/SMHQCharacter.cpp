#include "SMHQCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "SMHQInfoPoint.h"
#include "SMHQInteractable.h"
#include "SMHQPlayerController.h"
#include "TimerManager.h"

ASMHQCharacter::ASMHQCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(34.f, 88.f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetCapsuleComponent());
	Camera->SetRelativeLocation(FVector(0.f, 0.f, EyeHeight - 88.f));
	Camera->bUsePawnControlRotation = true;
	Camera->SetFieldOfView(85.f);

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->MaxWalkSpeed = WalkSpeed;
	Movement->MaxStepHeight = 20.f;
	Movement->SetWalkableFloorAngle(45.f);
	Movement->JumpZVelocity = 0.f;
	Movement->BrakingDecelerationWalking = 2000.f;
}

void ASMHQCharacter::BeginPlay()
{
	Super::BeginPlay();
	Camera->SetRelativeLocation(FVector(0.f, 0.f, EyeHeight - GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetWorldTimerManager().SetTimer(FocusTimer, this, &ASMHQCharacter::UpdateFocus, 0.1f, true);
}

void ASMHQCharacter::BuildInput()
{
	if (InputContext)
	{
		return;
	}

	auto MakeAction = [this](const TCHAR* Name, EInputActionValueType Type)
	{
		UInputAction* Action = NewObject<UInputAction>(this, Name);
		Action->ValueType = Type;
		return Action;
	};

	MoveAction = MakeAction(TEXT("IA_SMHQ_Move"), EInputActionValueType::Axis2D);
	LookAction = MakeAction(TEXT("IA_SMHQ_Look"), EInputActionValueType::Axis2D);
	RunAction = MakeAction(TEXT("IA_SMHQ_Run"), EInputActionValueType::Boolean);
	InteractAction = MakeAction(TEXT("IA_SMHQ_Interact"), EInputActionValueType::Boolean);
	MenuAction = MakeAction(TEXT("IA_SMHQ_Menu"), EInputActionValueType::Boolean);
	BackAction = MakeAction(TEXT("IA_SMHQ_Back"), EInputActionValueType::Boolean);

	InputContext = NewObject<UInputMappingContext>(this, TEXT("IMC_SMHQ"));

	// Key -> 2D axis: forward keys swizzle X into Y; back/left keys negate.
	auto MapAxis = [this](UInputAction* Action, const FKey& Key, bool bSwizzle, bool bNegate)
	{
		FEnhancedActionKeyMapping& Mapping = InputContext->MapKey(Action, Key);
		if (bSwizzle)
		{
			UInputModifierSwizzleAxis* Swizzle = NewObject<UInputModifierSwizzleAxis>(InputContext);
			Swizzle->Order = EInputAxisSwizzle::YXZ;
			Mapping.Modifiers.Add(Swizzle);
		}
		if (bNegate)
		{
			Mapping.Modifiers.Add(NewObject<UInputModifierNegate>(InputContext));
		}
	};

	MapAxis(MoveAction, EKeys::W, true, false);
	MapAxis(MoveAction, EKeys::Up, true, false);
	MapAxis(MoveAction, EKeys::S, true, true);
	MapAxis(MoveAction, EKeys::Down, true, true);
	MapAxis(MoveAction, EKeys::D, false, false);
	MapAxis(MoveAction, EKeys::Right, false, false);
	MapAxis(MoveAction, EKeys::A, false, true);
	MapAxis(MoveAction, EKeys::Left, false, true);

	{
		FEnhancedActionKeyMapping& Mapping = InputContext->MapKey(LookAction, EKeys::Mouse2D);
		UInputModifierNegate* InvertY = NewObject<UInputModifierNegate>(InputContext);
		InvertY->bX = false;
		InvertY->bY = true;
		InvertY->bZ = false;
		Mapping.Modifiers.Add(InvertY);
	}

	InputContext->MapKey(RunAction, EKeys::LeftShift);
	InputContext->MapKey(InteractAction, EKeys::E);
	InputContext->MapKey(MenuAction, EKeys::M);
	InputContext->MapKey(MenuAction, EKeys::Tab);
	InputContext->MapKey(BackAction, EKeys::Escape);
}

void ASMHQCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	BuildInput();

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!Input)
	{
		return; // DefaultInput.ini must point to EnhancedInputComponent
	}
	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASMHQCharacter::Move);
	Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASMHQCharacter::Look);
	Input->BindAction(RunAction, ETriggerEvent::Started, this, &ASMHQCharacter::RunStart);
	Input->BindAction(RunAction, ETriggerEvent::Completed, this, &ASMHQCharacter::RunStop);
	Input->BindAction(InteractAction, ETriggerEvent::Started, this, &ASMHQCharacter::InteractPressed);
	Input->BindAction(MenuAction, ETriggerEvent::Started, this, &ASMHQCharacter::MenuPressed);
	Input->BindAction(BackAction, ETriggerEvent::Started, this, &ASMHQCharacter::BackPressed);
}

void ASMHQCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();
	BuildInput();

	if (const APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(InputContext, 0);
		}
	}
}

ASMHQPlayerController* ASMHQCharacter::GetSMHQController() const
{
	return Cast<ASMHQPlayerController>(GetController());
}

void ASMHQCharacter::Move(const FInputActionValue& Value)
{
	const ASMHQPlayerController* PC = GetSMHQController();
	if (PC && PC->IsUIBlockingMovement())
	{
		return;
	}
	const FVector2D Axis = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), Axis.Y);
	AddMovementInput(GetActorRightVector(), Axis.X);
}

void ASMHQCharacter::Look(const FInputActionValue& Value)
{
	const ASMHQPlayerController* PC = GetSMHQController();
	if (PC && PC->IsUIBlockingMovement())
	{
		return;
	}
	const FVector2D Axis = Value.Get<FVector2D>();
	AddControllerYawInput(Axis.X * LookSensitivity);
	AddControllerPitchInput(Axis.Y * LookSensitivity);
}

void ASMHQCharacter::RunStart()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void ASMHQCharacter::RunStop()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ASMHQCharacter::InteractPressed()
{
	ASMHQPlayerController* PC = GetSMHQController();
	if (PC && PC->IsInfoOpen())
	{
		PC->CloseInfoPanel();
		return;
	}
	if (PC && PC->IsMenuOpen())
	{
		return;
	}
	if (AActor* Target = FindFocusActor())
	{
		ISMHQInteractable::Execute_Interact(Target, this);
	}
}

void ASMHQCharacter::MenuPressed()
{
	if (ASMHQPlayerController* PC = GetSMHQController())
	{
		PC->ToggleMenu();
	}
}

void ASMHQCharacter::BackPressed()
{
	if (ASMHQPlayerController* PC = GetSMHQController())
	{
		PC->CloseOverlays();
	}
}

void ASMHQCharacter::UpdateFocus()
{
	ASMHQPlayerController* PC = GetSMHQController();
	if (!PC)
	{
		return;
	}
	FocusActor = PC->IsUIBlockingMovement() ? nullptr : FindFocusActor();
	PC->SetPrompt(FocusActor ? ISMHQInteractable::Execute_GetPromptText(FocusActor) : FText::GetEmpty());
}

AActor* ASMHQCharacter::FindFocusActor() const
{
	const FVector Start = Camera->GetComponentLocation();
	const FVector End = Start + Camera->GetForwardVector() * InteractDistance;

	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(SMHQInteract), false, this);
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor->Implements<USMHQInteractable>())
		{
			return HitActor;
		}
	}

	if (const ASMHQPlayerController* PC = GetSMHQController())
	{
		return PC->GetNearbyInfoPoint();
	}
	return nullptr;
}
