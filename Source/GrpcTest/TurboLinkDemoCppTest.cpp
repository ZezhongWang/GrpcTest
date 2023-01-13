// Fill out your copyright notice in the Description page of Project Settings.


#include "TurboLinkDemoCppTest.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"

#include "TurboLinkGrpcManager.h"
#include "TurboLinkGrpcUtilities.h"
#include "TurboLinkGrpcConfig.h"

#include "SGreeter/GreeterService.h"

void UTurboLinkDemoCppTest::NativeConstruct()
{
	Super::NativeConstruct();

    FString greeterServiceEndPoint = UTurboLinkGrpcUtilities::GetTurboLinkGrpcConfig()->GetServiceEndPoint(TEXT("GreeterService"));
    GreeterServiceAddress->SetText(FText::FromString(FString::Printf(TEXT("(%s)"), *greeterServiceEndPoint)));

    ConnectGreeterServiceButton->OnClicked.AddDynamic(this, &UTurboLinkDemoCppTest::OnConnectGreeterServiceButtonClicked);
    CreatGreeterClientButton->OnClicked.AddDynamic(this, &UTurboLinkDemoCppTest::OnCreateGreeterClientButtonClicked);
    CallGreeterButton->OnClicked.AddDynamic(this, &UTurboLinkDemoCppTest::OnCallGreeterButtonClicked);
    CallGreeterWithLambdaButton->OnClicked.AddDynamic(this, &UTurboLinkDemoCppTest::OnCallGreeterWithLambdaButtonClicked);

    FString timeServiceEndPoint = UTurboLinkGrpcUtilities::GetTurboLinkGrpcConfig()->GetServiceEndPoint(TEXT("TimeService"));
    TimeServiceAddress->SetText(FText::FromString(FString::Printf(TEXT("(%s)"), *timeServiceEndPoint)));

    ConnectTimeServiceButton->OnClicked.AddDynamic(this, &UTurboLinkDemoCppTest::OnConnectTimeServiceButtonClicked);
    CreateTimeClientButton->OnClicked.AddDynamic(this, &UTurboLinkDemoCppTest::OnCreateTimeClientButtonClicked);
    CallTicktokButton->OnClicked.AddDynamic(this, &UTurboLinkDemoCppTest::OnCallTicktokButtonClicked);
    InitWatchButton->OnClicked.AddDynamic(this, &UTurboLinkDemoCppTest::OnInitWatchButtonClicked);
    CallWatchButton->OnClicked.AddDynamic(this, &UTurboLinkDemoCppTest::OnCallWatchButtonClicked);

    CreatGreeterClientButton->SetIsEnabled(false);
    CallGreeterButton->SetIsEnabled(false);
    CallGreeterWithLambdaButton->SetIsEnabled(false);

    CreateTimeClientButton->SetIsEnabled(false);
    CallTicktokButton->SetIsEnabled(false);
    InitWatchButton->SetIsEnabled(false);
    CallWatchButton->SetIsEnabled(false);
}

void UTurboLinkDemoCppTest::OnConnectGreeterServiceButtonClicked()
{
    if (GreeterService != nullptr) return;
    UTurboLinkGrpcManager* turboLinkManager = UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager();

    //create service adapter
    GreeterService = Cast<UGreeterService>(turboLinkManager->MakeService("GreeterService"));
    if (GreeterService == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Can't find greeter service!"));
        return;
    }
    GreeterService->OnServiceStateChanged.AddUniqueDynamic(this, &UTurboLinkDemoCppTest::OnGreeterServiceStateChanged);
    GreeterService->Connect();

    ConnectGreeterServiceButton->SetIsEnabled(false);
}

void UTurboLinkDemoCppTest::OnCreateGreeterClientButtonClicked()
{
    if (GreeterService != nullptr && 
        GreeterService->GetServiceState() == EGrpcServiceState::Ready &&
        GreeterServiceClient == nullptr)
    {
        //create client
        GreeterServiceClient = GreeterService->MakeClient();
        GreeterServiceClient->OnHelloResponse.AddUniqueDynamic(this, &UTurboLinkDemoCppTest::OnHelloResponse);

        CallGreeterButton->SetIsEnabled(true);
        CreatGreeterClientButton->SetIsEnabled(false);
    }
}

