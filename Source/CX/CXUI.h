// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameState/CXGameState.h"
#include "CXUI.generated.h"

class UChatUI;
class UTextBlock;

/**
 *  Simple Vehicle HUD class
 *  Displays the current speed and gear.
 *  Widget setup is handled in a Blueprint subclass.
 */
UCLASS(abstract)
class CX_API UCXUI : public UUserWidget
{
	GENERATED_BODY()
	

public:

	UFUNCTION(BlueprintCallable)
	void UpdateGameState(EGameState NewState);

	UFUNCTION(BlueprintCallable)
	void UpdateRemainingTime(int32 NewTime);

protected:

	/** Controls the display of speed in Km/h or MPH */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Vehicle)
	bool bIsMPH = false;

#pragma region ChatUI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UChatUI> UI_Chat;

public:
	void OnNewChatMessage(const FString& InSender, const FString& InMessage);
#pragma endregion

public:
	void UpdateGameStatus(EGameState NewState, int32 NewRemainingTime);

	void ShowGameResult(bool bIsWinner);


protected:
	UFUNCTION(BlueprintImplementableEvent, Category = "Game")
	void OnGameStatusUpdate(EGameState NewState, int32 NewRemainingTime);

	UFUNCTION(BlueprintImplementableEvent, Category = "Game")
	void OnGameResult(bool bIsWinner);

public:

	/** Called to update the speed display */
	void UpdateSpeed(float NewSpeed);

	/** Called to update the gear display */
	void UpdateGear(int32 NewGear);

protected:

	/** Implemented in Blueprint to display the new speed */
	UFUNCTION(BlueprintImplementableEvent, Category = Vehicle)
	void OnSpeedUpdate(float NewSpeed);

	/** Implemented in Blueprint to display the new gear */
	UFUNCTION(BlueprintImplementableEvent, Category = Vehicle)
	void OnGearUpdate(int32 NewGear);
};
