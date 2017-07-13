#ifndef __MY_TOOLS_H
#define __MY_TOOLS_H

#include "stdint.h"

extern void float_to_char(float data, uint8_t *ch_data);

extern void uint32_to_char(uint32_t data, uint8_t *ch_data);

extern float char_to_float(uint8_t * data);
extern uint16_t char_to_uint16(uint8_t *data);
extern uint32_t char_uint32(uint8_t * data);
extern void uint16_to_char(uint16_t data, uint8_t *ch_data);


#endif

