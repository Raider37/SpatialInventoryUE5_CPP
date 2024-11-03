// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Styling/SlateBrush.h"
#include "DragWidget.generated.h"

class UItemObject;
class USizeBox;
class UImage;
struct FSlateBrush;

UCLASS()
class SURVIVAL_API UDragWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBoxDrag;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ImageDrag;

	UPROPERTY()
	UItemObject* ItemObject;

	UPROPERTY()
	float TileSize;

	void SetImageDragDropItem(FSlateBrush& Brush);

protected:

	virtual void NativeConstruct() override;

private:

	
};
