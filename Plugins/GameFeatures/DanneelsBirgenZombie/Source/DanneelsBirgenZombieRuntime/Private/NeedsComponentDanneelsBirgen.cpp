#include "NeedsComponentDanneelsBirgen.h"

#include "Common/InventoryComponent.h"
#include "Survivor/SurvivorPawn.h"

void UNeedsComponentDanneelsBirgen::RecalculateNeeds(const ASurvivorPawn* Survivor, UBlackboardComponent* BlackBoard) const
{
    if (!BlackBoard || !Survivor) return;
    
    // if (Survivor->GetComponentByClass<UHealthComponent>()->GetHealth() <= MinimumHealthThreshold)
    //     BlackBoard->SetValueAsBool("NeedHealing", true);
    // else BlackBoard->SetValueAsBool("NeedHealing", false);
    //
    // if (Survivor->GetComponentByClass<UStaminaComponent>()->GetCurrentStamina() <= MinimumStaminaThreshold)
    //     BlackBoard->SetValueAsBool("NeedEat", true);
    // else BlackBoard->SetValueAsBool("NeedEat", false);
    
    // Inventory needs
    const int PistolNeed = GetWeaponNeed(Survivor, EItemType::Pistol);
    const int ShotgunNeed = GetWeaponNeed(Survivor, EItemType::Shotgun);
    const int FoodStockNeed = GetFoodNeed(Survivor);
    const int MedkitStockNeed = GetMedkitNeed(Survivor);
    
    BlackBoard->SetValueAsInt("PistolNeed", PistolNeed);
    BlackBoard->SetValueAsInt("ShotgunNeed", ShotgunNeed);
    BlackBoard->SetValueAsInt("FoodStockNeed", FoodStockNeed);
    BlackBoard->SetValueAsInt("MedkitStockNeed", MedkitStockNeed);
}

// Improve this
int UNeedsComponentDanneelsBirgen::GetWeaponNeed(const ASurvivorPawn* Survivor, const EItemType WeaponType) const
{
    int TotalWeapons = 0;

    for (auto Item : Survivor->GetComponentByClass<UInventoryComponent>()->GetInventory())
    {
        if (!Item)
            continue;
        
        if (Item->GetItemType() == WeaponType)
        {
            ++TotalWeapons;
        }
    }

    switch (WeaponType)
    {
        case EItemType::Pistol:
            return FMath::Clamp(PistolNeedThreshold - TotalWeapons, 0.f, PistolNeedThreshold);
        case EItemType::Shotgun:
            return FMath::Clamp(ShotgunNeedThreshold - TotalWeapons, 0.f, ShotgunNeedThreshold);
        default:
            return 0.f;
    }
    
}

int UNeedsComponentDanneelsBirgen::GetMedkitNeed(const ASurvivorPawn* Survivor) const
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

int UNeedsComponentDanneelsBirgen::GetFoodNeed(const ASurvivorPawn* Survivor) const
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

