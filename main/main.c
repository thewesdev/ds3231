#include <stdio.h>

#include <esp_log.h>
#include <freertos/FreeRTOS.h>

#include "ds3231.h"
#include "i2c.h"

void app_main(void) {
	i2c_master_init();
	ds3231_init();

	while (1) {
		uint8_t sec;
		uint8_t min;
		uint8_t hour;
		uint8_t day;
		uint8_t date;
		uint8_t month;

		ds3231_get_sec(&sec);
		ds3231_get_min(&min);
		ds3231_get_hour(&hour);
		ds3231_get_day(&day);
		ds3231_get_date(&date);
		ds3231_get_month(&month);

		printf("%02" PRIu8 " %02" PRIu8 " %02" PRIu8 " %02" PRIu8 ":%02" PRIu8
			   ":%02" PRIu8 "\n",
			   month, date, day, hour, min, sec);

		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
