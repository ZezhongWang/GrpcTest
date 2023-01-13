// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SGreeter/GreeterClient.h"
#include "TurboLinkDemoCppTest.generated.h"

class UGreeterService;
class UTimeService;

UCLASS()
class UTurboLinkDemoCppTest : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* GreeterServiceAddress;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* ConnectGreeterServiceButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* CreatGreeterClientButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UEditableText* GreeterRequestName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* CallGreeterButton;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* GreeterServiceStatus;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* GreeterResponse;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UEditableText* GreeterRequestWithLambdaName;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* CallGreeterWithLambdaButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* GreeterResponseWithLambda;
protected:
	UFUNCTION()
	void OnConnectGreeterServiceButtonClicked();

	UFUNCTION()
	void OnCreateGreeterClientButtonClicked();

	UFUNCTION()
	void OnCallGreeterButtonClicked();

	UFUNCTION()
	void OnGreeterServiceStateChanged(EGrpcServiceState NewState);

	UFUNCTION()
	void OnHelloResponse(FGrpcContextHandle Handle, const FGrpcResult& Result, const FGrpcGreeterHelloResponse& Response);

	UFUNCTION()
	void OnCallGreeterWithLambdaButtonClicked();

protected:
	UPROPERTY(BlueprintReadOnly)
	UGreeterService* GreeterService;

	UPROPERTY(BlueprintReadOnly)
	UGreeterServiceClient* GreeterServiceClient;

	FGrpcContextHandle CtxHello;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TimeServiceAddress;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* ConnectTimeServiceButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* CreateTimeClientButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TimeServiceStatus;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UEditableText* TicktokRequest;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* CallTicktokButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* TicktokResponse;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* InitWatchButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* CallWatchButton;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* WatchResponse;

protected:
	UFUNCTION()
	void OnConnectTimeServiceButtonClicked();

	UFUNCTION()
	void OnCreateTimeClientButtonClicked();

	UFUNCTION()
	void OnTimeServiceStateChanged(EGrpcServiceState NewState);

	UFUNCTION()
	void OnCallTicktokButtonClicked();

	UFUNCTION()
	void OnTicktokResponse(FGrpcContextHandle Handle, const FGrpcResult& Result, const FGrpcGreeterNowResponse& Response);

	UFUNCTION()
	void OnInitWatchButtonClicked();

	UFUNCTION()
	void OnCallWatchButtonClicked();

	UFUNCTION()
	void OnWatchResponse(FGrpcContextHandle Handle, const FGrpcResult& Result, const FGrpcGreeterNowResponse& Response);

	UFUNCTION()
	void OnTimeContextStateChanged(FGrpcContextHandle Handle, EGrpcContextState NewState);

protected:
	UPROPERTY(BlueprintReadOnly)
	UTimeService* TimeService;

	UPROPERTY(BlueprintReadOnly)
	UTimeServiceClient* TimeServiceClient;

	FGrpcContextHandle CtxTicktok;
	FGrpcContextHandle CtxWatch;
};
