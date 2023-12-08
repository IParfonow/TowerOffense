#pragma once

#include "UObject/Interface.h"
#include "AsyncLoadableObject.generated.h"


UINTERFACE(Blueprintable)
class TOWEROFFENSE_API UAsyncLoadableObject : public UInterface
{
	GENERATED_BODY()
};

class TOWEROFFENSE_API IAsyncLoadableObject
{
	GENERATED_BODY()
	
public:
	DECLARE_EVENT(IAsyncLoadableObject, FOnAssetsLoaded)

	virtual void LoadAssets(){};

	virtual void OnAssetsLoaded(){};

	virtual FOnAssetsLoaded& GetOnAssetsLoadedEvent() = 0;
};
