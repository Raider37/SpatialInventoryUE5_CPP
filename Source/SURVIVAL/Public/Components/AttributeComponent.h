// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

class ARaiderCharacter;

DECLARE_LOG_CATEGORY_EXTERN(AttributeComponent, Log, All);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVAL_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributeComponent();

	virtual void BeginPlay() override;

	UFUNCTION()
	void StartStamina();
	UFUNCTION()
	void StopStamina();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stamina")
	float Stamina;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
	float MaxStamina;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stamina")
	float UseStamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Strong")
	float Strong;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Strong")
	float MaxStrong;

	UPROPERTY(EditDefaultsOnly, Category = "Speed")
	float WalkSpeed;
	UPROPERTY(EditDefaultsOnly, Category = "Speed")
	float RunSpeed;

	UPROPERTY()
	FTimerHandle StaminaTimerHandle;
	FTimerHandle RegenStaminaTimerHandle;

private:

	ARaiderCharacter* Character;

	void CalculateStamina();
	void RegenStamina();

};
