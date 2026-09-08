#ifndef COMPONENT_DS3231_H
#define COMPONENT_DS3231_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

void ds3231_init();
void ds3231_get_sec(uint8_t *seconds);

#ifdef __cplusplus
}
#endif

#endif // COMPONENT_DS3231_H
