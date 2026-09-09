#ifndef COMPONENT_DS3231_H
#define COMPONENT_DS3231_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	DS3231_24HOUR_MODE = 0x00,
	DS3231_12HOUR_MODE = 0x40,
} ds3231_hour_mode_t;

typedef enum {
	DS3231_AM = 0x00,
	DS3231_PM = 0x20,
	DS3231_MERIDIEM_NA = 0xFF,
} ds3231_meridiem_t;

typedef enum {
	DS3231_CENTURY_0,
	DS3231_CENTURY_1,
} ds3231_century_t;

void ds3231_init();
void ds3231_get_sec(uint8_t *seconds);
void ds3231_get_min(uint8_t *minutes);
void ds3231_get_hour(uint8_t *hours);
void ds3231_get_hour_mode(ds3231_hour_mode_t *hour_mode);
void ds3231_get_hour_meridiem(ds3231_meridiem_t *meridiem);
void ds3231_get_day(uint8_t *day);
void ds3231_get_date(uint8_t *date);
void ds3231_get_month(uint8_t *month);
void ds3231_get_century(ds3231_century_t *century);
void ds3231_get_year(uint8_t *year);

#ifdef __cplusplus
}
#endif

#endif // COMPONENT_DS3231_H
