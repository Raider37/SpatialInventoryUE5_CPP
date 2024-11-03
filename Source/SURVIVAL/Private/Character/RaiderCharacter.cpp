// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RaiderCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "Player/RaiderPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InventoryComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"
#include "UI/InventoryGrid.h"
#include "UI/InventoryWidget.h"

#include "Engine/EngineTypes.h"

#include "Interface/InteractInterface.h"

ARaiderCharacter::ARaiderCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetCapsuleComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

    AttributeComponent = CreateDefaultSubobject<UAttributeComponent>("AttributeComponent");
    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");

	FCollisionResponseContainer CollisionResponse;
	CollisionResponse.SetAllChannels(ECR_Ignore); // ���������� ��� ������ �� ���������
	CollisionResponse.SetResponse(ECC_GameTraceChannel1, ECR_Block); // ��� ��������� �����

	GetCapsuleComponent()->SetCollisionResponseToChannels(CollisionResponse);
    
}

void ARaiderCharacter::BeginPlay()
{
	Super::BeginPlay();

    check(AttributeComponent);
    check(InventoryComponent);
    check(GetMovementComponent());

    InventoryInstance = CreateWidget<UInventoryWidget>(GetWorld(), InventoryComponent->InventoryWidget);
    PlayerController = Cast<ARaiderPlayerController>(Controller);
}

void ARaiderCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	InteractTrace();
}

void ARaiderCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//jump
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		//Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ARaiderCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ARaiderCharacter::StopSprint);
		//Move
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARaiderCharacter::Move);
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARaiderCharacter::Look);
        //Interact
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ARaiderCharacter::StartInteract);
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ARaiderCharacter::StopInteract);
        //ToggleInventory
        EnhancedInputComponent->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &ARaiderCharacter::ToggleEquipment);
	}
	else
	{
		//
	}
}

void ARaiderCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ARaiderCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ARaiderCharacter::StartSprint() 
{
   if (AttributeComponent)
   {
      AttributeComponent->StartStamina();
   }
}

void ARaiderCharacter::StopSprint() 
{
   if (AttributeComponent)
   {
      AttributeComponent->StopStamina();
   }
}

void ARaiderCharacter::InteractTrace()
{
   FVector Start = Camera->GetComponentLocation(); // ��������� ������� �����������
   FVector End = Start + (Camera->GetForwardVector() * 300.f); // �������� �������
   float Radius = 2.0f; // ������ �����
   FCollisionQueryParams CollisionParams;
   CollisionParams.AddIgnoredActor(this); // ���������� ������ ����

   FColor Color = FColor::Red;

   FHitResult HitResult;

   // ��������� �����������
   bool bHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(Radius), CollisionParams);

   AActor* HitActor = HitResult.GetActor(); // �������� ������

   // ���� ���-�� ���� �������
   if (bHit && HitActor && HitActor->Implements<UInteractInterface>())
   {
      IInteractInterface* InteractInterfaceActor = Cast<IInteractInterface>(HitActor);
      if (InteractInterfaceActor)
      {
         if (InteractActor == HitActor)
         {
            InteractInterfaceActor->ActiveOutline(true);
            //UKismetSystemLibrary::PrintString(GetWorld(), TEXT("true"));
            Color = FColor::Green;
         }
         else
         {
            if (InteractActor)
            {
               IInteractInterface* PreviousInteractActor = Cast<IInteractInterface>(InteractActor);
               if (PreviousInteractActor)
               {
                  PreviousInteractActor->ActiveOutline(false);
               }
            }

            InteractActor = HitActor; // ��������� ������ �� ��������� ������
            InteractInterfaceActor->ActiveOutline(true);
         }
      }
   }
   else
   {
      if (InteractActor)
      {
         IInteractInterface* PreviousInteractActor = Cast<IInteractInterface>(InteractActor);
         if (PreviousInteractActor)
         {
            PreviousInteractActor->ActiveOutline(false);
         }
         InteractActor = nullptr; // ���������� ������, ���� ������ ���
         Color = FColor::Red;
      }
   }

   // ��������� ����� ��� �������
   DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, Radius, 12, Color, false, 2.0f);
}

void ARaiderCharacter::StartInteract()
{
   if (InteractActor)
   {
      IInteractInterface* Interactable = Cast<IInteractInterface>(InteractActor);
      if (Interactable)
      {
         Interactable->StartInteract(this);
         UKismetSystemLibrary::PrintString(GetWorld(), TEXT("StartInteract"));
      }
   }
}

void ARaiderCharacter::StopInteract() 
{
   if (InteractActor)
   {
	   if (IInteractInterface* Interactable = Cast<IInteractInterface>(InteractActor))
      {
         Interactable->StopInteract();
         UKismetSystemLibrary::PrintString(GetWorld(), TEXT("StopInteract"));
      }
   }
}

void ARaiderCharacter::ToggleEquipment() 
{
    if (IsLocallyControlled())
    {
        if (InventoryInstance && InventoryInstance->IsInViewport())
        {
            InventoryInstance->RemoveFromParent();
            const FInputModeGameOnly GameOnly;
            PlayerController->bShowMouseCursor = false;
            PlayerController->SetInputMode(GameOnly);
        }
        else
        {
            if (InventoryInstance)
            {
                InventoryInstance->Grid->Initialization(InventoryComponent);

                InventoryInstance->AddToViewport();
                
                if (PlayerController)
                {
                    PlayerController->bShowMouseCursor = true;
                    FInputModeGameAndUI GameAndUI;
                    GameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
                    GameAndUI.SetWidgetToFocus(InventoryInstance->TakeWidget());
                	GameAndUI.SetHideCursorDuringCapture(true);
                	
                    PlayerController->SetInputMode(GameAndUI);
                }
            }
        }
    }
}

