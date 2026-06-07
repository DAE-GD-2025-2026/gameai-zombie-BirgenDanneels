#include "BTS_NeedsDanneelsBirgen.h"
#include "AIController.h"
#include "NeedsComponentDanneelsBirgen.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Common/HealthComponent.h"
#include "Common/InventoryComponent.h"
#include "Common/StaminaComponent.h"
#include "Survivor/SurvivorPawn.h"

UBTS_NeedsDanneelsBirgen::UBTS_NeedsDanneelsBirgen()
{
    NodeName = "Needs Service";
}

void UBTS_NeedsDanneelsBirgen::TickNode(UBehaviorTreeComponent& root, uint8* nodeMemory, float deltaSeconds)
{
    AAIController* AIController = root.GetAIOwner();
    if (!AIController) return;

    ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
    if (!Survivor) return;

    UBlackboardComponent* BlackBoard = root.GetBlackboardComponent();
    if (!BlackBoard) return;

    UNeedsComponentDanneelsBirgen* Needs = Survivor->FindComponentByClass<UNeedsComponentDanneelsBirgen>();
    if (!Needs) return;

    Needs->RecalculateNeeds(Survivor, BlackBoard);
}