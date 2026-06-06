#include "BTS_Needs.h"
#include "AIController.h"
#include "NeedsComponent.h"
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
    AAIController* AIController = root.GetAIOwner();
    if (!AIController) return;

    ASurvivorPawn* Survivor = Cast<ASurvivorPawn>(AIController->GetPawn());
    if (!Survivor) return;

    UBlackboardComponent* BlackBoard = root.GetBlackboardComponent();
    if (!BlackBoard) return;

    UNeedsComponent* Needs = Survivor->FindComponentByClass<UNeedsComponent>();
    if (!Needs) return;

    Needs->RecalculateNeeds(Survivor, BlackBoard);
}