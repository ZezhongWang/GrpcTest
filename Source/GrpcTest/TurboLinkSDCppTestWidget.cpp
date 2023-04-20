// Fill out your copyright notice in the Description page of Project Settings.


#include "TurboLinkSDCppTestWidget.h"

#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"

#include "TurboLinkGrpcManager.h"
#include "TurboLinkGrpcUtilities.h"
#include "TurboLinkGrpcConfig.h"

#include "SGreeter/GreeterService.h"

void UTurboLinkCppTestWidget::NativeConstruct()
{
	Super::NativeConstruct();

    FString greeterServiceEndPoint = UTurboLinkGrpcUtilities::GetTurboLinkGrpcConfig()->GetServiceEndPoint(TEXT("GreeterService"));
    GreeterServiceAddress->SetText(FText::FromString(FString::Printf(TEXT("(%s)"), *greeterServiceEndPoint)));

    ConnectGreeterServiceButton->OnClicked.AddDynamic(this, &UTurboLinkCppTestWidget::OnConnectGreeterServiceButtonClicked);
    CreatGreeterClientButton->OnClicked.AddDynamic(this, &UTurboLinkCppTestWidget::OnCreateGreeterClientButtonClicked);
    CallGreeterButton->OnClicked.AddDynamic(this, &UTurboLinkCppTestWidget::OnCallGreeterButtonClicked);
    CallGreeterWithLambdaButton->OnClicked.AddDynamic(this, &UTurboLinkCppTestWidget::OnCallGreeterWithLambdaButtonClicked);

    FString timeServiceEndPoint = UTurboLinkGrpcUtilities::GetTurboLinkGrpcConfig()->GetServiceEndPoint(TEXT("TimeService"));
    TimeServiceAddress->SetText(FText::FromString(FString::Printf(TEXT("(%s)"), *timeServiceEndPoint)));

    ConnectTimeServiceButton->OnClicked.AddDynamic(this, &UTurboLinkCppTestWidget::OnConnectTimeServiceButtonClicked);
    CreateTimeClientButton->OnClicked.AddDynamic(this, &UTurboLinkCppTestWidget::OnCreateTimeClientButtonClicked);
    CallTicktokButton->OnClicked.AddDynamic(this, &UTurboLinkCppTestWidget::OnCallTicktokButtonClicked);
    InitWatchButton->OnClicked.AddDynamic(this, &UTurboLinkCppTestWidget::OnInitWatchButtonClicked);
    CallWatchButton->OnClicked.AddDynamic(this, &UTurboLinkCppTestWidget::OnCallWatchButtonClicked);

    CreatGreeterClientButton->SetIsEnabled(false);
    CallGreeterButton->SetIsEnabled(false);
    CallGreeterWithLambdaButton->SetIsEnabled(false);

    CreateTimeClientButton->SetIsEnabled(false);
    CallTicktokButton->SetIsEnabled(false);
    InitWatchButton->SetIsEnabled(false);
    CallWatchButton->SetIsEnabled(false);
}

void UTurboLinkCppTestWidget::OnConnectGreeterServiceButtonClicked()
{
    if (GreeterService != nullptr) return;
    UTurboLinkGrpcManager* turboLinkManager = UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager(this);

    //create service adapter
    GreeterService = Cast<UGreeterService>(turboLinkManager->MakeService("GreeterService"));
    if (GreeterService == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Can't find greeter service!"));
        return;
    }
    GreeterService->OnServiceStateChanged.AddUniqueDynamic(this, &UTurboLinkCppTestWidget::OnGreeterServiceStateChanged);
    GreeterService->Connect();

    ConnectGreeterServiceButton->SetIsEnabled(false);
}

void UTurboLinkCppTestWidget::OnCreateGreeterClientButtonClicked()
{
    if (GreeterService != nullptr && 
        GreeterService->GetServiceState() == EGrpcServiceState::Ready &&
        GreeterServiceClient == nullptr)
    {
        //create client
        GreeterServiceClient = GreeterService->MakeClient();
        GreeterServiceClient->OnHelloResponse.AddUniqueDynamic(this, &UTurboLinkCppTestWidget::OnHelloResponse);

        CallGreeterButton->SetIsEnabled(true);
        CreatGreeterClientButton->SetIsEnabled(false);
    }
}

