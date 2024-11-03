// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Interface/InteractInterface.h"
#include "Data/SItemData.h"
#include "BaseInventoryItem.generated.h"

class UInventoryComponent;
class UCurveFloat;
class UItemObject;
class UDataTable;
class IInteractInterface;
class ARaiderCharacter;
struct FSItemData;

UCLASS()
class SURVIVAL_API ABaseInventoryItem : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	ABaseInventoryItem();


	UPROPERTY()
	UStaticMeshComponent* StaticMeshComponent;

protected:

	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	//IInteractInterface
	virtual void StartInteract(ARaiderCharacter* InteractedCharacter) override;
	virtual void StopInteract() override;
	virtual void ActiveOutline(bool Active) override;

	void Interact();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	UItemObject* ItemObject;

	UPROPERTY(EditAnywhere, Category = "Data")
	FDataTableRowHandle DataTableRow;

private: 

	UFUNCTION()
	void TimelineCallback(float Value);

	UFUNCTION()
	void TimelineFinished();

	UFUNCTION(BlueprintCallable)
	void StartTimeline();

	UFUNCTION(BlueprintCallable)
	void StopTimeline();

	UPROPERTY()
	UTimelineComponent* Timeline;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* CurveFloat;

	UPROPERTY()
	ARaiderCharacter* Character;

	FOnTimelineFloat TimelineDelegate;
	FOnTimelineEvent TimelineFinish;
};
