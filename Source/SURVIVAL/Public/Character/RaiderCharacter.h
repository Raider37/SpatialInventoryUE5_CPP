// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/AttributeComponent.h"
#include "RaiderCharacter.generated.h"

class UInputComponent;
class UCameraComponent;
class UCapsuleComponent;
class USpringArmComponent;
class UAttributeComponent;
class UInventoryComponent;
class UInputMappingContext;
class UInputAction;
class UWorld;
class AActor;
class ARaiderPlayerController;
class UEnhancedInputComponent;
class IInteractInterface;
class UInventoryWidget;
struct FInputActionValue;

UCLASS()
class SURVIVAL_API ARaiderCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARaiderCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UAttributeComponent* AttributeComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	ARaiderPlayerController* PlayerController;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ToggleInventoryAction;

	UPROPERTY(BlueprintReadWrite, Category = "Interact")
	AActor* InteractActor;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void StartSprint();
	void StopSprint();

private:

	UPROPERTY()
	UInventoryWidget* InventoryInstance;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void InteractTrace();

	void StartInteract();
	void StopInteract();

	void ToggleEquipment();

};