void UTurboLinkCppTestWidget::OnCallGreeterButtonClicked()
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

void UTurboLinkCppTestWidget::OnCallGreeterWithLambdaButtonClicked()
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

void UTurboLinkCppTestWidget::OnGreeterServiceStateChanged(EGrpcServiceState NewState)
{
    int preFixLen = FString(TEXT("EGrpcServiceState::")).Len();
    GreeterServiceStatus->SetText(FText::FromString(UEnum::GetValueAsString(NewState).RightChop(preFixLen)));
    if (NewState == EGrpcServiceState::Ready && GreeterServiceClient==nullptr)
    {
        CreatGreeterClientButton->SetIsEnabled(true);
        CallGreeterWithLambdaButton->SetIsEnabled(true);
    }
}

void UTurboLinkCppTestWidget::OnHelloResponse(FGrpcContextHandle Handle, const FGrpcResult& Result, const FGrpcGreeterHelloResponse& Response)
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

void UTurboLinkCppTestWidget::OnConnectTimeServiceButtonClicked()
{
    if (TimeService != nullptr) return;
    UTurboLinkGrpcManager* turboLinkManager = UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager(this);

    //create service adapter
    TimeService = Cast<UTimeService>(turboLinkManager->MakeService("TimeService"));
    if (TimeService == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Can't find time service!"));
        return;
    }
    TimeService->OnServiceStateChanged.AddUniqueDynamic(this, &UTurboLinkCppTestWidget::OnTimeServiceStateChanged);
    TimeService->Connect();

    ConnectTimeServiceButton->SetIsEnabled(false);
}

void UTurboLinkCppTestWidget::OnTimeServiceStateChanged(EGrpcServiceState NewState)
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

void UTurboLinkCppTestWidget::OnCreateTimeClientButtonClicked()
{
    if (TimeService != nullptr &&
        TimeService->GetServiceState() == EGrpcServiceState::Ready &&
        TimeServiceClient == nullptr)
    {
        //create client
        TimeServiceClient = TimeService->MakeClient();
        TimeServiceClient->OnTicktokResponse.AddUniqueDynamic(this, &UTurboLinkCppTestWidget::OnTicktokResponse);
        TimeServiceClient->OnWatchResponse.AddUniqueDynamic(this, &UTurboLinkCppTestWidget::OnWatchResponse);
        TimeServiceClient->OnContextStateChange.AddUniqueDynamic(this, &UTurboLinkCppTestWidget::OnTimeContextStateChanged);

        CreateTimeClientButton->SetIsEnabled(false);

        CallTicktokButton->SetIsEnabled(true);
        InitWatchButton->SetIsEnabled(true);
    }
}

void UTurboLinkCppTestWidget::OnCallTicktokButtonClicked()
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

void UTurboLinkCppTestWidget::OnTicktokResponse(FGrpcContextHandle Handle, const FGrpcResult& Result, const FGrpcGreeterNowResponse& Response)
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

void UTurboLinkCppTestWidget::OnInitWatchButtonClicked()
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

void UTurboLinkCppTestWidget::OnCallWatchButtonClicked()
{
    if (TimeServiceClient && TimeServiceClient->GetContextState(CtxWatch) == EGrpcContextState::Busy)
    {
        TimeServiceClient->Watch(CtxWatch, {});
    }
}

void UTurboLinkCppTestWidget::OnWatchResponse(FGrpcContextHandle Handle, const FGrpcResult& Result, const FGrpcGreeterNowResponse& Response)
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

void UTurboLinkCppTestWidget::OnTimeContextStateChanged(FGrpcContextHandle Handle, EGrpcContextState NewState)
{
    if (Handle == CtxTicktok && NewState == EGrpcContextState::Done)
    {
        CallTicktokButton->SetIsEnabled(true);
    }
}