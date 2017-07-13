#include "common_tools.h"

/*
    通用辅助工具
*/
void SplitCmdData(char *cmd_data , char receive[][20], uint8_t size)
{
    uint8_t i,j;
    for(i = 0 ; i < size ; i++)
    {
        j = 0;

        while((*cmd_data != '_') &&(*cmd_data != '\0'))
        {
            receive[i][j] = *cmd_data;
            j++;
            cmd_data++;
        }

        receive[i][j] = '\0';
        cmd_data++;

    }
}

uint32_t StrToInt(char *str)
{
    uint32_t data_i = 0;

    while(*str)
    {
        data_i *= 10;
        data_i = data_i + *str -'0';
        str++;
    }

    return data_i;
}

// static double StrToDouble(char *str , uint8_t PointPlace)
// {
//     uint32_t data_i, pointdata = 1;
//     double data_d;

//     data_i = StrToInt(str);

//     while(PointPlace)
//     {
//         pointdata *= 10;
//         PointPlace --;
//     }

//     data_d = 1.0 * data_i / pointdata;

//     return data_d;
// }

static void IntToStr(char str[] ,uint32_t i_data , uint8_t data_len)
{
    uint8_t i;

    str[data_len] = '\0';
        
    for(i = data_len ; i > 0 ; i--)
    {
        str[i - 1] = i_data % 10 + '0';
        i_data /= 10;
    }   
}

static void DoubleToStr(double d_data , char Str[] , const uint8_t d_len , const uint8_t point_place )
{
	uint8_t i;
	uint32_t i_data = 1;
	
	for(i = 0; i < point_place ; i++) {
		i_data *= 10;
	}
	
	i_data *= d_data;
	
	IntToStr( Str, i_data , d_len);
}

void CmdStrCatDouble(char str[] , double d_data , uint8_t d_len , const uint8_t point_place)
{
    char str_data[20];

    DoubleToStr(d_data , str_data , d_len , point_place );
    strcat(str,str_data);   
}

void CmdStrCatInt(char str[] , uint16_t i_data , uint8_t i_len)
{
    char str_data[20];
	
    IntToStr(str_data , i_data , i_len);
    strcat(str,str_data);
}
