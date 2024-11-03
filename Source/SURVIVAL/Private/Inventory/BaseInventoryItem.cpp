// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/BaseInventoryItem.h"
#include "Character/RaiderCharacter.h"
#include "Components/TimelineComponent.h"
#include "Components/InventoryComponent.h"
#include "Curves/CurveFloat.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Character/RaiderCharacter.h"
#include "Inventory/ItemObject.h"

// Sets default values
ABaseInventoryItem::ABaseInventoryItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(GetRootComponent());

	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
	ItemObject = nullptr;
	Character = nullptr;
	CurveFloat = nullptr;
}

// Called when the game starts or when spawned
void ABaseInventoryItem::BeginPlay()
{
	Super::BeginPlay();

	//Initializated timeline
	if (CurveFloat)
	{
		// Привязка делегата для интерполяции
		TimelineDelegate.BindUFunction(this, TEXT("TimelineCallback"));
		Timeline->AddInterpFloat(CurveFloat, TimelineDelegate);

		// Устанавливаем функцию, которая будет вызвана по завершении Timeline
		TimelineFinish.BindUFunction(this, TEXT("TimelineFinished"));
		Timeline->SetTimelineFinishedFunc(TimelineFinish);
	}
}



void ABaseInventoryItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (DataTableRow.DataTable)
	{
		FSItemData* Row = DataTableRow.DataTable->FindRow<FSItemData>(DataTableRow.RowName, TEXT("Context string"));

		if (Row)
		{
			ItemObject = NewObject<UItemObject>(this);
			ItemObject->ItemInfo = *Row;

			StaticMeshComponent->SetStaticMesh(Row->VisibleMesh);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Row not found in data table."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MyDataTable is null."));
	}
}

void ABaseInventoryItem::StartInteract(ARaiderCharacter* InteractedCharacter)
{
	StartTimeline();
	UE_LOG(LogTemp, Display, TEXT("StartInteractFunc"));

	Character = InteractedCharacter;

}

void ABaseInventoryItem::StopInteract()
{
	StopTimeline();
}

void ABaseInventoryItem::ActiveOutline(bool Active)
{
	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetRenderCustomDepth(Active);
	}
}

void ABaseInventoryItem::Interact()
{

	if (Character->InventoryComponent->CheckCanSplitItem(ItemObject))
	{
		UE_LOG(LogTemp, Display, TEXT("ActorSplit"));
	}
	else
	{
		if (Character->InventoryComponent->TryAddItem(ItemObject))
		{
			this->Destroy();
			UE_LOG(LogTemp, Display, TEXT("ActorDestoryed"));
		}
	}
}

// Called every frame
void ABaseInventoryItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseInventoryItem::TimelineCallback(float Value)
{
	UE_LOG(LogTemp, Display, TEXT("hhhhhh"), Value);
}

void ABaseInventoryItem::TimelineFinished()
{
	Interact();
	UE_LOG(LogTemp, Display, TEXT("InteractFunc"));
}

void ABaseInventoryItem::StartTimeline()
{
	if (Timeline)
	{
		Timeline->PlayFromStart();
	}
}

void ABaseInventoryItem::StopTimeline()
{
	if (Timeline)
	{
		Timeline->Stop();
	}
}

