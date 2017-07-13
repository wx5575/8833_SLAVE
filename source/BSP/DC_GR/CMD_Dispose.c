/*
	直流模块

*/

#include "CMD_Dispose.h"
#include 	"STM32_GPIO_Config.h"
#include 	"serve_test.h"

static const char  * SendCmdList[] = {
    "SET_CALIB_DAT:",
    "SET_TEST_TIME:",
    "SET_TEST_DAC_VALUE:",
    "TEST_START:",
    "TEST_STOP",
    "ASK",
	"SET_VOL_GEAR:",/* 有数据的要加冒号 */
	"SET_TEST_METHOD:",/* 设置测试方法 */

};
/* */
static const char * ReceiveCmdList[] = {
    "T_D",
    "OPEN_ERROR",
    "TEST_OVER",
	"OK",
}; 
 
enum {
    SET_CALIB_DAT,          //校准参数
    SET_TEST_TIME ,
    SET_TEST_DAC_VALUE,
    TEST_START,
    TEST_STOP,
    ASK,
	SET_VOL_GEAR,
	SET_TEST_METHOD,
    END_OF_Send_LIST
};

enum {
    TEST_DATA,
    OPEN_ERROR,
    TEST_OVER,
	OK,

    END_OF_Receive_LIST
};

#define 	SEND_TEST_METHOD_LEN	( 1 )
#define 	SEND_VOL_GEAR_LEN		( 1 )
#define     SEND_CURRENT_LEN        ( 5 )
#define     SEND_CURRENT_POINT      ( 3 )
#define     SEND_TIME_LEN           ( 7 )
#define     SEND_CALIB_LEN          ( 10)
#define     SEND_CALIB_POINT        ( 6 )


/*获得校验值*/
static char GetCheckData(char str[])
{
	char check_data = 0;
	uint8_t i = 0;
	
	while(str[i] != '\0') {
		check_data += str[i];
		i++;
	}
	
	check_data |= 0x80;

    return check_data;
}

static void CmdStrCatCheckData(char str[] ,char check_str[])
{
    check_str[0] =  GetCheckData(str);
    check_str[1] = '\0';

    strcat(str , check_str );
}

/*指令迭代器，寻找上位机指令对应的编号*/
static uint8_t ReceiveCmdIterator(char *CmdStr)
{
    uint8_t Iterator;
    for(Iterator = 0; Iterator < END_OF_Receive_LIST; Iterator ++) {
        if(!(strcmp(CmdStr , ReceiveCmdList[Iterator])))  {
            break;
        }
    }

    return Iterator;
} 

/*将指令和数据从接收到的数据中分离开*/
static void SplitCmdAndCmd_data(char *CmdStr , char * cmd , char * cmd_data)
{
    uint8_t i , j ;

    i = 0;
    j = 0;

    while((CmdStr[i] != '\0') &&(CmdStr[i] != ':') && (CmdStr[i] != '#')) {
        cmd[i] = CmdStr[i] ;
        i++;
    }
    cmd[i] = '\0';
    i++;

    while(CmdStr[i] != '\0')  {
        cmd_data[j] = CmdStr[i] ;

        i++;
        j++;
    }
    cmd_data[j] = '\0';
}  

/*判断校验结果是否正确 */
static ErrorStatus CheckReceiveData(char str[])
{
    ErrorStatus state;
    char check_data ;
    char str_len;    
	const char ok_data[] = "OK#";

	if(!(strcmp(str, ok_data)))
	{
		return SUCCESS;
	}
	
    str_len = strlen(str);
    check_data = str[str_len - 2];
    str[str_len - 2] = '\0';

    if(check_data == GetCheckData(str)) {
        state = SUCCESS;
    }
	else {
        state = ERROR; 
    } 

    return state;
}   

static void ReceiveListDispsoe(uint8_t CmdListNum , char *cmd_data)
{
    switch(CmdListNum)
    {
        case TEST_DATA:
            ReceiveTestDataDispose(cmd_data);
        break;
        
        case OPEN_ERROR:
            ReceiveOpenErrorDispose(); 
        break;
        
        case TEST_OVER:
            ReceiveTestOverDispose(cmd_data);
        break;
		
		case OK:
			
		break;

        
        default:

        break; 
    }
}

/*处理接收到的指令
    对接收到的数据进行校验判断数据是否正确
    将接收到的数据分离出指令和数据
    对指令进行迭代，找出指令对应的编码
    进入指令处理代码
 */
 
