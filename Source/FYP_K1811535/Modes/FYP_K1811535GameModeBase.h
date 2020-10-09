#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FYP_K1811535GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FYP_K1811535_API AFYP_K1811535GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
private:
	void HandleGameStart();
	void HandleGameOver(bool PlayerWon);
public:
	void DeathHandle(AActor* DeadActor);
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void GameStart();
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool PlayerWon);
};
