//Copyright 2022~2024 DevGrain. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "VoltModuleItem.h"
#include "Layout/Margin.h"
#include "Volt_ASM_InterpBackgroundColor.generated.h"

UCLASS(EditInlineNew, Blueprintable, BlueprintType)
class VOLT_API UVolt_ASM_InterpBackgroundColor : public UVoltModuleItem
{
	
	GENERATED_BODY()

public:

	virtual void ModifySlateVariable(const float DeltaTime, const TScriptInterface<IVoltInterface>& Volt) override;

	virtual bool IsActive() override;
	
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Module Property")
	FLinearColor StartColor;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Module Property")
	bool bUseStartColor = false; //If not specified, do nothing.
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Module Property")
	FLinearColor TargetColor;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Module Property")
	float InterpSpeed = 5;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Module Property")
	bool bUseConstantInterp = false;

private:
	
	UPROPERTY(Transient)
	bool bEverUpdated = false; //If not specified, do nothing.
	
};