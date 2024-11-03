#include "UI/SlotGrid.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Inventory/ItemObject.h"
#include "UI/DragWidget.h"
#include "UI/ItemDragDropOperation.h"
#include "Components/InventoryComponent.h"

void USlotGrid::NativeConstruct()
{
	Super::NativeConstruct();
	UE_LOG(LogTemp, Warning, TEXT("ItemObject"));
	Refresh();
}

FReply USlotGrid::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	// submenu on right click will happen here

	return Reply.Unhandled();
}

void USlotGrid::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (DragWidgetClass)
	{
		const TObjectPtr<UDragWidget> DragVisual = CreateWidget<UDragWidget>(this, DragWidgetClass);
		DragVisual->ImageDrag->SetBrushFromTexture(ItemObject->ItemInfo.Icon);
		DragVisual->SizeBoxDrag->SetHeightOverride(TileSize* ItemObject->ItemInfo.Dimension.Y);
		DragVisual->SizeBoxDrag->SetWidthOverride(TileSize* ItemObject->ItemInfo.Dimension.X);
		

		UItemDragDropOperation* DragItemOperation = NewObject<UItemDragDropOperation>();
		DragItemOperation->Payload = ItemObject;
		DragItemOperation->InventoryComp = InventoryComponent;

		DragItemOperation->DefaultDragVisual = DragVisual;
		DragItemOperation->Pivot = EDragPivot::CenterCenter;

		this->RemoveFromParent();
		OnRemoved.Broadcast(ItemObject);
		OutOperation = DragItemOperation;
	}
}

void USlotGrid::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
}

bool USlotGrid::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (!Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation))
	{
		return false;
	}

	if (UItemObject* Item = CastChecked<UItemObject>(InOperation->Payload))
	{
		if (InventoryComponent && InventoryComponent->AddStackToSlot(Item, ItemObject))
		{
			return true;
		}
	}
	return false;
}

void USlotGrid::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
}

void USlotGrid::Refresh() const
{
	if (SlotSizeBox && ItemObject)
	{
		SlotSizeBox->SetWidthOverride(ItemObject->ItemInfo.Dimension.X * TileSize);
		SlotSizeBox->SetHeightOverride(ItemObject->ItemInfo.Dimension.Y * TileSize);

		UMaterialInstanceDynamic* Image = ImageIcon->GetDynamicMaterial();
		Image->SetTextureParameterValue("Icon", ItemObject->ItemInfo.Icon);

		const float RotationAngle = ItemObject->ItemInfo.bIsRotatable ? -0.25f : 0.0f;
		Image->SetScalarParameterValue("RotationAngle(0 - 1)", RotationAngle);
	}
}