int32_t ReceiveCmdDispose(char  CmdStr[])
{
    char cmd[20];           //存放指令
    char cmd_data[100];      //存放操作数
    uint8_t CmdListNum;
    
    if(CheckReceiveData(CmdStr) == ERROR) {
		
		CmdListNum=CmdListNum;
		test_flag.uart_next = 0;
        return -1;
    }
	
	test_flag.uart_next = 0;
	
    SplitCmdAndCmd_data(CmdStr , cmd , cmd_data);
	
    CmdListNum = ReceiveCmdIterator(cmd);
    
    ReceiveListDispsoe(CmdListNum , cmd_data); 
	return 0;
}
/*
    发送设置电压档位
*/
void Send_Vol_Gear(uint32_t vol_gear)
{
    char  send_str[100];
    char  check_data[2];
 //   test_current = 5;
    strcpy(send_str , SendCmdList[SET_VOL_GEAR] );

    CmdStrCatInt(send_str ,vol_gear , SEND_VOL_GEAR_LEN );
    CmdStrCatCheckData(send_str , check_data);
    
    strcat(send_str , "#");

    Uart5_SendStr(send_str);
}
/*
    发送设置测试模式
*/
void Send_Test_Method(uint8_t method)
{
    char  send_str[100];
    char  check_data[2];
 //   test_current = 5;
    strcpy(send_str , SendCmdList[SET_TEST_METHOD]);
	
    CmdStrCatInt(send_str ,method , SEND_TEST_METHOD_LEN);
    CmdStrCatCheckData(send_str , check_data);
    
    strcat(send_str , "#");
	
    Uart5_SendStr(send_str);
}

/*
    发送启动指令
*/
void Send_Test_Start(uint32_t test_current)
{
    char  send_str[100];
    char  check_data[2];
 //   test_current = 5;
    strcpy(send_str , SendCmdList[TEST_START] );

    CmdStrCatInt(send_str ,test_current , SEND_CURRENT_LEN );
    CmdStrCatCheckData(send_str , check_data);
    
    strcat(send_str , "#");

    Uart5_SendStr(send_str);
}

/*
    发送停止指令
*/
void Send_Test_Stop(void)
{
    char  send_str[100];
    char  check_data[2];
    
    strcpy(send_str , SendCmdList[TEST_STOP] );
    CmdStrCatCheckData(send_str , check_data);
    
    strcat(send_str , "#"); 
    
    Uart5_SendStr(send_str);     
}

/*
    发送寻求结果指令
*/
void Send_Ask_Result(void)
{
    char  send_str[100];
    char  check_data[2];
    
    strcpy(send_str , SendCmdList[ASK] );
    CmdStrCatCheckData(send_str , check_data);
    
    strcat(send_str , "#");  
    
    Uart5_SendStr(send_str);     
}

/*
    发送修改测试时间指令
*/
void Send_Test_Time(uint32_t test_time)
{
    char  send_str[100];
    char  check_data[2];

    strcpy(send_str , SendCmdList[SET_TEST_TIME] );

    CmdStrCatInt(send_str ,test_time , SEND_TIME_LEN);
    CmdStrCatCheckData(send_str , check_data);
    
    strcat(send_str , "#");

    Uart5_SendStr(send_str);
}

/*
    发送修改校准信息的指令
*/
void Send_Calib( CALIB_MSG * WriteCalib)
{
    char  send_str[100];
    char  check_data[2];

    strcpy(send_str , SendCmdList[SET_CALIB_DAT] ); 
 
    CmdStrCatDouble(send_str ,WriteCalib->KV , SEND_CALIB_LEN , SEND_CALIB_POINT);
    CmdStrCatDouble(send_str ,WriteCalib->KI , SEND_CALIB_LEN , SEND_CALIB_POINT);
    CmdStrCatDouble(send_str ,WriteCalib->KDac , SEND_CALIB_LEN , SEND_CALIB_POINT);
    CmdStrCatDouble(send_str ,WriteCalib->BV , SEND_CALIB_LEN , SEND_CALIB_POINT);
    CmdStrCatDouble(send_str ,WriteCalib->BI , SEND_CALIB_LEN , SEND_CALIB_POINT);
    CmdStrCatDouble(send_str ,WriteCalib->BDac , SEND_CALIB_LEN , SEND_CALIB_POINT);
 
    CmdStrCatCheckData(send_str , check_data);
    
    strcat(send_str , "#");
    
    Uart5_SendStr(send_str); 
}

/*
    发送校准测试指令
        实际发送的数据位DAC的量化值
*/
void Send_Test_Calib(uint32_t test_current)
{
    char  send_str[100];
    char  check_data[2];
	
    strcpy(send_str , SendCmdList[SET_TEST_DAC_VALUE] );
	
    CmdStrCatInt(send_str ,test_current , SEND_CURRENT_LEN );
    CmdStrCatCheckData(send_str , check_data);
    
    strcat(send_str , "#");
	
    Uart5_SendStr(send_str);
}


