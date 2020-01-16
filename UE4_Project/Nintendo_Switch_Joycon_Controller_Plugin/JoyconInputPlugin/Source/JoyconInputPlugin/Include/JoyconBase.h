#ifndef JOYCON_BASE
#define JOYCON_BASE

#if defined(API_COMPILE)
#define Api __declspec(dllexport)
#else
#define Api __declspec(dllimport)
#endif

#define MAX_STR 255

#include <string>
#include <unordered_map>
#include <vector>


enum SIDE {LEFT, RIGHT};
enum BUTTON_NAME {ZL_ZR, L_R, MINUS_PLUS, STICK_BUTTON, UP_X, DOWN_B, LEFT_Y, RIGHT_A, CAPTURE_HOME, SL, SR};
static std::unordered_map<BUTTON_NAME, std::string> ButtonEnumToString { {ZL_ZR, "ZL_ZR"}, {L_R, "L_R"}, {MINUS_PLUS, "MINUS_PLUS"}, {STICK_BUTTON, "STICK_BUTTON"}, {UP_X, "UP_X"}, {DOWN_B, "DOWN_B"}, {LEFT_Y, "LEFT_Y"}, {RIGHT_A, "RIGHT_A"}, {CAPTURE_HOME, "CAPTURE_HOME"}, {SL, "SL"}, {SR, "SR"}};
typedef struct hid_device_ hid_device;

struct Stick {
	uint16_t x = 0;
	uint16_t y = 0;
	float CalX = 0;
	float CalY = 0;
};

struct Gyroscope {
	// relative:
	float pitch = 0;
	float yaw = 0;
	float roll = 0;

	struct Offset {
		int n = 0;
		// absolute:
		float pitch = 0;
		float yaw = 0;
		float roll = 0;
	} offset;
};

struct Accelerometer {
	float prevX = 0;
	float prevY = 0;
	float prevZ = 0;
	float x = 0;
	float y = 0;
	float z = 0;
};

class Api JoyconBase
{
public:
	JoyconBase(SIDE side);

	// init and dev info
	void Init();
	wchar_t * GetSerial();
	std::string GetName();
	SIDE GetSide();
	bool IsValid();

	// poll & get joycon state
	void Poll();
	void debug_receive_data(bool mute);
	float GetStickX();
	float GetStickY();
	bool GetButtonState(BUTTON_NAME b);
	std::vector<BUTTON_NAME> GetPressedButton();

	// set rumble
	void rumble(int frequency, int intensity);
	void rumble2(uint16_t hf, uint8_t hfa, uint8_t lf, uint16_t lfa);
	void rumble3(float frequency, uint8_t hfa, uint16_t lfa);
	void rumble4(float real_LF, float real_HF, uint8_t hfa, uint16_t lfa);
	void rumble_freq(uint16_t hf, uint8_t hfa, uint8_t lf, uint16_t lfa);

private:
	// communication
	int hid_exchange(hid_device *handle, unsigned char *buf, int len);
	int send_command(int command, uint8_t *data, int len);
	int send_subcommand(int command, int subcommand, uint8_t *data, int len);

	// calibrate
	void setGyroOffsets();
	void CalcAnalogStick();
	void CalcAnalogStick2(float &pOutX, float &pOutY, uint16_t x, uint16_t y, uint16_t x_calc[3], uint16_t y_calc[3]);
	int get_spi_data(uint32_t offset, const uint16_t read_len, uint8_t *test_buf);
	int write_spi_data(uint32_t offset, const uint16_t write_len, uint8_t* test_buf);

private:
	// device info
	hid_device* handle;
	wchar_t serial[MAX_STR];
	std::string name;
	SIDE leftOrRight;
	bool isValid;

	// joycon state
	std::unordered_map<BUTTON_NAME, bool> buttonState;
	std::vector<BUTTON_NAME> pressedButton;
	Stick stick;
	Gyroscope gyro;
	Accelerometer accel;

	// device data
	int global_count = 0;
	uint8_t buf[0x41];

	// calibration data
	float acc_cal_coeff[3];
	float gyro_cal_coeff[3];
	float cal_x[1] = { 0.0f };
	float cal_y[1] = { 0.0f };

	bool has_user_cal_stick_l = false;
	bool has_user_cal_stick_r = false;
	bool has_user_cal_sensor = false;

	unsigned char factory_stick_cal[0x12];
	unsigned char user_stick_cal[0x16];
	unsigned char sensor_model[0x6];
	unsigned char stick_model[0x24];
	unsigned char factory_sensor_cal[0x18];
	unsigned char user_sensor_cal[0x1A];
	uint16_t factory_sensor_cal_calm[0xC];
	uint16_t user_sensor_cal_calm[0xC];
	int16_t sensor_cal[0x2][0x3];
	uint16_t stick_cal_x_l[0x3];
	uint16_t stick_cal_y_l[0x3];
	uint16_t stick_cal_x_r[0x3];
	uint16_t stick_cal_y_r[0x3];

};
#endif