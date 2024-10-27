// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

class ARaiderCharacter;

UINTERFACE(Blueprintable, BlueprintType)
class SURVIVAL_API UInteractInterface : public UInterface
{
   GENERATED_BODY()
};

class SURVIVAL_API IInteractInterface
{
   GENERATED_BODY()

public:
   virtual void ActiveOutline(bool Active);
   virtual void StartInteract(ARaiderCharacter* Charcter);
   virtual void StopInteract();
   virtual void GetName(FText& Name);
   virtual void GetActionType(FText& Action);
};
