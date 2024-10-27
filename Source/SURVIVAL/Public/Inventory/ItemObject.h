// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Data/SItemData.h"
#include "ItemObject.generated.h"

class UInventoryComponent;
class ABaseInventoryItem;
class UStaticMesh;
class UTexture2D;

struct FSItemData;


UCLASS()
class SURVIVAL_API UItemObject : public UObject
{
   GENERATED_BODY()

public:
   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Info")
   FSItemData ItemInfo;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
   bool bRotate;

   UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
   FDataTableRowHandle Data;
};
