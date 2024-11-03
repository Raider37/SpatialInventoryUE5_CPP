// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EqupmentComponent.generated.h"

class ARaiderCharacter;
class UInventoryComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVAL_API UEqupmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEqupmentComponent();

	UPROPERTY()
	UInventoryComponent* InventoryComponent;
	UPROPERTY()
	ARaiderCharacter* Character;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Initialization(ARaiderCharacter* CharacterOwner);

protected:
	virtual void BeginPlay() override;

private:

		
};
