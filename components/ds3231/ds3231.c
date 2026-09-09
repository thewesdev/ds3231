#include "ds3231.h"

#include <esp_log.h>

#include "i2c.h"

static const char *TAG = "DS3231";

static void *ds3231_handle = NULL;

static inline uint8_t bcd_to_dec(uint8_t bcd) {
	return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

static inline uint8_t dec_to_bcd(uint8_t dec) {
	return ((dec / 10) << 4) | (dec % 10);
}

void ds3231_init() {
	ds3231_handle = i2c_master_add_device(0x68, 100000);

	ds3231_hour_mode_t hour_mode = DS3231_24HOUR_MODE;
	ds3231_century_t century = DS3231_CENTURY_0;
	ds3231_osf_t osf = DS3231_NON_STOP;

	ds3231_get_hour_mode(&hour_mode);
	ds3231_get_century(&century);
	ds3231_status_get_osf(&osf);

	ESP_LOGI(TAG, "Hour mode: %s",
			 hour_mode == DS3231_24HOUR_MODE ? "24-hour" : "12-hour AM/PM");
	ESP_LOGI(TAG, "Century: %" PRId8, century == DS3231_CENTURY_0 ? 0 : 1);
	ESP_LOGI(TAG, "OSF: %" PRId8, osf == DS3231_NON_STOP ? 0 : 1);
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

	if (_hours & DS3231_12HOUR_MODE) {
		_hours = bcd_to_dec(_hours & 0x1F);
	} else {
		_hours = bcd_to_dec(_hours & 0x3F);
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
	uint8_t reg_addr = 0x02;
	uint8_t _meridiem;
	i2c_master_device_trans_recv(ds3231_handle, &reg_addr, 1, &_meridiem, 1);

	if (!(_meridiem & DS3231_12HOUR_MODE)) {
		*meridiem = DS3231_MERIDIEM_NA;
		return;
	}

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
	*century = (ds3231_century_t)(_century & 0x80);
}

void ds3231_get_year(uint8_t *year) {
	uint8_t reg_addr = 0x06;
	i2c_master_device_trans_recv(ds3231_handle, &reg_addr, 1, year, 1);
	*year = bcd_to_dec(*year);
}

void ds3231_status_get_osf(ds3231_osf_t *osf) {
	uint8_t reg_addr = 0x0F;
	uint8_t reg_value;
	i2c_master_device_trans_recv(ds3231_handle, &reg_addr, 1, &reg_value, 1);
	*osf = (ds3231_osf_t)(reg_value & 0x80);
}

void ds3231_status_set_osf(ds3231_osf_t osf) {
	uint8_t reg_addr = 0x0F;
	uint8_t reg_value;
	i2c_master_device_trans_recv(ds3231_handle, &reg_addr, 1, &reg_value, 1);

	if (osf == (reg_value & 0x80))
		return;

	uint8_t buffer[2] = {0x0F, osf | (reg_value & 0x0F)};
	i2c_master_device_trans(ds3231_handle, buffer, 2);
}

void ds3231_set_sec(uint8_t seconds) {
	uint8_t buffer[2] = {0x00, dec_to_bcd(seconds)};
	i2c_master_device_trans(ds3231_handle, buffer, 2);
}

void ds3231_set_min(uint8_t minutes) {
	uint8_t buffer[2] = {0x01, dec_to_bcd(minutes)};
	i2c_master_device_trans(ds3231_handle, buffer, 2);
}

void ds3231_set_hour(uint8_t hours) {
	if (hours > 23)
		return;

	ds3231_hour_mode_t hour_mode;

	ds3231_get_hour_mode(&hour_mode);

	uint8_t buffer[2];

	buffer[0] = 0x02;

	if (hour_mode == DS3231_24HOUR_MODE) {
		buffer[1] = dec_to_bcd(hours);
	} else {
		if (hours == 0) {
			buffer[1] = (1 << 6) | dec_to_bcd(12);
		} else if (hours > 0 && hours <= 12) {
			buffer[1] = (1 << 6) | dec_to_bcd(hours);
		} else {
			buffer[1] = (1 << 6) | (1 << 5) | (dec_to_bcd(hours - 12));
		}
	}

	i2c_master_device_trans(ds3231_handle, buffer, 2);
}

void ds3231_set_hour_mode(ds3231_hour_mode_t hour_mode) { // concertar
	uint8_t reg_addr = 0x02;
	uint8_t hour;

	i2c_master_device_trans_recv(ds3231_handle, &reg_addr, 1, &hour, 1);

	ds3231_hour_mode_t _hour_mode = (ds3231_hour_mode_t)(hour & (1 << 6));
	if (_hour_mode == hour_mode)
		return;

	if (hour_mode == DS3231_12HOUR_MODE) {
		uint8_t hour_value = bcd_to_dec(hour & 0x3F);
		ds3231_meridiem_t meridiem = DS3231_AM;

		if (hour_value == 0) {
			hour_value = 12;
			meridiem = DS3231_AM;
		} else if (hour_value > 0 && hour_value < 12) {
			meridiem = DS3231_AM;
		} else if (hour_value == 12) {
			meridiem = DS3231_PM;
		} else {
			hour_value -= 12;
			meridiem = DS3231_PM;
		}

		hour = (1 << 6) | meridiem | dec_to_bcd(hour_value);
	} else {
		uint8_t hour_value = bcd_to_dec(hour & 0x1F);
		ds3231_meridiem_t meridiem = hour & 0x20;

		if (meridiem == DS3231_AM && hour_value == 12) {
			hour_value = 0;
		} else if (meridiem == DS3231_PM && hour_value >= 1 &&
				   hour_value < 12) {
			hour_value += 12;
		}

		hour = dec_to_bcd(hour_value);
	}

	uint8_t buffer[2] = {0x02, hour};
	i2c_master_device_trans(ds3231_handle, buffer, 2);
}

void ds3231_set_day(uint8_t day) {
	uint8_t buffer[2] = {0x03, dec_to_bcd(day)};
	i2c_master_device_trans(ds3231_handle, buffer, 2);
}

void ds3231_set_date(uint8_t date) {
	uint8_t buffer[2] = {0x04, dec_to_bcd(date)};
	i2c_master_device_trans(ds3231_handle, buffer, 2);
}

void ds3231_set_month(uint8_t month) {
	uint8_t reg_addr = 0x05;
	uint8_t _month;

	i2c_master_device_trans_recv(ds3231_handle, &reg_addr, 1, &_month, 1);

	_month = _month & 0x80;

	uint8_t buffer[2] = {0x05, _month | dec_to_bcd(month)};
	i2c_master_device_trans(ds3231_handle, buffer, 2);
}

void ds3231_set_year(uint8_t year) {
	uint8_t buffer[2] = {0x06, dec_to_bcd(year)};
	i2c_master_device_trans(ds3231_handle, buffer, 2);
}
