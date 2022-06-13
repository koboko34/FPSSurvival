// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayMode.generated.h"

/**
 * 
 */

UENUM()
enum EPlayMode
{
	MODE_Default	UMETA(DisplayName = "DefaultMode"),
	MODE_Ability	UMETA(DisplayName = "AbilityMode")
};
