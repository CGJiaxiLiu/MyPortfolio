#include "JoyconBase.h"
#include "hidapi.h"
#include <windows.h>

float clamp(float a, float min, float max) {
	if (a < min) {
		return min;
	}
	else if (a > max) {
		return max;
	}
	else {
		return a;
	}
}

int16_t uint16_to_int16(uint16_t a) {
	int16_t b;
	char* aPointer = (char*)&a, *bPointer = (char*)&b;
	memcpy(bPointer, aPointer, sizeof(a));
	return b;
}

struct brcm_hdr {
	uint8_t cmd;
	uint8_t rumble[9];
};

struct brcm_cmd_01 {
	uint8_t subcmd;
	uint32_t offset;
	uint8_t size;
};

int timing_byte = 0x0;

JoyconBase::JoyconBase(SIDE side)
{
	if (SIDE::LEFT == side) {
		this->name = std::string("Joy-Con (L)");
		this->leftOrRight = SIDE::LEFT;
		this->isValid = false;
	}
	else {
		this->name = std::string("Joy-Con (R)");
		this->leftOrRight = SIDE::RIGHT;
		this->isValid = false;
	}
}

void JoyconBase::Init()
{
	if (SIDE::LEFT == this->leftOrRight) {
		this->handle = hid_open(0x057e, 0x2006, NULL);
		if (handle)
		{
			wchar_t wstr[MAX_STR];
			memset(wstr, 0, sizeof(wstr));
			int res = hid_get_serial_number_string(handle, wstr, MAX_STR);
			if (res >= 0) {
				memcpy(this->serial, wstr, MAX_STR);
			}
			this->isValid = true;
		}
		else {
			this->isValid = false;
			return;
		}
	}
	else{
		this->handle = hid_open(0x057e, 0x2007, NULL);
		if (handle)
		{
			wchar_t wstr[MAX_STR];
			memset(wstr, 0, sizeof(wstr));
			int res = hid_get_serial_number_string(handle, wstr, MAX_STR);
			if (res >= 0) {
				memcpy(this->serial, wstr, MAX_STR);
			}
			this->isValid = true;
		}
		else {
			this->isValid = false;
			return;
		}
	}

	unsigned char buf[0x40];
	memset(buf, 0, 0x40);

	// set blocking to ensure command is recieved:
	hid_set_nonblocking(this->handle, 0);

	// Enable vibration
	printf("Enabling vibration...\n");
	buf[0] = 0x01; // Enabled
	send_subcommand(0x1, 0x48, buf, 1);

	// Enable IMU data
	printf("Enabling IMU data...\n");
	buf[0] = 0x01; // Enabled
	send_subcommand(0x01, 0x40, buf, 1);

	// Set input report mode (to push at 60hz)
	// x00	Active polling mode for IR camera data. Answers with more than 300 bytes ID 31 packet
	// x01	Active polling mode
	// x02	Active polling mode for IR camera data.Special IR mode or before configuring it ?
	// x21	Unknown.An input report with this ID has pairing or mcu data or serial flash data or device info
	// x23	MCU update input report ?
	// 30	NPad standard mode. Pushes current state @60Hz. Default in SDK if arg is not in the list
	// 31	NFC mode. Pushes large packets @60Hz
	printf("Set input report mode to 0x30...\n");
	buf[0] = 0x30;
	send_subcommand(0x01, 0x03, buf, 1);

	// get calibration data:
	printf("Getting calibration data...\n");
	memset(factory_stick_cal, 0, 0x12);
	memset(user_stick_cal, 0, 0x16);
	memset(sensor_model, 0, 0x6);
	memset(stick_model, 0, 0x12);
	memset(factory_sensor_cal, 0, 0x18);
	memset(user_sensor_cal, 0, 0x1A);
	memset(factory_sensor_cal_calm, 0, 0xC);
	memset(user_sensor_cal_calm, 0, 0xC);
	memset(sensor_cal, 0, sizeof(sensor_cal));
	memset(stick_cal_x_l, 0, sizeof(stick_cal_x_l));
	memset(stick_cal_y_l, 0, sizeof(stick_cal_y_l));
	memset(stick_cal_x_r, 0, sizeof(stick_cal_x_r));
	memset(stick_cal_y_r, 0, sizeof(stick_cal_y_r));

	get_spi_data(0x6020, 0x18, factory_sensor_cal);
	get_spi_data(0x603D, 0x12, factory_stick_cal);
	get_spi_data(0x6080, 0x6, sensor_model);
	get_spi_data(0x6086, 0x12, stick_model);
	get_spi_data(0x6098, 0x12, &stick_model[0x12]);
	get_spi_data(0x8010, 0x16, user_stick_cal);
	get_spi_data(0x8026, 0x1A, user_sensor_cal);

	// get stick calibration data:
	// factory calibration:

	if (this->leftOrRight == SIDE::LEFT) {
		stick_cal_x_l[1] = (factory_stick_cal[4] << 8) & 0xF00 | factory_stick_cal[3];
		stick_cal_y_l[1] = (factory_stick_cal[5] << 4) | (factory_stick_cal[4] >> 4);
		stick_cal_x_l[0] = stick_cal_x_l[1] - ((factory_stick_cal[7] << 8) & 0xF00 | factory_stick_cal[6]);
		stick_cal_y_l[0] = stick_cal_y_l[1] - ((factory_stick_cal[8] << 4) | (factory_stick_cal[7] >> 4));
		stick_cal_x_l[2] = stick_cal_x_l[1] + ((factory_stick_cal[1] << 8) & 0xF00 | factory_stick_cal[0]);
		stick_cal_y_l[2] = stick_cal_y_l[1] + ((factory_stick_cal[2] << 4) | (factory_stick_cal[2] >> 4));

	}
	else
	{
		stick_cal_x_r[1] = (factory_stick_cal[10] << 8) & 0xF00 | factory_stick_cal[9];
		stick_cal_y_r[1] = (factory_stick_cal[11] << 4) | (factory_stick_cal[10] >> 4);
		stick_cal_x_r[0] = stick_cal_x_r[1] - ((factory_stick_cal[13] << 8) & 0xF00 | factory_stick_cal[12]);
		stick_cal_y_r[0] = stick_cal_y_r[1] - ((factory_stick_cal[14] << 4) | (factory_stick_cal[13] >> 4));
		stick_cal_x_r[2] = stick_cal_x_r[1] + ((factory_stick_cal[16] << 8) & 0xF00 | factory_stick_cal[15]);
		stick_cal_y_r[2] = stick_cal_y_r[1] + ((factory_stick_cal[17] << 4) | (factory_stick_cal[16] >> 4));
	}

	// get gyro / accelerometer calibration data:
	// factory calibration:

	// Acc cal origin position
	sensor_cal[0][0] = uint16_to_int16(factory_sensor_cal[0] | factory_sensor_cal[1] << 8);
	sensor_cal[0][1] = uint16_to_int16(factory_sensor_cal[2] | factory_sensor_cal[3] << 8);
	sensor_cal[0][2] = uint16_to_int16(factory_sensor_cal[4] | factory_sensor_cal[5] << 8);

	// Gyro cal origin position
	sensor_cal[1][0] = uint16_to_int16(factory_sensor_cal[0xC] | factory_sensor_cal[0xD] << 8);
	sensor_cal[1][1] = uint16_to_int16(factory_sensor_cal[0xE] | factory_sensor_cal[0xF] << 8);
	sensor_cal[1][2] = uint16_to_int16(factory_sensor_cal[0x10] | factory_sensor_cal[0x11] << 8);

	// Use SPI calibration and convert them to SI acc unit
	acc_cal_coeff[0] = (float)(1.0 / (float)(16384 - uint16_to_int16(sensor_cal[0][0]))) * 4.0f  * 9.8f;
	acc_cal_coeff[1] = (float)(1.0 / (float)(16384 - uint16_to_int16(sensor_cal[0][1]))) * 4.0f  * 9.8f;
	acc_cal_coeff[2] = (float)(1.0 / (float)(16384 - uint16_to_int16(sensor_cal[0][2]))) * 4.0f  * 9.8f;

	// Use SPI calibration and convert them to SI gyro unit
	gyro_cal_coeff[0] = (float)(936.0 / (float)(13371 - uint16_to_int16(sensor_cal[1][0])) * 0.01745329251994);
	gyro_cal_coeff[1] = (float)(936.0 / (float)(13371 - uint16_to_int16(sensor_cal[1][1])) * 0.01745329251994);
	gyro_cal_coeff[2] = (float)(936.0 / (float)(13371 - uint16_to_int16(sensor_cal[1][2])) * 0.01745329251994);

	hid_set_nonblocking(this->handle, 1);
	printf("Get Calibration Data Success!");
}

