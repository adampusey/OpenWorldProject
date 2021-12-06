// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstances/SteamGameInstance.h"

#include "Blueprint/UserWidget.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

void USteamGameInstance::Init()
{
	if (const IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get())
	{
		SessionInterface = SubSystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			// Bind delegates here
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &USteamGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &USteamGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &USteamGameInstance::OnJoinSessionComplete);
		}
	}
}

void USteamGameInstance::OnCreateSessionComplete(const FName ServerName, const bool bSucceeded)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete, bSucceeded: %d"), bSucceeded);
	if (bSucceeded)
	{
		GetWorld()->ServerTravel("/Game/_Maps/ThirdPersonExampleMap?listen");
	}
}

void USteamGameInstance::OnFindSessionComplete(const bool bSucceeded)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete, bSucceeded: %d"), bSucceeded);
	if (bSucceeded)
	{
		const TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
		for(auto Result : SearchResults)
		{
			if (!Result.IsValid())
				continue;

			FServerInfo Info;
			Info.ServerName = "Test Server Name";
			Info.MaxPlayers = Result.Session.SessionSettings.NumPublicConnections;
			Info.CurrentPlayers = Info.MaxPlayers - Result.Session.NumOpenPublicConnections;
			ServerListDelegate.Broadcast(Info);
		}

		UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete, Server Count: %d"), SearchResults.Num());
		if (SearchResults.Num())
		{
			//UE_LOG(LogTemp, Warning, TEXT("Joining session..."));
			//SessionInterface->JoinSession(0, "My Session", SearchResults[0]);
		}
	}
}

void USteamGameInstance::OnJoinSessionComplete(const FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete, SessionName: %s"), *SessionName.ToString());
	if (const auto PlayerController = GetPrimaryPlayerController())
	{
		FString JoinAddress = "";
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		if (JoinAddress != "")
		{
			PlayerController->ClientTravel(JoinAddress, TRAVEL_Absolute);
		}
	}
}

void USteamGameInstance::HostServer(const int NumberOfPlayers, const bool bEnableLan, const FName ServerName)
{
	UE_LOG(LogTemp, Warning, TEXT("HostServer"));

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = bEnableLan;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = NumberOfPlayers;

	SessionInterface->CreateSession(0, ServerName, SessionSettings);
}

void USteamGameInstance::JoinServer(const bool bEnableLan)
{
	UE_LOG(LogTemp, Warning, TEXT("JoinServer"));

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = bEnableLan;
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void USteamGameInstance::DestroySessionCaller()
{
}
