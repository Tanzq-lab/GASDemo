// Copyright Amos Tan

#include "AbilitySystem/Data/AttributeInfo.h"

#include "Utility/GasDemoLog.h"

FGasAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FGasAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		GAS_DEMO_LOG(Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."), *AttributeTag.ToString(),*GetNameSafe(this))
	}

	return FGasAttributeInfo();
}