wchar_t * JoyconBase::GetSerial()
{
	return this->serial;
}

std::string JoyconBase::GetName()
{
	return this->name;
}

SIDE JoyconBase::GetSide()
{
	return this->leftOrRight;
}

bool JoyconBase::IsValid()
{
	return this->isValid;
}

float JoyconBase::GetStickX()
{
	return this->stick.CalX;
}

float JoyconBase::GetStickY()
{
	return this->stick.CalY;
}

bool JoyconBase::GetButtonState(BUTTON_NAME b)
{
	return this->buttonState[b];
}

std::vector<BUTTON_NAME> JoyconBase::GetPressedButton()
{
	return this->pressedButton;
}

void JoyconBase::hid_exchange(hid_device * handle, unsigned char * buf, int len)
{
	if (this->IsValid())
	{
		int res;
		res = hid_write(handle, buf, len);
		res = hid_read(handle, buf, 0x40);
	}
}

void JoyconBase::send_command(int command, uint8_t * data, int len)
{
	unsigned char buf[0x40];
	memset(buf, 0, 0x40);

	buf[0x0] = command;

	if (data != nullptr && len != 0) {
		memcpy(buf + 0x1, data, len);
	}

	hid_exchange(this->handle, buf, len + 0x1);

	if (data) {
		memcpy(data, buf, 0x40);
	}

}

