// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/EqupmentComponent.h"
#include "Character/RaiderCharacter.h"
#include "Components/InventoryComponent.h"


UEqupmentComponent::UEqupmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UEqupmentComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UEqupmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UEqupmentComponent::Initialization(ARaiderCharacter* CharacterOwner)
{
	if (CharacterOwner)
	{
		Character = CharacterOwner;
		InventoryComponent = Character->InventoryComponent;
	}
}

