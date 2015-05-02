#ifndef ADVANCED_DELAY_H
#define ADVANCED_DELAY_H

#include "export.h"

#include <stdint.h>

extern MY_EXPORT void adv_delay_setup();
extern MY_EXPORT void adv_delaym(uint32_t interval);
extern MY_EXPORT void adv_delayu(uint32_t interval);
extern MY_EXPORT void adv_delayn(uint32_t interval);

#endif // ADVANCED_DELAY_H
