// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GrpcTestGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class  UGrpcTestGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void Init() override;
	void Shutdown() override;
};
