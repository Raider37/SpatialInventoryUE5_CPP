// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Widgets/Layout/SBorder.h"
#include "InventoryGrid.generated.h"

class ARaiderCharacter;
class UInventoryComponent;
class UBorder;
class UGridPanel;
class USlotGrid;
class UBackgroundSlotGrid;

UCLASS()
class SURVIVAL_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadOnly, Category = "Camponents")
	UInventoryComponent* InvetoryComponent;
	UPROPERTY(BlueprintReadOnly, Category = "Camponents", meta = (BindWidget))
	TObjectPtr <UBorder> GridBorder;
	UPROPERTY(BlueprintReadOnly, Category = "Camponents", meta = (BindWidget))
	TObjectPtr <UGridPanel> GridPanel;
	UPROPERTY(BlueprintReadOnly, Category = "Camponents", meta = (BindWidget))
	TObjectPtr <UCanvasPanel> CanvasPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	bool bIsLootInventory;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<USlotGrid> ItemSlotClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UBackgroundSlotGrid> BackgroundSlotClass;

	UFUNCTION()
	void Initialization(UInventoryComponent* InvComp);
	UFUNCTION()
	void Refresh();
	UFUNCTION()
	void GenerateBackgroundSlots();

protected:

	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	float TileSize;
	UPROPERTY()
	ARaiderCharacter* Character;
	UPROPERTY()
	UInventoryComponent* InventoryComponent;
};
