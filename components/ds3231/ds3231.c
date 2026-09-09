#include "ds3231.h"

#include <esp_log.h>

#include "i2c.h"

static const char *TAG = "DS3231";

static void *ds3231_handle = NULL;

static ds3231_hour_mode_t s_hour_mode = DS3231_24HOUR_MODE;
static ds3231_century_t s_century = DS3231_CENTURY_0;

static inline uint8_t bcd_to_dec(uint8_t bcd) {
	return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

void ds3231_init() {
	ds3231_handle = i2c_master_add_device(0x68, 100000);

	ds3231_get_hour_mode(&s_hour_mode);
	ds3231_get_century(&s_century);

	ESP_LOGI(TAG, "Hour mode: %s",
			 s_hour_mode == DS3231_24HOUR_MODE ? "24-hour" : "12-hour AM/PM");
	ESP_LOGI(TAG, "Century: %" PRId8, s_century);
}

void ds3231_get_sec(uint8_t *seconds) {
	uint8_t reg_addr = 0x00;
	i2c_master_device_trans_recv(ds3231_handle, &reg_addr, 1, seconds, 1);
	*seconds = bcd_to_dec(*seconds);
}

void ds3231_get_min(uint8_t *minutes) {
	uint8_t reg_addr = 0x01;
	i2c_master_device_trans_recv(ds3231_handle, &reg_addr, 1, minutes, 1);
	*minutes = bcd_to_dec(*minutes);
}

void ds3231_get_hour(uint8_t *hours) {
	uint8_t reg_addr = 0x02;
	uint8_t _hours;
	i2c_master_device_trans_recv(ds3231_handle, &reg_addr, 1, &_hours, 1);

	if (s_hour_mode == DS3231_24HOUR_MODE) {
		_hours = bcd_to_dec(_hours & 0x3F);
	} else {
		_hours = bcd_to_dec(_hours & 0x1F);
	}

	*hours = _hours;
}

void ds3231_get_hour_mode(ds3231_hour_mode_t *hour_mode) {
	uint8_t reg_addr = 0x02;
	uint8_t _hour_mode;
	i2c_master_device_trans_recv(ds3231_handle, &reg_addr, 1, &_hour_mode, 1);
	*hour_mode = (ds3231_hour_mode_t)(_hour_mode & (1 << 6));
}

void ds3231_get_hour_meridiem(ds3231_meridiem_t *meridiem) {
	if (s_hour_mode != DS3231_12HOUR_MODE) {
		*meridiem = DS3231_MERIDIEM_NA;
		return;
	}

	uint8_t reg_addr = 0x02;
	uint8_t _meridiem;
	i2c_master_device_trans_recv(ds3231_handle, &reg_addr, 1, &_meridiem, 1);
	*meridiem = (ds3231_meridiem_t)(_meridiem & 0x20);
}

void ds3231_get_day(uint8_t *day) {
	uint8_t reg_addr = 0x03;
	i2c_master_device_trans_recv(ds3231_handle, &reg_addr, 1, day, 1);
	*day = bcd_to_dec(*day);
}

void ds3231_get_date(uint8_t *date) {
	uint8_t reg_addr = 0x04;
	i2c_master_device_trans_recv(ds3231_handle, &reg_addr, 1, date, 1);
	*date = bcd_to_dec(*date);
}

void ds3231_get_month(uint8_t *month) {
	uint8_t reg_addr = 0x05;
	uint8_t _month;
	i2c_master_device_trans_recv(ds3231_handle, &reg_addr, 1, &_month, 1);
	*month = bcd_to_dec(_month & 0x1F);
}

void ds3231_get_century(ds3231_century_t *century) {
	uint8_t reg_addr = 0x05;
	uint8_t _century;
	i2c_master_device_trans_recv(ds3231_handle, &reg_addr, 1, &_century, 1);
	*century = (ds3231_century_t)((_century >> 7) & 0x01);
}