void UTurboLinkDemoCppTest::OnCallGreeterButtonClicked()
{
    if (GreeterService == nullptr ||
        (GreeterService->GetServiceState() != EGrpcServiceState::Ready && GreeterService->GetServiceState() != EGrpcServiceState::Idle) ||
        GreeterServiceClient == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Connect to greeter service first!"));
        return;
    }
    if (GreeterServiceClient->GetContextState(CtxHello) != EGrpcContextState::Done)
    {
        UE_LOG(LogTemp, Warning, TEXT("Busy!"));
        return;
    }

    CtxHello = GreeterServiceClient->InitHello();

    FGrpcGreeterHelloRequest helloRequest;
    helloRequest.Name = GreeterRequestName->GetText().ToString();
    GreeterServiceClient->Hello(CtxHello, helloRequest);

    CallGreeterButton->SetIsEnabled(false);
}

void UTurboLinkDemoCppTest::OnCallGreeterWithLambdaButtonClicked()
{
    if (GreeterService == nullptr ||
        (GreeterService->GetServiceState() != EGrpcServiceState::Ready && GreeterService->GetServiceState() != EGrpcServiceState::Idle))
    {
        UE_LOG(LogTemp, Warning, TEXT("Connect to greeter service first!"));
        return;
    }

    CallGreeterWithLambdaButton->SetIsEnabled(false);
    FGrpcGreeterHelloRequest helloRequest;
    helloRequest.Name = GreeterRequestWithLambdaName->GetText().ToString();

    GreeterService->CallHello(helloRequest, [this](const FGrpcResult& Result, const FGrpcGreeterHelloResponse& Response) {
        if (Result.Code == EGrpcResultCode::Ok)
        {
            this->GreeterResponseWithLambda->SetText(FText::FromString(Response.ReplyMessage));
        }
        else
        {
            this->GreeterResponseWithLambda->SetText(FText::FromString(Result.GetCodeString()));
        }
        CallGreeterWithLambdaButton->SetIsEnabled(true);
    });
}

void UTurboLinkDemoCppTest::OnGreeterServiceStateChanged(EGrpcServiceState NewState)
{
    int preFixLen = FString(TEXT("EGrpcServiceState::")).Len();
    GreeterServiceStatus->SetText(FText::FromString(UEnum::GetValueAsString(NewState).RightChop(preFixLen)));
    if (NewState == EGrpcServiceState::Ready && GreeterServiceClient==nullptr)
    {
        CreatGreeterClientButton->SetIsEnabled(true);
        CallGreeterWithLambdaButton->SetIsEnabled(true);
    }
}

void UTurboLinkDemoCppTest::OnHelloResponse(FGrpcContextHandle Handle, const FGrpcResult& Result, const FGrpcGreeterHelloResponse& Response)
{
    if (Result.Code == EGrpcResultCode::Ok)
    {
        GreeterResponse->SetText(FText::FromString(Response.ReplyMessage));
    }
    else
    {
        GreeterResponse->SetText(FText::FromString(Result.GetCodeString()));
    }
    CallGreeterButton->SetIsEnabled(true);
}

void UTurboLinkDemoCppTest::OnConnectTimeServiceButtonClicked()
{
    if (TimeService != nullptr) return;
    UTurboLinkGrpcManager* turboLinkManager = UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager();

    //create service adapter
    TimeService = Cast<UTimeService>(turboLinkManager->MakeService("TimeService"));
    if (TimeService == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Can't find time service!"));
        return;
    }
    TimeService->OnServiceStateChanged.AddUniqueDynamic(this, &UTurboLinkDemoCppTest::OnTimeServiceStateChanged);
    TimeService->Connect();

    ConnectTimeServiceButton->SetIsEnabled(false);
}

void UTurboLinkDemoCppTest::OnTimeServiceStateChanged(EGrpcServiceState NewState)
{
    int preFixLen = FString(TEXT("EGrpcServiceState::")).Len();
    TimeServiceStatus->SetText(FText::FromString(UEnum::GetValueAsString(NewState).RightChop(preFixLen)));
    if (NewState == EGrpcServiceState::Ready && TimeServiceClient == nullptr)
    {
        CreateTimeClientButton->SetIsEnabled(true);
    }
    else if (NewState == EGrpcServiceState::Idle && TimeServiceClient != nullptr)
    {
        InitWatchButton->SetIsEnabled(true);
        CallWatchButton->SetIsEnabled(false);
    }
}

