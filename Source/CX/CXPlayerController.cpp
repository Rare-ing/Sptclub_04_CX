// CXPlayerController.cpp


#include "CXPlayerController.h"
#include "CXPawn.h"
#include "CXUI.h"
#include "EnhancedInputSubsystems.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "EngineUtils.h"
#include "GameState/CXGameState.h"
#include "PlayerState/CXPlayerState.h"
#include "SubSystem/ChatWorldSubsystem.h"

void ACXPlayerController::STCChatLog_Implementation(const TArray<FChatData>& ChatLog)
{
	if (UChatWorldSubsystem* ChatSubSystem = GetWorld()->GetSubsystem<UChatWorldSubsystem>())
	{
		ChatSubSystem->SetChatLog(ChatLog);
	}
	/*for (const FChatData& ChatData : ChatLog)
	{
		VehicleUI->OnNewChatMessage(ChatData.Sender, ChatData.Message);
	}*/
	
}

void ACXPlayerController::CTSChatMessage_Implementation(const FString& Message)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);

	const FString PlayerName = PlayerState->GetPlayerName();

	if (UChatWorldSubsystem* ChatSubSystem = GetWorld()->GetSubsystem<UChatWorldSubsystem>())
	{
		ChatSubSystem->AddChatMessage(PlayerName, Message);
	}

	for (TActorIterator<ACXPlayerController> It(GetWorld()); It; ++It)
	{
		if (ACXPlayerController* PC = *It)
		{
			PC->STCChatMessage(PlayerName, Message);
		}
	}
}

void ACXPlayerController::STCChatMessage_Implementation(const FString& Sender, const FString& Message)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *Sender, *Message);

	VehicleUI->OnNewChatMessage(Sender, Message);
}

void ACXPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}
	
	// spawn the UI widget and add it to the viewport
	VehicleUI = CreateWidget<UCXUI>(this, VehicleUIClass);

	check(VehicleUI);

	VehicleUI->AddToViewport();

	if (UChatWorldSubsystem* ChatSubSystem = GetWorld()->GetSubsystem<UChatWorldSubsystem>())
	{
		const TArray<FChatData> ChatLog = ChatSubSystem->GetChatLog();

		for (const FChatData& ChatData : ChatLog)
		{
			VehicleUI->OnNewChatMessage(ChatData.Sender, ChatData.Message);
		}
	}

}

void ACXPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);

		// optionally add the steering wheel context
		if (bUseSteeringWheelControls && SteeringWheelInputMappingContext)
		{
			Subsystem->AddMappingContext(SteeringWheelInputMappingContext, 1);
		}
	}
}

void ACXPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, VehiclePawn);
}

void ACXPlayerController::Tick(float Delta)
{
	Super::Tick(Delta);

	if (IsValid(VehiclePawn) && IsValid(VehicleUI))
	{
		VehicleUI->UpdateSpeed(VehiclePawn->GetChaosVehicleMovement()->GetForwardSpeed());
		VehicleUI->UpdateGear(VehiclePawn->GetChaosVehicleMovement()->GetCurrentGear());
	}

	if (!IsLocalController() || !IsValid(VehicleUI))
	{
		return;
	}

	ACXGameState* CXGameState =
		GetWorld()->GetGameState<ACXGameState>();

	if (!IsValid(CXGameState))
	{
		return;
	}

	const EGameState CurrentState =
		CXGameState->GetCurrentGameState();

	const int32 RemainingTime =
		CXGameState->GetRemainingTime();

	VehicleUI->UpdateGameStatus(CurrentState, RemainingTime);

	if (CurrentState == EGameState::Finished &&
		!bHasShownGameResult)
	{
		ACXPlayerState* MyPlayerState =
			GetPlayerState<ACXPlayerState>();

		if (IsValid(MyPlayerState))
		{
			const bool bIsWinner =
				CXGameState->GetWinners().Contains(MyPlayerState);

			VehicleUI->ShowGameResult(bIsWinner);

			bHasShownGameResult = true;
		}
	}
}

void ACXPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// get a pointer to the controlled pawn
	VehiclePawn = CastChecked<ACXPawn>(InPawn);
}
