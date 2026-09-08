#include <stdio.h>

#include <freertos/FreeRTOS.h>

#include "ds3231.h"
#include "i2c.h"

void app_main(void) {
	i2c_master_init();
	ds3231_init();

	while (1) {
		uint8_t sec;

		ds3231_get_sec(&sec);

		printf("Seconds: %" PRIu8 "\n", sec);

		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
