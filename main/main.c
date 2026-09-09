#include <stdio.h>

#include <esp_log.h>
#include <freertos/FreeRTOS.h>

#include "ds3231.h"
#include "i2c.h"

void app_main(void) {
	i2c_master_init();
	ds3231_init();

	ds3231_hour_mode_t hour_mode = DS3231_12HOUR_MODE;

	ds3231_set_hour_mode(hour_mode);

	// ds3231_set_year(26);
	// ds3231_set_month(9);
	// ds3231_set_date(9);
	// ds3231_set_day(4);
	// ds3231_set_hour(14);
	// ds3231_set_min(10);
	// ds3231_set_sec(20);

	while (1) {
		uint8_t sec;
		uint8_t min;
		uint8_t hour;
		uint8_t day;
		uint8_t date;
		uint8_t month;
		uint8_t year;
		ds3231_meridiem_t meridiem;

		ds3231_get_sec(&sec);
		ds3231_get_min(&min);
		ds3231_get_hour(&hour);
		ds3231_get_day(&day);
		ds3231_get_date(&date);
		ds3231_get_month(&month);
		ds3231_get_year(&year);
		ds3231_get_hour_meridiem(&meridiem);

		if (hour_mode == DS3231_24HOUR_MODE) {
			printf("%02" PRIu8 " %02" PRIu8 " %02" PRIu8 " %02" PRIu8
				   " %02" PRIu8 ":%02" PRIu8 ":%02" PRIu8 "\n",
				   year, month, date, day, hour, min, sec);
		} else {
			printf("%02" PRIu8 " %02" PRIu8 " %02" PRIu8 " %02" PRIu8 " %s"
				   " %02" PRIu8 ":%02" PRIu8 ":%02" PRIu8 "\n",
				   year, month, date, day, meridiem == DS3231_AM ? "AM" : "PM",
				   hour, min, sec);
		}

		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
