// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/CXPlayerState.h"
#include "Net/UnrealNetwork.h"

static int32 GMyTestCoin = 0;

static FAutoConsoleVariableRef CVarAddCoinServer(
TEXT("cheat.addcoin.server"),
GMyTestCoin,
TEXT("코인을 입력한 수치만큼 증가"),
FConsoleVariableDelegate::CreateLambda([](IConsoleVariable* CVar)
	{
		if (CVar)
		{
			UE_LOG(LogTemp, Warning, TEXT("콘솔 변수 변경 값 : %d"), CVar->GetInt());
			UE_LOG(LogTemp, Warning, TEXT("콘솔 변수 변경 GMyTestCoin값 : %d"), GMyTestCoin);

			GWorld->GetFirstPlayerController()->GetPlayerState<ACXPlayerState>()->CTSAddCoin(GMyTestCoin);
			GMyTestCoin = 0;
		}
	}),
	ECVF_Cheat
);

static FAutoConsoleVariableRef CVarAddCoinClient(
	TEXT("cheat.addcoin.client"),
	GMyTestCoin,
	TEXT("코인을 입력한 수치만큼 증가"),
	FConsoleVariableDelegate::CreateLambda([](IConsoleVariable* CVar)
		{
			if (CVar)
			{
				UE_LOG(LogTemp, Warning, TEXT("콘솔 변수 변경 값 : %d"), CVar->GetInt());
				UE_LOG(LogTemp, Warning, TEXT("콘솔 변수 변경 GMyTestCoin값 : %d"), GMyTestCoin);

				GWorld->GetFirstPlayerController()->GetPlayerState<ACXPlayerState>()->Coins += GMyTestCoin;
				GMyTestCoin = 0;
			}
		}),
	ECVF_Cheat
);

void ACXPlayerState::CTSAddCoin_Implementation(const int32& Amount)
{
	AddCoins(Amount);
}

void ACXPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, Coins, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(ThisClass, BonusMaxSpeed, COND_OwnerOnly);
}

void ACXPlayerState::AddCoins(int32 Amount)
{
	if(HasAuthority() == false || Amount <= 0)
	{
		return;
	}

	FString PlayerName = GetPlayerName();

	UE_LOG(LogTemp, Warning, TEXT("%s - AddCoins : %d(+%d)"),*PlayerName, Coins, Amount);
	Coins += Amount;
}

bool ACXPlayerState::TryRemoveCoins(int32 Amount)
{
	if (HasAuthority() == false || Amount <= 0 || Coins < 5)
	{
		return false;
	}
	Coins -= Amount;
	return true;
}

void ACXPlayerState::AddBonusMaxSpeed(float Amount)
{

	if (HasAuthority() == false || Amount <= 0)
	{
		return;
	}

	BonusMaxSpeed += Amount;
}

FString ACXPlayerState::GetPlayerStateInfo() const
{
	return FString::Printf(TEXT("Coins: %d, BonusMaxSpeed: %f"), Coins, BonusMaxSpeed);
}