void JoyconBase::send_subcommand(int command, int subcommand, uint8_t * data, int len)
{
	unsigned char buf[0x40];
	memset(buf, 0, 0x40);

	uint8_t rumble_base[9] = { (++this->global_count) & 0xF, 0x00, 0x01, 0x40, 0x40, 0x00, 0x01, 0x40, 0x40 };
	memcpy(buf, rumble_base, 9);

	if (global_count > 0xF) {
		global_count = 0x0;
	}

	buf[9] = subcommand;
	if (data && len != 0) {
		memcpy(buf + 10, data, len);
	}

	send_command(command, buf, 10 + len);

	if (data) {
		memcpy(data, buf, 0x40); //TODO
	}
}

void JoyconBase::rumble(int frequency, int intensity)
{
	unsigned char buf[0x40];
	memset(buf, 0, 0x40);

	// intensity: (0, 8)
	// frequency: (0, 255)

	//	 X	AA	BB	 Y	CC	DD
	//[0 1 x40 x40 0 1 x40 x40] is neutral.

	buf[1 + 0 + intensity] = 0x1;
	buf[1 + 4 + intensity] = 0x1;

	// Set frequency to increase
	if (this->leftOrRight == SIDE::LEFT) {
		buf[1 + 0] = frequency;// (0, 255)
	}
	else {
		buf[1 + 4] = frequency;// (0, 255)
	}

	// set non-blocking:
	hid_set_nonblocking(this->handle, 1);
	send_command(0x10, (uint8_t*)buf, 0x40);
}

