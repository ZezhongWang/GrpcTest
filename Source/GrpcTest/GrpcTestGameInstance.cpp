#include "GrpcTestGameInstance.h"
#include "TurboLinkGrpcManager.h"
#include "TurboLinkGrpcUtilities.h"

void UGrpcTestGameInstance::Init()
{
	Super::Init();
	UE_LOG(LogBlueprint, Warning, TEXT("Init UGrpcTestGameInstance"));
	UTurboLinkGrpcManager* turboLinkManager = UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager();
	turboLinkManager->InitManager();
}

void UGrpcTestGameInstance::Shutdown()
{
	Super::Shutdown();
	UTurboLinkGrpcManager* turboLinkManager = UTurboLinkGrpcUtilities::GetTurboLinkGrpcManager();
	turboLinkManager->Shutdown();
}
