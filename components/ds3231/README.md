# DS3231

## Features

- ds3231 init with add_device function
- reading and writing seconds, minutes, hours, day, date, month and year
- reading and writing hour_mode, hour_meridiem and century
- reading and writing osf (Oscillator Stop Flag) status

## I2C

address: 0x68
speed: 100 kHz

## Supported Data

| Data          | Function                                                     | Format  |
| ------------- | ------------------------------------------------------------ | ------- |
| Seconds       | `void ds3231_get_sec(uint8_t *seconds)`                      | Decimal |
| Minutes       | `void ds3231_get_min(uint8_t *minutes)`                      | Decimal |
| Hours         | `void ds3231_get_hour(uint8_t *hours)`                       | Decimal |
| Day           | `void ds3231_get_day(uint8_t *day)`                          | Decimal |
| Date          | `void ds3231_get_date(uint8_t *date)`                        | Decimal |
| Month         | `void ds3231_get_month(uint8_t *month)`                      | Decimal |
| Year          | `void ds3231_get_year(uint8_t *year)`                        | Decimal |
| Hour Mode     | `void ds3231_get_hour_mode(ds3231_hour_mode_t *hour_mode)`   | Enum    |
| Hour Meridiem | `void ds3231_get_hour_meridiem(ds3231_meridiem_t *meridiem)` | Enum    |
| Century       | `void ds3231_get_century(ds3231_century_t *century)`         | Enum    |
| OSF           | `void ds3231_status_get_osf(ds3231_osf_t *osf)`              | Enum    |

## Dependencies

- ESP-IDF
- thewesdev/i2c

## Installation

### IDF Component Registry

```bash
idf.py add-dependency thewesdev/ds3231
```

For update newest versions

```bash
idf.py update-dependencies
```

### Github

```bash
git clone https://github.com/thewesdev/ds3231
```

## Code Examples

```c
#include <stdio.h>

#include <esp_log.h>
#include <freertos/FreeRTOS.h>

#include "i2c.h"
#include "ds3231.h"

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
		uint8_t year;

		ds3231_get_sec(&sec);
		ds3231_get_min(&min);
		ds3231_get_hour(&hour);
		ds3231_get_day(&day);
		ds3231_get_date(&date);
		ds3231_get_month(&month);
		ds3231_get_year(&year);

		printf("%02" PRIu8 " %02" PRIu8 " %02" PRIu8 " %02" PRIu8 " %02" PRIu8
			   ":%02" PRIu8 ":%02" PRIu8 "\n",
			   year, month, date, day, hour, min, sec);

		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
```