void UTurboLinkDemoCppTest::OnCreateTimeClientButtonClicked()
{
    if (TimeService != nullptr &&
        TimeService->GetServiceState() == EGrpcServiceState::Ready &&
        TimeServiceClient == nullptr)
    {
        //create client
        TimeServiceClient = TimeService->MakeClient();
        TimeServiceClient->OnTicktokResponse.AddUniqueDynamic(this, &UTurboLinkDemoCppTest::OnTicktokResponse);
        TimeServiceClient->OnWatchResponse.AddUniqueDynamic(this, &UTurboLinkDemoCppTest::OnWatchResponse);
        TimeServiceClient->OnContextStateChange.AddUniqueDynamic(this, &UTurboLinkDemoCppTest::OnTimeContextStateChanged);

        CreateTimeClientButton->SetIsEnabled(false);

        CallTicktokButton->SetIsEnabled(true);
        InitWatchButton->SetIsEnabled(true);
    }
}

void UTurboLinkDemoCppTest::OnCallTicktokButtonClicked()
{
    if (TimeService == nullptr ||
        (TimeService->GetServiceState() != EGrpcServiceState::Ready && TimeService->GetServiceState() != EGrpcServiceState::Idle) ||
        TimeServiceClient == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Connect to time service first!"));
        return;
    }

    if (TimeServiceClient->GetContextState(CtxTicktok) != EGrpcContextState::Done)
    {
        UE_LOG(LogTemp, Warning, TEXT("Busy!"));
        return;
    }

    CtxTicktok = TimeServiceClient->InitTicktok();

    FGrpcGreeterTicktokRequest request;
    request.Counts = FCString::Atoi(*(TicktokRequest->GetText().ToString()));
    TimeServiceClient->Ticktok(CtxTicktok, request);

    CallTicktokButton->SetIsEnabled(false);
}

void UTurboLinkDemoCppTest::OnTicktokResponse(FGrpcContextHandle Handle, const FGrpcResult& Result, const FGrpcGreeterNowResponse& Response)
{
    if (Result.Code == EGrpcResultCode::Ok)
    {
        FString value = FString::Printf(TEXT("%lld/%d"), Response.Now, Response.Counts);
        TicktokResponse->SetText(FText::FromString(value));
    }
    else
    {
        TicktokResponse->SetText(FText::FromString(Result.GetCodeString()));
    }
}

void UTurboLinkDemoCppTest::OnInitWatchButtonClicked()
{
    if (TimeService == nullptr ||
        (TimeService->GetServiceState() != EGrpcServiceState::Ready && TimeService->GetServiceState() != EGrpcServiceState::Idle) ||
        TimeServiceClient == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Connect to time service first!"));
        return;
    }

    if (TimeServiceClient->GetContextState(CtxWatch) != EGrpcContextState::Done)
    {
        UE_LOG(LogTemp, Warning, TEXT("Busy!"));
        return;
    }

    CtxWatch = TimeServiceClient->InitWatch();

    InitWatchButton->SetIsEnabled(false);
    CallWatchButton->SetIsEnabled(true);
}

void UTurboLinkDemoCppTest::OnCallWatchButtonClicked()
{
    if (TimeServiceClient && TimeServiceClient->GetContextState(CtxWatch) == EGrpcContextState::Busy)
    {
        TimeServiceClient->Watch(CtxWatch, {});
    }
}

void UTurboLinkDemoCppTest::OnWatchResponse(FGrpcContextHandle Handle, const FGrpcResult& Result, const FGrpcGreeterNowResponse& Response)
{
    if (Result.Code == EGrpcResultCode::Ok)
    {
        FString value = FString::Printf(TEXT("%lld"), Response.Now);
        WatchResponse->SetText(FText::FromString(value));
    }
    else
    {
        WatchResponse->SetText(FText::FromString(Result.GetCodeString()));
    }
}

void UTurboLinkDemoCppTest::OnTimeContextStateChanged(FGrpcContextHandle Handle, EGrpcContextState NewState)
{
    if (Handle == CtxTicktok && NewState == EGrpcContextState::Done)
    {
        CallTicktokButton->SetIsEnabled(true);
    }
}