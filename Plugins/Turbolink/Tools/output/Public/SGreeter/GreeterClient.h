//Generated by TurboLink CodeGenerator, do not edit!
#pragma once
#include "TurboLinkGrpcClient.h"
#include "SGreeter/GreeterMessage.h"
#include "GreeterClient.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnGreeterServiceHelloResponse, FGrpcContextHandle, Handle, const FGrpcResult&, Result, const FGrpcGreeterHelloResponse&, Response);

UCLASS(ClassGroup = TurboLink, BlueprintType)
class TURBOLINKGRPC_API UGreeterServiceClient : public UGrpcClient
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnGreeterServiceHelloResponse OnHelloResponse;

public:
	UFUNCTION(BlueprintCallable)
	FGrpcContextHandle InitHello();

	UFUNCTION(BlueprintCallable)
	void Hello(FGrpcContextHandle Handle, const FGrpcGreeterHelloRequest& Request);

public:
	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable)
	void TryCancel(FGrpcContextHandle Handle);
};