void JoyconBase::rumble2(uint16_t hf, uint8_t hfa, uint8_t lf, uint16_t lfa)
{
	unsigned char buf[0x400];
	memset(buf, 0, 0x400);

	int off = 0;// offset
	if (this->leftOrRight == SIDE::RIGHT) {
		off = 4;
	}

	// Byte swapping
	buf[0 + off] = hf & 0xFF;
	buf[1 + off] = hfa + ((hf >> 8) & 0xFF); //Add amp + 1st byte of frequency to amplitude byte

	// Byte swapping
	buf[2 + off] = lf + ((lfa >> 8) & 0xFF); //Add freq + 1st byte of LF amplitude to the frequency byte
	buf[3 + off] = lfa & 0xFF;

	// set non-blocking:
	hid_set_nonblocking(this->handle, 1);

	send_command(0x10, (uint8_t*)buf, 0x9);
}

void JoyconBase::rumble3(float frequency, uint8_t hfa, uint16_t lfa)
{
	//Float frequency to hex conversion
	if (frequency < 0.0f) {
		frequency = 0.0f;
	}
	else if (frequency > 1252.0f) {
		frequency = 1252.0f;
	}
	uint8_t encoded_hex_freq = (uint8_t)round(log2((double)frequency / 10.0)*32.0);

	//uint16_t encoded_hex_freq = (uint16_t)floor(-32 * (0.693147f - log(frequency / 5)) / 0.693147f + 0.5f); // old

	//Convert to Joy-Con HF range. Range in big-endian: 0x0004-0x01FC with +0x0004 steps.
	uint16_t hf = (encoded_hex_freq - 0x60) * 4;
	//Convert to Joy-Con LF range. Range: 0x01-0x7F.
	uint8_t lf = encoded_hex_freq - 0x40;

	rumble2(hf, hfa, lf, lfa);
}

void JoyconBase::rumble4(float real_LF, float real_HF, uint8_t hfa, uint16_t lfa)
{
	real_LF = clamp(real_LF, 40.875885f, 626.286133f);
	real_HF = clamp(real_HF, 81.75177, 1252.572266f);

	////Float frequency to hex conversion
	//if (frequency < 0.0f) {
	//	frequency = 0.0f;
	//} else if (frequency > 1252.0f) {
	//	frequency = 1252.0f;
	//}
	//uint8_t encoded_hex_freq = (uint8_t)round(log2((double)frequency / 10.0)*32.0);

	//uint16_t encoded_hex_freq = (uint16_t)floor(-32 * (0.693147f - log(frequency / 5)) / 0.693147f + 0.5f); // old

	////Convert to Joy-Con HF range. Range in big-endian: 0x0004-0x01FC with +0x0004 steps.
	//uint16_t hf = (encoded_hex_freq - 0x60) * 4;
	////Convert to Joy-Con LF range. Range: 0x01-0x7F.
	//uint8_t lf = encoded_hex_freq - 0x40;

	uint16_t hf = ((uint8_t)round(log2((double)real_HF * 0.01)*32.0) - 0x60) * 4;
	uint8_t lf = (uint8_t)round(log2((double)real_LF * 0.01)*32.0) - 0x40;

	rumble2(hf, hfa, lf, lfa);
}

void JoyconBase::rumble_freq(uint16_t hf, uint8_t hfa, uint8_t lf, uint16_t lfa)
{
	unsigned char buf[0x400];
	memset(buf, 0, 0x40);

	//int hf		= HF;
	//int hf_amp	= HFA;
	//int lf		= LF;
	//int lf_amp	= LFA;
	// maybe:
	//int hf_band = hf + hf_amp;

	int off = 0;// offset
	if (this->leftOrRight == SIDE::RIGHT) {
		off = 4;
	}

	// Byte swapping
	buf[0 + off] = hf & 0xFF;
	buf[1 + off] = hfa + ((hf >> 8) & 0xFF); //Add amp + 1st byte of frequency to amplitude byte

	// Byte swapping
	buf[2 + off] = lf + ((lfa >> 8) & 0xFF); //Add freq + 1st byte of LF amplitude to the frequency byte
	buf[3 + off] = lfa & 0xFF;

	// set non-blocking:
	hid_set_nonblocking(this->handle, 1);

	send_command(0x10, (uint8_t*)buf, 0x9);
}

