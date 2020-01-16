#pragma once
#include "IInputDevice.h"
#include "JoyconBase.h"
#include "GenericPlatform/GenericApplicationMessageHandler.h"
#include "JoyconInputLibrary.h"

//class JoyconBase;

class FJoyconDevice : public IInputDevice
{
public:
	FJoyconDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler);
	~FJoyconDevice();

	/** barely override the pure virtual function to implement the class **/
	virtual void Tick(float DeltaTime) override {}
	virtual void SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) override { MessageHandler = InMessageHandler; }
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override { return false; }
	virtual void SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value) override {}
	virtual void SetChannelValues(int32 ControllerId, const FForceFeedbackValues& Values) override {}

	/** poll the Device and use MessageHandler to fire event **/
	virtual void SendControllerEvents() override;
	void setJoyconHandle(TSharedPtr<JoyconBase> InJoyconHandle_L, TSharedPtr<JoyconBase> InJoyconHandle_R);

	//enum DataTarget{CURRENT_FRAME_LEFT, CURRENT_FRAME_RIGHT, LAST_FRAME_LEFT, LAST_FRAME_RIGHT};

	// Map the ButtonName to UE4 Keyname
	const TMap<BUTTON_NAME, FGamepadKeyNames::Type> ButtonNameToLeftJoyconInputKeyNames { { BUTTON_NAME::ZL_ZR, FJoyconInputKeyNames::Joycon_ZL }, { BUTTON_NAME::L_R, FJoyconInputKeyNames::Joycon_L }, { BUTTON_NAME::MINUS_PLUS, FJoyconInputKeyNames::Joycon_Minus }, { BUTTON_NAME::STICK_BUTTON, FJoyconInputKeyNames::Joycon_Stick_Button_L }, { BUTTON_NAME::UP_X, FJoyconInputKeyNames::Joycon_Up }, { BUTTON_NAME::DOWN_B, FJoyconInputKeyNames::Joycon_Down }, { BUTTON_NAME::LEFT_Y, FJoyconInputKeyNames::Joycon_L }, { BUTTON_NAME::RIGHT_A, FJoyconInputKeyNames::Joycon_Right }, { BUTTON_NAME::CAPTURE_HOME, FJoyconInputKeyNames::Joycon_Capture }, { BUTTON_NAME::SL, FJoyconInputKeyNames::Joycon_SL_L }, { BUTTON_NAME::SR, FJoyconInputKeyNames::Joycon_SR_L } };
	const TMap<BUTTON_NAME, FGamepadKeyNames::Type> ButtonNameToRightJoyconInputKeyNames { { BUTTON_NAME::ZL_ZR, FJoyconInputKeyNames::Joycon_ZR }, { BUTTON_NAME::L_R, FJoyconInputKeyNames::Joycon_R }, { BUTTON_NAME::MINUS_PLUS, FJoyconInputKeyNames::Joycon_Plus }, { BUTTON_NAME::STICK_BUTTON, FJoyconInputKeyNames::Joycon_Stick_Button_R }, { BUTTON_NAME::UP_X, FJoyconInputKeyNames::Joycon_X }, { BUTTON_NAME::DOWN_B, FJoyconInputKeyNames::Joycon_B }, { BUTTON_NAME::LEFT_Y, FJoyconInputKeyNames::Joycon_Y }, { BUTTON_NAME::RIGHT_A, FJoyconInputKeyNames::Joycon_A }, { BUTTON_NAME::CAPTURE_HOME, FJoyconInputKeyNames::Joycon_Home }, { BUTTON_NAME::SL, FJoyconInputKeyNames::Joycon_SL_R }, { BUTTON_NAME::SR, FJoyconInputKeyNames::Joycon_SR_R } };

	struct JoyconData {
		float jc_x;
		float jc_y;

		TMap<BUTTON_NAME, bool> ButtonState { { BUTTON_NAME::ZL_ZR, false }, { BUTTON_NAME::L_R, false }, { BUTTON_NAME::MINUS_PLUS, false }, { BUTTON_NAME::STICK_BUTTON, false }, { BUTTON_NAME::UP_X, false }, { BUTTON_NAME::DOWN_B, false }, { BUTTON_NAME::LEFT_Y, false }, { BUTTON_NAME::RIGHT_A, false }, { BUTTON_NAME::CAPTURE_HOME, false }, { BUTTON_NAME::SL, false }, { BUTTON_NAME::SR, false } };
	};

protected:
	/** Handler to send all messages to. */
	TSharedRef<FGenericApplicationMessageHandler> MessageHandler;
	//int CalledCount = 0;

	TSharedPtr<JoyconBase> JoyconHandle_L;
	TSharedPtr<JoyconBase> JoyconHandle_R;

	TArray<JoyconData> LeftJoyconData;
	TArray<JoyconData> RightJoyconData;


};

