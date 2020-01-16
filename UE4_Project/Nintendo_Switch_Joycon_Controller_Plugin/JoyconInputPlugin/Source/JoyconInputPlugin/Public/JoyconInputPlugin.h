// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IInputDeviceModule.h"
#include "JoyconDevice.h"
#include "Modules/ModuleManager.h"
#include "InputCoreTypes.h"


class JoyconBase;

class IJoyconInputPlugin : public IInputDeviceModule
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual TSharedPtr<class IInputDevice > CreateInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) override;
	TSharedPtr<class FJoyconDevice> JoyconInputDevice;

	/** Relevent to the Joycon data **/
	TSharedPtr<JoyconBase> jc_L;
	TSharedPtr<JoyconBase> jc_R;

	/** Get DLLhandle before use **/
	void *HidApiDLLHandle;
	void *JoyconBaseDLLHandle;

};