void JoyconBase::setGyroOffsets()
{
	float thresh = 0.1;
	if (abs(this->gyro.roll) > thresh || abs(this->gyro.pitch) > thresh || abs(this->gyro.yaw) > thresh) {
		return;
	}

	//average = current + ((newData - current) / n);
	this->gyro.offset.n += 1;
	this->gyro.offset.roll = this->gyro.offset.roll + ((this->gyro.roll - this->gyro.offset.roll) / this->gyro.offset.n);
	this->gyro.offset.pitch = this->gyro.offset.pitch + ((this->gyro.pitch - this->gyro.offset.pitch) / this->gyro.offset.n);
	this->gyro.offset.yaw = this->gyro.offset.yaw + ((this->gyro.yaw - this->gyro.offset.yaw) / this->gyro.offset.n);
	//this->gyro.offset.roll	= this->gyro.roll;
	//this->gyro.offset.pitch = this->gyro.pitch;
	//this->gyro.offset.yaw	= this->gyro.yaw;
}

void JoyconBase::CalcAnalogStick()
{
	if (this->leftOrRight == SIDE::LEFT) 
	{
		CalcAnalogStick2(
			this->stick.CalX,
			this->stick.CalY,
			this->stick.x,
			this->stick.y,
			this->stick_cal_x_l,
			this->stick_cal_y_l);
	}
	else
	{
		CalcAnalogStick2(
			this->stick.CalX,
			this->stick.CalY,
			this->stick.x,
			this->stick.y,
			this->stick_cal_x_r,
			this->stick_cal_y_r);

	}
}

void JoyconBase::CalcAnalogStick2(float & pOutX, float & pOutY, uint16_t x, uint16_t y, uint16_t x_calc[3], uint16_t y_calc[3])
{
	float x_f, y_f;
	// Apply Joy-Con center deadzone. 0xAE translates approx to 15%. Pro controller has a 10% () deadzone
	float deadZoneCenter = 0.15f;
	// Add a small ammount of outer deadzone to avoid edge cases or machine variety.
	float deadZoneOuter = 0.10f;

	// convert to float based on calibration and valid ranges per +/-axis
	x = clamp(x, x_calc[0], x_calc[2]);
	y = clamp(y, y_calc[0], y_calc[2]);
	if (x >= x_calc[1]) {
		x_f = (float)(x - x_calc[1]) / (float)(x_calc[2] - x_calc[1]);
	}
	else {
		x_f = -((float)(x - x_calc[1]) / (float)(x_calc[0] - x_calc[1]));
	}
	if (y >= y_calc[1]) {
		y_f = (float)(y - y_calc[1]) / (float)(y_calc[2] - y_calc[1]);
	}
	else {
		y_f = -((float)(y - y_calc[1]) / (float)(y_calc[0] - y_calc[1]));
	}

	// Interpolate zone between deadzones
	float mag = sqrtf(x_f*x_f + y_f * y_f);
	if (mag > deadZoneCenter) {
		// scale such that output magnitude is in the range [0.0f, 1.0f]
		float legalRange = 1.0f - deadZoneOuter - deadZoneCenter;
		float normalizedMag = min(1.0f, (mag - deadZoneCenter) / legalRange);
		float scale = normalizedMag / mag;
		pOutX = (x_f * scale);
		pOutY = (y_f * scale);
	}
	else {
		// stick is in the inner dead zone
		pOutX = 0.0f;
		pOutY = 0.0f;
	}
}

