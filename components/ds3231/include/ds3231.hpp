#ifndef COMPONENT_DS3231_HPP
#define COMPONENT_DS3231_HPP

#include "ds3231.h"

namespace ds3231 {
	inline void init() { return ds3231_init(); }

	namespace get {
		inline void sec(uint8_t *seconds) { return ds3231_get_sec(seconds); }

		inline void min(uint8_t *minutes) { return ds3231_get_min(minutes); }

		inline void hour(uint8_t *hours) { return ds3231_get_hour(hours); }

		inline void hour_mode(ds3231_hour_mode_t *hour_mode) {
			return ds3231_get_hour_mode(hour_mode);
		}

		inline void hour_meridiem(ds3231_meridiem_t *meridiem) {
			return ds3231_get_hour_meridiem(meridiem);
		}

		inline void day(uint8_t *day) { return ds3231_get_day(day); }

		inline void date(uint8_t *date) { return ds3231_get_date(date); }

		inline void month(uint8_t *month) { return ds3231_get_month(month); }

		inline void century(ds3231_century_t *century) {
			return ds3231_get_century(century);
		}

		inline void year(uint8_t *year) { return ds3231_get_year(year); }
	} // namespace get

	namespace set {
		inline void sec(uint8_t seconds) { return ds3231_set_sec(seconds); }

		inline void min(uint8_t minutes) { return ds3231_set_min(minutes); }

		inline void hour(uint8_t hours) { return ds3231_set_hour(hours); }

		inline void hour_mode(ds3231_hour_mode_t hour_mode) {
			return ds3231_set_hour_mode(hour_mode);
		}

		inline void day(uint8_t day) { return ds3231_set_day(day); }

		inline void date(uint8_t date) { return ds3231_set_date(date); }

		inline void month(uint8_t month) { return ds3231_set_month(month); }

		inline void year(uint8_t year) { return ds3231_set_year(year); }
	} // namespace set

	namespace status {
		namespace get {
			inline void osf(ds3231_osf_t *osf) {
				return ds3231_status_get_osf(osf);
			}
		} // namespace get

		namespace set {
			inline void osf(ds3231_osf_t osf) {
				return ds3231_status_set_osf(osf);
			}
		} // namespace set
	} // namespace status
} // namespace ds3231

#endif
