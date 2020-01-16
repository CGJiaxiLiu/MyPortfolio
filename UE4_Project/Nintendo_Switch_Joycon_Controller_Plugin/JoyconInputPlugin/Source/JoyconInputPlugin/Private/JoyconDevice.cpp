#include "JoyconDevice.h"
#include "JoyconInputLibrary.h"

FJoyconDevice::FJoyconDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
	: MessageHandler(InMessageHandler)
{
	LeftJoyconData.Push(JoyconData());
	LeftJoyconData.Push(JoyconData());

	RightJoyconData.Push(JoyconData());
	RightJoyconData.Push(JoyconData());
}

FJoyconDevice::~FJoyconDevice()
{
}

void FJoyconDevice::SendControllerEvents()
{
	//CalledCount++;
	//UE_LOG(LogTemp, Warning, TEXT("FJoyconDevice::SendControllerEvents():%d"), CalledCount);

	/** Poll Left Joycon and send message **/  
	this->JoyconHandle_L->Poll();
	LeftJoyconData[1].jc_x = this->JoyconHandle_L->GetStickX();
	LeftJoyconData[1].jc_y = this->JoyconHandle_L->GetStickY();

	for (auto& item : LeftJoyconData[1].ButtonState) {
		LeftJoyconData[1].ButtonState[item.Key] = this->JoyconHandle_L->GetButtonState(item.Key);
		if (LeftJoyconData[0].ButtonState[item.Key] != LeftJoyconData[1].ButtonState[item.Key]) {
			UE_LOG(LogTemp, Warning, TEXT("Button State Change %s"), UTF8_TO_TCHAR(ButtonEnumToString[item.Key].c_str()));
			if (LeftJoyconData[1].ButtonState[item.Key]) {
				this->MessageHandler->OnControllerButtonPressed(ButtonNameToLeftJoyconInputKeyNames[item.Key], 0, false);
			}
			else {
				this->MessageHandler->OnControllerButtonReleased(ButtonNameToLeftJoyconInputKeyNames[item.Key], 0, false);
			}
		}
	}
	LeftJoyconData[0].ButtonState = LeftJoyconData[1].ButtonState;

	this->MessageHandler->OnControllerAnalog(FJoyconInputKeyNames::Joycon_AxisX_L, 0, LeftJoyconData[1].jc_x);
	this->MessageHandler->OnControllerAnalog(FJoyconInputKeyNames::Joycon_AxisY_L, 0, LeftJoyconData[1].jc_y);


	/** Poll Right Joycon and send message **/
	this->JoyconHandle_R->Poll();
	RightJoyconData[1].jc_x = this->JoyconHandle_R->GetStickX();
	RightJoyconData[1].jc_y = this->JoyconHandle_R->GetStickY();

	for (auto& item : RightJoyconData[1].ButtonState) {
		RightJoyconData[1].ButtonState[item.Key] = this->JoyconHandle_R->GetButtonState(item.Key);
		if (RightJoyconData[0].ButtonState[item.Key] != RightJoyconData[1].ButtonState[item.Key]) {
			UE_LOG(LogTemp, Warning, TEXT("Button State Change %s"), UTF8_TO_TCHAR(ButtonEnumToString[item.Key].c_str()));
			if (RightJoyconData[1].ButtonState[item.Key]) {
				this->MessageHandler->OnControllerButtonPressed(ButtonNameToRightJoyconInputKeyNames[item.Key], 0, false);
			}
			else {
				this->MessageHandler->OnControllerButtonReleased(ButtonNameToRightJoyconInputKeyNames[item.Key], 0, false);
			}
		}
	}
	RightJoyconData[0].ButtonState = RightJoyconData[1].ButtonState;

	this->MessageHandler->OnControllerAnalog(FJoyconInputKeyNames::Joycon_AxisX_R, 0, RightJoyconData[1].jc_x);
	this->MessageHandler->OnControllerAnalog(FJoyconInputKeyNames::Joycon_AxisY_R, 0, RightJoyconData[1].jc_y);
}

void FJoyconDevice::setJoyconHandle(TSharedPtr<JoyconBase> InJoyconHandle_L, TSharedPtr<JoyconBase> InJoyconHandle_R)
{
	this->JoyconHandle_L = InJoyconHandle_L;
	this->JoyconHandle_R = InJoyconHandle_R;
}
