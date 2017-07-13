#include "my_tools.h"
#include "cs99xx_config.h"

#if SYNCHRO_ROADS_EN>0

uint32_t char_uint32(uint8_t * data)
{
    union {
        uint8_t ch_data[4];
        uint32_t i_data;
    }u_data;

    u_data.ch_data[0] = data[0];
    u_data.ch_data[1] = data[1];
    u_data.ch_data[2] = data[2];
    u_data.ch_data[3] = data[3];

    return u_data.i_data;
}

void uint32_to_char(uint32_t data, uint8_t *ch_data)
{
    union {
        uint8_t ch_data[4];
        uint32_t  i_data;
    }u_data;    

    u_data.i_data = data;

    ch_data[0] = u_data.ch_data[0];
    ch_data[1] = u_data.ch_data[1];
    ch_data[2] = u_data.ch_data[2];
    ch_data[3] = u_data.ch_data[3];
}

float char_to_float(uint8_t * data)
{
    union {
        uint8_t ch_data[4];
        float f_data;
    }u_data;

    u_data.ch_data[0] = data[0];
    u_data.ch_data[1] = data[1];
    u_data.ch_data[2] = data[2];
    u_data.ch_data[3] = data[3];

    return u_data.f_data;
}

uint16_t char_to_uint16(uint8_t *data)
{
	uint16_t data_16 = 0;
	
	data_16 |= (data[0] << 8);
	data_16 |= data[1];
	
	return data_16;
}

void float_to_char(float f_data, uint8_t *ch_data)
{
    union {
        uint8_t ch_data[4];
        float f_data;
    }u_data;
    
    u_data.f_data = f_data;
	
    ch_data[0] = u_data.ch_data[0];
    ch_data[1] = u_data.ch_data[1];
    ch_data[2] = u_data.ch_data[2];
    ch_data[3] = u_data.ch_data[3];
}

void uint16_to_char(uint16_t uint16_data, uint8_t *ch_data)
{
    union {
        uint8_t ch_data[4];
        uint16_t uint16_data;
    }u_data;
    
    u_data.uint16_data = uint16_data;

    ch_data[0] = u_data.ch_data[0];
    ch_data[1] = u_data.ch_data[1];       
}



#endif
