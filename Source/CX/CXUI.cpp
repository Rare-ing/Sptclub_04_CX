// Copyright Epic Games, Inc. All Rights Reserved.


#include "CXUI.h"

#include "UI/ChatUI.h"


void UCXUI::UpdateGameState(EGameState NewState)
{

    /*if (!IsValid(Text_GameState))
    {
        return;
    }*/

    FString StateText;

    switch (NewState)
    {
    case EGameState::Waiting:
        StateText = TEXT("waiting");
        break;

    case EGameState::Playing:
        StateText = TEXT("playing");
        break;

    case EGameState::Finished:
        StateText = TEXT("finished");
        break;

    default:
        StateText = TEXT("error");
        break;
    }

    //Text_GameState->SetText(FText::FromString(StateText));
}

void UCXUI::UpdateRemainingTime(int32 NewTime)
{
    /*if (!IsValid(Text_RemainingTime))
    {
        return;
    }*/

    const int32 Minutes = NewTime / 60;
    const int32 Seconds = NewTime % 60;

    const FString TimeText = FString::Printf(
        TEXT("%02d:%02d"),
        Minutes,
        Seconds
    );

    //Text_RemainingTime->SetText(FText::FromString(TimeText));
}

void UCXUI::OnNewChatMessage(const FString& InSender, const FString& InMessage)
{
	UI_Chat->OnNewChatMessage(InSender, InMessage);
}

void UCXUI::UpdateSpeed(float NewSpeed)
{
	// format the speed to KPH or MPH
	float FormattedSpeed = FMath::Abs(NewSpeed) * (bIsMPH ? 0.022f : 0.036f);

	// call the Blueprint handler
	OnSpeedUpdate(FormattedSpeed);
}

void UCXUI::UpdateGear(int32 NewGear)
{
	// call the Blueprint handler
	OnGearUpdate(NewGear);
}

void UCXUI::UpdateGameStatus(
    EGameState NewState,
    int32 NewRemainingTime)
{
    OnGameStatusUpdate(NewState, NewRemainingTime);
}

void UCXUI::ShowGameResult(bool bIsWinner)
{
    OnGameResult(bIsWinner);
}