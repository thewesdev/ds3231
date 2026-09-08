#include "ds3231.h"

#include "i2c.h"

static void *ds3231_handle = NULL;

static inline uint8_t bcd_to_dec(uint8_t bcd) {
	return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

void ds3231_init() { ds3231_handle = i2c_master_add_device(0x68, 100000); }

void ds3231_get_sec(uint8_t *seconds) {
	uint8_t reg_addr = 0x00;
	i2c_master_device_trans_recv(ds3231_handle, &reg_addr, 1, seconds, 1);
	*seconds = bcd_to_dec(*seconds);
}
