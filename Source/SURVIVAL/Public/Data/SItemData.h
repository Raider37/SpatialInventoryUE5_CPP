// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SItemData.generated.h"

class UInventoryComponent;
class ABaseInventoryItem;
class UStaticMesh;
class UTexture2D;

USTRUCT(BlueprintType)
struct FSItemData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info") 
        FText Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info") 
        FText Desc;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info") 
        UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info") 
        bool bIsStack;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info") 
        int32 Amount;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info") 
        int32 MaxAmount;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info") 
        UStaticMesh* VisibleMesh;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info") 
        TSubclassOf<ABaseInventoryItem> UsableClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info") 
        FIntPoint Dimension;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Info") 
        bool bIsRotatable;

    FSItemData()
        : Name(), Desc(), Icon(nullptr), bIsStack(false), Amount(0), MaxAmount(1), VisibleMesh(nullptr), UsableClass(nullptr), Dimension(0, 0), bIsRotatable(false)
    {}
};