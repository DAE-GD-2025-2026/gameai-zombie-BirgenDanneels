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
    const int PistolNeed = GetWeaponNeed(Survivor, EItemType::Pistol);
    const int ShotgunNeed = GetWeaponNeed(Survivor, EItemType::Shotgun);
    const int FoodStockNeed = GetFoodNeed(Survivor);
    const int MedkitStockNeed = GetMedkitNeed(Survivor);
    
    BB->SetValueAsInt("PistolNeed", PistolNeed);
    BB->SetValueAsInt("ShotgunNeed", ShotgunNeed);
    BB->SetValueAsInt("FoodStockNeed", FoodStockNeed);
    BB->SetValueAsInt("MedkitStockNeed", MedkitStockNeed);
}

// Improve this
int UBTS_Needs::GetWeaponNeed(ASurvivorPawn* Survivor, EItemType WeaponType)
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
