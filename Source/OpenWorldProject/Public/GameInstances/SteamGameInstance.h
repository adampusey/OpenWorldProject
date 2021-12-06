// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Widgets/MainMenuWidget.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "SteamGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FServerInfo
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly)
	FString ServerName;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPlayers;
	UPROPERTY(BlueprintReadOnly)
	int32 MaxPlayers;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FServerDel, FServerInfo, ServerInfo);

/**
 * 
 */
UCLASS()
class OPENWORLDPROJECT_API USteamGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = MainMenu, meta = (DisplayName = "Join Server"))
	void JoinServer(const bool bEnableLan = false);

	UFUNCTION(BlueprintCallable, Category = MainMenu, meta = (DisplayName = "Host Server"))
	void HostServer(int NumberOfPlayers = 1, bool bEnableLan = false, FName ServerName = TEXT("Server Name"));

	UFUNCTION(BlueprintCallable, Category = MainMenu, meta = (DisplayName = "Destroy Session Caller"))
	void DestroySessionCaller();

protected:
	UPROPERTY(BlueprintAssignable)
	FServerDel ServerListDelegate;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	virtual void Init() override;
	virtual void OnCreateSessionComplete(FName ServerName, bool bSucceeded);
	virtual void OnFindSessionComplete(bool bSucceeded);
	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};
