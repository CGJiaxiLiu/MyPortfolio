// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "JoyconInputPlugin.h"
#include "JoyconInputLibrary.h"
#include "Interfaces/IPluginManager.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Engine.h"
#include "Core.h"
#include "JoyconBase.h"


#define LOCTEXT_NAMESPACE "FJoyconInputPluginModule"

const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_AxisX_L("Joycon_AxisX_L");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_AxisY_L("Joycon_AxisY_L");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_ZL("Joycon_ZL");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_L("Joycon_L");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_Minus("Joycon_Minus");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_Stick_Button_L("Joycon_Stick_Button_L");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_Up("Joycon_Up");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_Down("Joycon_Down");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_Left("Joycon_Left");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_Right("Joycon_Right");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_Capture("Joycon_Capture");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_SL_L("Joycon_SL_L");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_SR_L("Joycon_SR_L");

const FKey FJoyconInputKeys::Joycon_AxisX_L(FJoyconInputKeyNames::Joycon_AxisX_L);
const FKey FJoyconInputKeys::Joycon_AxisY_L(FJoyconInputKeyNames::Joycon_AxisY_L);
const FKey FJoyconInputKeys::Joycon_ZL(FJoyconInputKeyNames::Joycon_ZL);
const FKey FJoyconInputKeys::Joycon_L(FJoyconInputKeyNames::Joycon_L);
const FKey FJoyconInputKeys::Joycon_Minus(FJoyconInputKeyNames::Joycon_Minus);
const FKey FJoyconInputKeys::Joycon_Stick_Button_L(FJoyconInputKeyNames::Joycon_Stick_Button_L);
const FKey FJoyconInputKeys::Joycon_Up(FJoyconInputKeyNames::Joycon_Up);
const FKey FJoyconInputKeys::Joycon_Down(FJoyconInputKeyNames::Joycon_Down);
const FKey FJoyconInputKeys::Joycon_Left(FJoyconInputKeyNames::Joycon_Left);
const FKey FJoyconInputKeys::Joycon_Right(FJoyconInputKeyNames::Joycon_Right);
const FKey FJoyconInputKeys::Joycon_Capture(FJoyconInputKeyNames::Joycon_Capture);
const FKey FJoyconInputKeys::Joycon_SL_L(FJoyconInputKeyNames::Joycon_SL_L);
const FKey FJoyconInputKeys::Joycon_SR_L(FJoyconInputKeyNames::Joycon_SR_L);

const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_AxisX_R("Joycon_AxisX_R");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_AxisY_R("Joycon_AxisY_R");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_ZR("Joycon_ZR");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_R("Joycon_R");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_Plus("Joycon_Plus");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_Stick_Button_R("Joycon_Stick_Button_R");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_X("Joycon_X");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_Y("Joycon_Y");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_A("Joycon_A");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_B("Joycon_B");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_Home("Joycon_Home");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_SL_R("Joycon_SL_R");
const FGamepadKeyNames::Type FJoyconInputKeyNames::Joycon_SR_R("Joycon_SR_R");

const FKey FJoyconInputKeys::Joycon_AxisX_R(FJoyconInputKeyNames::Joycon_AxisX_R);
const FKey FJoyconInputKeys::Joycon_AxisY_R(FJoyconInputKeyNames::Joycon_AxisY_R);
const FKey FJoyconInputKeys::Joycon_ZR(FJoyconInputKeyNames::Joycon_ZR);
const FKey FJoyconInputKeys::Joycon_R(FJoyconInputKeyNames::Joycon_R);
const FKey FJoyconInputKeys::Joycon_Plus(FJoyconInputKeyNames::Joycon_Plus);
const FKey FJoyconInputKeys::Joycon_Stick_Button_R(FJoyconInputKeyNames::Joycon_Stick_Button_R);
const FKey FJoyconInputKeys::Joycon_X(FJoyconInputKeyNames::Joycon_X);
const FKey FJoyconInputKeys::Joycon_Y(FJoyconInputKeyNames::Joycon_Y);
const FKey FJoyconInputKeys::Joycon_A(FJoyconInputKeyNames::Joycon_A);
const FKey FJoyconInputKeys::Joycon_B(FJoyconInputKeyNames::Joycon_B);
const FKey FJoyconInputKeys::Joycon_Home(FJoyconInputKeyNames::Joycon_Home);
const FKey FJoyconInputKeys::Joycon_SL_R(FJoyconInputKeyNames::Joycon_SL_R);
const FKey FJoyconInputKeys::Joycon_SR_R(FJoyconInputKeyNames::Joycon_SR_R);

