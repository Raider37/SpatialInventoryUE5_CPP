// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/RaiderCharacter.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY(AttributeComponent);

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	WalkSpeed = 500.f;
	RunSpeed = 700.f;
	Stamina = 100.f;
	UseStamina = 3.f;
	MaxStamina = 100.f;

	

}

// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ARaiderCharacter>(GetOwner());
}

void UAttributeComponent::StartStamina()
{
	bool bIsFalling = Character->GetCharacterMovement()->IsFalling();
	bool bIsCrouching = Character->GetCharacterMovement()->IsCrouching();

	if (GetWorld() && !bIsFalling && !bIsCrouching)
	{
		if (RegenStaminaTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(RegenStaminaTimerHandle);
		}
		GetWorld()->GetTimerManager().SetTimer(StaminaTimerHandle, this, &UAttributeComponent::CalculateStamina, 0.1f, true);

		if (Character) // Убедитесь, что Character был успешно инициализирован
		{
			Character->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;  // Используйте метод для установки скорости
		}
	}
}

void UAttributeComponent::StopStamina()
{
	if (GetWorld() && StaminaTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);

		if (Character) // Убедитесь, что Character был успешно инициализирован
		{
			Character->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;  // Используйте метод для установки скорости
		}
	}
	if (Stamina < MaxStamina)
	{
		GetWorld()->GetTimerManager().SetTimer(RegenStaminaTimerHandle, this, &UAttributeComponent::RegenStamina, 0.1f, true);
	}
}

void UAttributeComponent::CalculateStamina()
{
	if (Stamina > UseStamina)
	{
		Stamina = FMath::Clamp(Stamina - UseStamina, 0.f, MaxStamina);
		//UE_LOG(AttributeComponent, Display, TEXT("Current Stamina: %f"), Stamina);
	}
	else
	{
		StopStamina();
		if (Character) // Убедитесь, что Character был успешно инициализирован
		{
			Character->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;  // Используйте метод для установки скорости
		}
	}
}

void UAttributeComponent::RegenStamina()
{
	if (Stamina < MaxStamina)
	{
		Stamina = FMath::Clamp(Stamina + UseStamina, 0.f, MaxStamina);
		//UE_LOG(AttributeComponent, Display, TEXT("Current Stamina: %f"), Stamina);
	}
	else
	{
		if (GetWorld() && RegenStaminaTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
		}
	}
}