int JoyconBase::get_spi_data(uint32_t offset, const uint16_t read_len, uint8_t * test_buf)
{
	int res;
	uint8_t buf[0x100];
	while (1) {
		memset(buf, 0, sizeof(buf));
		auto hdr = (brcm_hdr *)buf;
		auto pkt = (brcm_cmd_01 *)(hdr + 1);
		hdr->cmd = 1;
		hdr->rumble[0] = timing_byte;

		buf[1] = timing_byte;

		timing_byte++;
		if (timing_byte > 0xF) {
			timing_byte = 0x0;
		}
		pkt->subcmd = 0x10;
		pkt->offset = offset;
		pkt->size = read_len;

		for (int i = 11; i < 22; ++i) {
			buf[i] = buf[i + 3];
		}

		res = hid_write(handle, buf, sizeof(*hdr) + sizeof(*pkt));

		res = hid_read(handle, buf, sizeof(buf));

		if ((*(uint16_t*)&buf[0xD] == 0x1090) && (*(uint32_t*)&buf[0xF] == offset)) {
			break;
		}
	}
	if (res >= 0x14 + read_len) {
		for (int i = 0; i < read_len; i++) {
			test_buf[i] = buf[0x14 + i];
		}
	}

	return 0;
}

int JoyconBase::write_spi_data(uint32_t offset, const uint16_t write_len, uint8_t * test_buf)
{
	int res;
	uint8_t buf[0x100];
	int error_writing = 0;
	while (1) {
		memset(buf, 0, sizeof(buf));
		auto hdr = (brcm_hdr *)buf;
		auto pkt = (brcm_cmd_01 *)(hdr + 1);
		hdr->cmd = 1;
		hdr->rumble[0] = timing_byte;
		timing_byte++;
		if (timing_byte > 0xF) {
			timing_byte = 0x0;
		}
		pkt->subcmd = 0x11;
		pkt->offset = offset;
		pkt->size = write_len;
		for (int i = 0; i < write_len; i++) {
			buf[0x10 + i] = test_buf[i];
		}
		res = hid_write(handle, buf, sizeof(*hdr) + sizeof(*pkt) + write_len);

		res = hid_read(handle, buf, sizeof(buf));

		if (*(uint16_t*)&buf[0xD] == 0x1180)
			break;

		error_writing++;
		if (error_writing == 125) {
			return 1;
		}
	}
	return 0;
}

void JoyconBase::debug_receive_data(bool mute)
{
	memset(this->buf, 0, 0x41);
	this->send_command(0x1E, this->buf, 0);
	while (hid_read(this->handle, this->buf, 0x40) > 0) {};
	this->pressedButton.clear();

	if (!mute) {
		printf("\n");
		for (uint8_t i = 0; i < 0x41; i++) {
			printf("%u", this->buf[i]);
		}
		printf("\n");
	}


	if (this->leftOrRight == SIDE::LEFT)
	{
		uint16_t stick_x = this->buf[6] | ((this->buf[7] & 0xF) << 8);
		uint16_t stick_y = (this->buf[7] >> 4) | (this->buf[8] << 4);
		this->stick.x = stick_x;
		this->stick.y = stick_y;


		uint16_t state = (this->buf[4] << 8) | this->buf[5];
		uint16_t button_list[] = { 128, 64, 1 << 8, 8 << 8, 2, 1, 8, 4, 32 << 8, 32, 16 };
		char button_name[] = "ZL-|8246C<>";

		if (!mute) {
			printf("This is Joycon(L)\n");
			printf("stick_x: %d  ", stick_x);
			printf("stick_y: %d  ", stick_y);
			for (int i = 0; i < ((sizeof(button_list)) / (sizeof(button_list[0]))); i++)
			{
				//printf("state&button_list[%d] = %d", i, state & button_list[i]);
				if (state & button_list[i])
					printf("%c ", button_name[i]);
			}
			printf("\n");
		}

	}
	else
	{
		//uint16_t stick_x = this->buf[6] | ((this->buf[7] & 0xF) << 8);
		//uint16_t stick_y = (this->buf[7] >> 4) | (this->buf[8] << 4);

		//uint16_t state = (this->buf[3] << 8) | this->buf[4];
		//uint16_t button_list[] = { 128 << 8, 64 << 8, 2, 2 << 8, 1 << 8, 8 << 8, 4 << 8, 4, 16 };
		//char button_name[] = "ZR+XYAB|H";

		//if (!mute) {
		//	printf("This is Joycon(R)\n");
		//	printf("stick_x: %d  ", stick_x);
		//	printf("stick_y: %d  ", stick_y);

		//	for (int i = 0; i < ((sizeof(button_list)) / (sizeof(button_list[0]))); i++)
		//	{
		//		//printf("state&button_list[%d] = %d", i, state & button_list[i]);
		//		if (state & button_list[i])
		//			//printf("%c ", button_name[i]);
		//			0;
		//	}
		//	printf("\n");
		//}

		uint16_t stick_x = this->buf[9] | ((this->buf[10] & 0xF) << 8);
		uint16_t stick_y = (this->buf[10] >> 4) | (this->buf[11] << 4);
		this->stick.x = stick_x;
		this->stick.y = stick_y;

		uint16_t state = (this->buf[3] << 8) | this->buf[4];
		uint16_t button_list[] = { 128 << 8, 64 << 8, 2, 2 << 8, 1 << 8, 8 << 8, 4 << 8, 4, 16, 16<<8, 32<<8 };
		char button_name[] = "ZR+XYAB|H<>";

		if (!mute) {
			printf("This is Joycon(L)\n");
			printf("stick_x: %d  ", stick_x);
			printf("stick_y: %d  ", stick_y);
			for (int i = 0; i < ((sizeof(button_list)) / (sizeof(button_list[0]))); i++)
			{
				//printf("state&button_list[%d] = %d", i, state & button_list[i]);
				if (state & button_list[i])
					printf("%c ", button_name[i]);
			}
			printf("\n");
		}
	}
}

