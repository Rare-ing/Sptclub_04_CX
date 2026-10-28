// Fill out your copyright notice in the Description page of Project Settings.


#include "SubSystem/ChatWorldSubsystem.h"

void UChatWorldSubsystem::AddChatMessage(const FString& InSender, const FString& InMessage)
{
	if (ChatLog.Num() > MaxChatLogSize)
	{
		ChatLog.RemoveAt(0);
	}

	ChatLog.Emplace(InSender, InMessage);
}