void IJoyconInputPlugin::StartupModule()
{
	UE_LOG(LogTemp, Warning, TEXT("IJoyconInputPlugin::StartupModule()"));
	IModularFeatures::Get().RegisterModularFeature(IInputDeviceModule::GetModularFeatureName(), this);

	FString PluginBaseDir = IPluginManager::Get().FindPlugin("JoyconInputPlugin")->GetBaseDir();
	//FString BaseDir = FPaths::ProjectDir();
	FString LibraryPath = FPaths::Combine(*PluginBaseDir, TEXT("Source/JoyconInputPlugin/Bin/hidapi.dll"));
	//FString LibraryPath = FPaths::Combine(*BaseDir, TEXT("Plugins/JoyconInputPlugin/Source/JoyconInputPlugin/Bin/hidapi.dll"));
	HidApiDLLHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	LibraryPath = FPaths::Combine(*PluginBaseDir, TEXT("Source/JoyconInputPlugin/Bin/JoyconProject.dll"));
	//LibraryPath = FPaths::Combine(*BaseDir, TEXT("Plugins/JoyconInputPlugin/Source/JoyconInputPlugin/Bin/JoyconProject.dll"));
	JoyconBaseDLLHandle = !LibraryPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*LibraryPath) : nullptr;

	JoyconBase* pt_L = new JoyconBase(SIDE::LEFT);
	jc_L = MakeShareable(pt_L);

	JoyconBase* pt_R = new JoyconBase(SIDE::RIGHT);
	jc_R = MakeShareable(pt_R);

	if (HidApiDLLHandle && JoyconBaseDLLHandle) {
		UE_LOG(LogTemp, Warning, TEXT("DLL Load Success!"));
		jc_L->Init();
		jc_R->Init();
		if (jc_L->IsValid() && jc_R->IsValid()) {
			UE_LOG(LogTemp, Warning, TEXT("Joycon Init Success!"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Joycon Init Fail!"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("DLL Load Fail!"));
	}

	const FName NAME_Joycon(TEXT("NintendoJoyconController"));

	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_AxisX_R, LOCTEXT("Joycon_AxisX_R", "Joycon AxisX R"), FKeyDetails::FloatAxis, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_AxisY_R, LOCTEXT("Joycon_AxisY_R", "Joycon AxisY R"), FKeyDetails::FloatAxis, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_ZR, LOCTEXT("Joycon_ZR", "Joycon ZR"), FKeyDetails::GamepadKey, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_R, LOCTEXT("Joycon_R", "Joycon R"), FKeyDetails::GamepadKey, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_Plus, LOCTEXT("Joycon_Plus", "Joycon Plus"), FKeyDetails::GamepadKey, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_Stick_Button_R, LOCTEXT("Joycon_Stick_Button_R", "Joycon Stick Button R"), FKeyDetails::GamepadKey, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_X, LOCTEXT("Joycon_X", "Joycon X"), FKeyDetails::GamepadKey, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_Y, LOCTEXT("Joycon_Y", "Joycon Y"), FKeyDetails::GamepadKey, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_A, LOCTEXT("Joycon_A", "Joycon A"), FKeyDetails::GamepadKey, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_B, LOCTEXT("Joycon_B", "Joycon B"), FKeyDetails::GamepadKey, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_Home, LOCTEXT("Joycon_Home", "Joycon Home"), FKeyDetails::GamepadKey, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_SL_R, LOCTEXT("Joycon_SL_R", "Joycon SL R"), FKeyDetails::GamepadKey, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_SR_R, LOCTEXT("Joycon_SR_R", "Joycon SR R"), FKeyDetails::GamepadKey, NAME_Joycon));

	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_AxisX_L, LOCTEXT("Joycon_AxisX_L", "Joycon AxisX L"), FKeyDetails::FloatAxis, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_AxisY_L, LOCTEXT("Joycon_AxisY_L", "Joycon AxisY L"), FKeyDetails::FloatAxis, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_ZL, LOCTEXT("Joycon_ZL", "Joycon ZL"), FKeyDetails::GamepadKey, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_L, LOCTEXT("Joycon_L", "Joycon L"), FKeyDetails::GamepadKey, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_Minus, LOCTEXT("Joycon_Minus", "Joycon Minus"), FKeyDetails::GamepadKey, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_Stick_Button_L, LOCTEXT("Joycon_Stick_Button_L", "Joycon Stick Button L"), FKeyDetails::GamepadKey, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_Up, LOCTEXT("Joycon_Up", "Joycon Up"), FKeyDetails::GamepadKey, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_Down, LOCTEXT("Joycon_Down", "Joycon Down"), FKeyDetails::GamepadKey, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_Left, LOCTEXT("Joycon_Left", "Joycon Left"), FKeyDetails::GamepadKey, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_Right, LOCTEXT("Joycon_Right", "Joycon Right"), FKeyDetails::GamepadKey, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_Capture, LOCTEXT("Joycon_Capture", "Joycon Capture"), FKeyDetails::GamepadKey, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_SL_L, LOCTEXT("Joycon_SL_L", "Joycon SL L"), FKeyDetails::GamepadKey, NAME_Joycon));
	EKeys::AddKey(FKeyDetails(FJoyconInputKeys::Joycon_SR_L, LOCTEXT("Joycon_SR_L", "Joycon SR L"), FKeyDetails::GamepadKey, NAME_Joycon));
}

void IJoyconInputPlugin::ShutdownModule()
{
	IModularFeatures::Get().UnregisterModularFeature(IInputDeviceModule::GetModularFeatureName(), this);


	FPlatformProcess::FreeDllHandle(HidApiDLLHandle);
	FPlatformProcess::FreeDllHandle(JoyconBaseDLLHandle);
	JoyconBaseDLLHandle = nullptr;
	HidApiDLLHandle = nullptr;
}

TSharedPtr<class IInputDevice> IJoyconInputPlugin::CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	FJoyconDevice* pt = new FJoyconDevice(InMessageHandler);
	JoyconInputDevice = MakeShareable(pt);
	JoyconInputDevice->setJoyconHandle(jc_L, jc_R);
	return JoyconInputDevice;
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(IJoyconInputPlugin, JoyconInputPlugin)