void JoyconBase::Poll()
{
	memset(this->buf, 0, 0x41);
	this->send_command(0x1E, this->buf, 0);
	while (hid_read(this->handle, this->buf, 0x40) > 0) {};
	this->pressedButton.clear();

	if (this->leftOrRight == SIDE::LEFT)
	{
		uint16_t stick_x = this->buf[6] | ((this->buf[7] & 0xF) << 8);
		uint16_t stick_y = (this->buf[7] >> 4) | (this->buf[8] << 4);
		this->stick.x = stick_x;
		this->stick.y = stick_y;

		uint16_t state = (this->buf[4] << 8) | this->buf[5];
		uint16_t button_list[] = { 128, 64, 1 << 8, 8 << 8, 2, 1, 8, 4, 32 << 8, 32, 16 };
		BUTTON_NAME button_name[] = { ZL_ZR, L_R, MINUS_PLUS, STICK_BUTTON, UP_X, DOWN_B, LEFT_Y, RIGHT_A, CAPTURE_HOME, SL, SR };

		for (int i = 0; i < ((sizeof(button_list)) / (sizeof(button_list[0]))); i++)
		{
			if (state & button_list[i]) {
				this->buttonState[button_name[i]] = true;
				this->pressedButton.push_back(button_name[i]);
			}
			else {
				this->buttonState[button_name[i]] = false;
			}
		}
	}
	else
	{
		uint16_t stick_x = this->buf[6] | ((this->buf[7] & 0xF) << 8);
		uint16_t stick_y = (this->buf[7] >> 4) | (this->buf[8] << 4);
		this->stick.x = stick_x;
		this->stick.y = stick_y;

		uint16_t state = (this->buf[3] << 8) | this->buf[4];
		uint16_t button_list[] = { 128 << 8, 64 << 8, 2, 4, 2 << 8, 4 << 8, 1 << 8, 8 << 8, 16, 32 << 8, 16 << 8 };
		BUTTON_NAME button_name[] = { ZL_ZR, L_R, MINUS_PLUS, STICK_BUTTON, UP_X, DOWN_B, LEFT_Y, RIGHT_A, CAPTURE_HOME, SL, SR };

		for (int i = 0; i < ((sizeof(button_list)) / (sizeof(button_list[0]))); i++)
		{
			if (state & button_list[i]) {
				this->buttonState[button_name[i]] = true;
				this->pressedButton.push_back(button_name[i]);
			}
			else {
				this->buttonState[button_name[i]] = false;
			}
		}
	}

	this->CalcAnalogStick();
}
