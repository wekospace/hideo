#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "drv_24lcxxx.h"

#define eeprom_init	drv_24lcxxx_init
#define eeprom_read	drv_24lcxxx_read
#define eeprom_write	drv_24lcxxx_write

#define EEPROM_MEMMAP__DISPLAY_LENS		0x00
#define EEPROM_MEMMAP__DISPLAY_TRAPEZOID	0x02

#endif /* !_EEPROM_H_ */