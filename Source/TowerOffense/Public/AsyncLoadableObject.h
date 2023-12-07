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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category= "Async Loading")
	void LoadAssets();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category= "Async Loading")
	void OnAssetsLoaded();
	//load assets
	//macros
	//
};
