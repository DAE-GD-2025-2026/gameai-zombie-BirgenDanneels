#include "BTS_Needs.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/HealthComponent.h"
#include "Common/InventoryComponent.h"
#include "Common/StaminaComponent.h"
#include "Survivor/SurvivorPawn.h"

UBTS_Needs::UBTS_Needs()
{
    NodeName = "Needs Service";
}

void UBTS_Needs::TickNode(UBehaviorTreeComponent& root, uint8* nodeMemory, float deltaSeconds)
{
    UBlackboardComponent* BB = root.GetBlackboardComponent();
    APawn* Pawn = root.GetAIOwner()->GetPawn();
    ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(Pawn);

    if (!BB || !Survivor) return;
    
    if (Survivor->GetComponentByClass<UHealthComponent>()->GetHealth() <= MinimumHealthThreshold)
        BB->SetValueAsBool("NeedHealing", true);
    else BB->SetValueAsBool("NeedHealing", false);
    
    if (Survivor->GetComponentByClass<UStaminaComponent>()->GetCurrentStamina() <= MinimumStaminaThreshold)
        BB->SetValueAsBool("NeedEat", true);
    else BB->SetValueAsBool("NeedEat", false);
    
    // Inventory needs
    const int AmmoNeed = GetAmmoNeed(Survivor);
    const int FoodStockNeed = GetFoodNeed(Survivor);
    const int MedkitStockNeed = GetMedkitNeed(Survivor);
    
    BB->SetValueAsInt("AmmoNeed", AmmoNeed);
    BB->SetValueAsInt("FoodStockNeed", FoodStockNeed);
    BB->SetValueAsInt("MedkitStockNeed", MedkitStockNeed);
    GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::Printf(TEXT("AmmoNeed: %d, FoodStockNeed: %d, MedkitStockNeed: %d"), AmmoNeed, FoodStockNeed, MedkitStockNeed));

    /*
    // if zombie threat exists (from your other service)
    if (BB->GetValueAsBool("bInDanger"))
    {
        Immediate = EImmediateNeed::Flee;
        ImmediateScore = 1.f;
    }
    */
}

// Improve this
float UBTS_Needs::GetAmmoNeed(ASurvivorPawn* Survivor)
{
    int TotalAmmo = 0;

    for (auto Item : Survivor->GetComponentByClass<UInventoryComponent>()->GetInventory())
    {
        if (!Item)
            continue;
        
        if (Item->GetItemType() == EItemType::Pistol ||
            Item->GetItemType() == EItemType::Shotgun)
        {
            TotalAmmo += Item->GetValue();
        }
    }

    return FMath::Clamp(1.f - (TotalAmmo / 10.f), 0.f, 1.f);
}

int UBTS_Needs::GetMedkitNeed(ASurvivorPawn* Survivor)
{
    int TotalMedkitsInInventory = 0;
    
    for (auto Item : Survivor->GetComponentByClass<UInventoryComponent>()->GetInventory())
    {
        if (!Item)
            continue;
        
        if (Item->GetItemType() == EItemType::Medkit)
        {
            ++TotalMedkitsInInventory;
        }
    }
    
    return  FMath::Clamp( MedkitNeedThreshold - TotalMedkitsInInventory, 0, MedkitNeedThreshold);
}

int UBTS_Needs::GetFoodNeed(ASurvivorPawn* Survivor)
{
    int TotalFoodInInventory = 0;
    
    for (auto Item : Survivor->GetComponentByClass<UInventoryComponent>()->GetInventory())
    {
        if (!Item)
            continue;
        
        if (Item->GetItemType() == EItemType::Food)
        {
            ++TotalFoodInInventory;
        }
    }
    
    return  FMath::Clamp( FoodNeedThreshold - TotalFoodInInventory, 0, FoodNeedThreshold);
}
