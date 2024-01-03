// Copyright Amos Tan

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGasDemo, Display, All);

#define GAS_DEMO_LOG(Verbosity, Format, ...) \
{ \
	UE_LOG(LogGasDemo, Verbosity, Format, ##__VA_ARGS__); \
}