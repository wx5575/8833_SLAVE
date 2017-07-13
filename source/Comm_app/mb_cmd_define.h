#ifndef __MB_CMD_DEFINE_H__
#define __MB_CMD_DEFINE_H__

#define MB_READ_PAR     0X5A    //读参数
#define MB_SET_PAR      0XA5    //写参数
#define MB_EXE_OK       0XF5    //执行命令成功
#define MB_EXE_NG       0XF5    //执行命令失败

typedef enum{
    USB_ACCESS_NO,//不能访问U盘
    USB_ACCESS_OK,//u盘访问OK
    
    USB_CON_NG,//U盘未连接
    USB_CON_OK,//U盘连接OK
}USB_ST;

typedef enum{
    MB_YES_WAIT_ACK,/* MODBUS等待命令响应 */
    MB_NO_WAIT_ACK,/* MODBUS不等待命令响应 */
}MB_WAIT_ACK;

typedef enum{
    NO_ERR,/* 指令执行成功 */
    SYNTAX_ERR,/* 指令中包含不合法的字符或指令串格式错误 */
    EXE_NOT_ALLOWED,/* 在当前状态下不允许执行此指令 */
    PAR_NOT_ALLOWED,/* 接收到指令所不允许的参数 */
    MISS_PARAMETER,/* 指令串中遗漏参数 */
    UNDEFINED_HEADER,/* 未定义的指令表头 */
    PAR_TYPE_ERR,/* 参数类型错误 */
    INVALID_STR_DATA,/* 不允许的字符串参数 */
    EXE_TIME_OUT,/* 指令执行超时 */
    DATA_OUT_RANGE,/* 参数值超出允许范围 */
}MB_COMM_ERR_T;

typedef enum{
    CTRL_LOCAL,/* 本地控制 */
    CTRL_REMOTE,/* 远程控制 */
}CONTROL_ST_T;

typedef enum{
    M_POWER_CHECKRES, /* 电源检测 */
    M_COMM_ADDR, /*与上位机通信地址*/ 
    M_COMM_CTRL_STATUS, /*控制状态*/
    M_CONTROL_TEST_BUTTON, /*设置测试状态*/
    M_DELETE_FILE, /* 删除文件 */
    M_QUERY_ACTIVE_FILE_NUM,/*查询当前文件的编号*/
    M_QUERY_ACTIVE_STEP_NUM,/*查询当前步骤编号*/
    M_CATALOG_FILE, /* 查询文件是否存在存在就返回其文件内容 */
    M_INSERT_STEP, /*插入测试步骤*/
    M_DELETE_STEP, /*删除当前活动步骤*/
    M_ACTIVE_STEP_FRONT, /*当前活动步骤前移一位*/
    M_ACTIVE_STEP_BEHIND, /*当前活动步骤后移一位*/
    M_INTER_CHANGE_STEP, /*交换步骤信息*/
    M_UPDATA_STEP_MODE, /*修改当前活动文件的模式*/
    
    /********** 系统参数 ***************/
    M_SYSTEM_NRULE,/* 测试结果编号规则 */
    M_SYSTEM_TIME,/* 系统时间 */
    M_PASS_WORD,/* 系统密码 */
    M_KEYKLOCK,/* 键盘锁 */
    M_FAIL_CONTINUE,/* 失败继续 */
    M_SYS_LANGUAGE,/* 系统语言 */
    M_TEST_PORT,/* 测试端口 接地 浮地*/
    M_RESET_STOP,/* 复位暂停 */
    M_CHECK_HINT, /* 检验提示 */
    M_SELF_CHECK,/* 自检 */
    M_TEST_SIGNAL,/* 测试信号 */
    M_PHV,/* */
    M_GFI,/* GFI保护 */
    M_RESULT_RECOVER,/* 结果覆盖 */
    M_RESULT_SAVE,/* 结果保存 */
    M_RES_REMAIN_HINT, /* 余量提示 */
    M_SYS_VOLUME,/* 系统音量 */
    M_SYS_CONTRAST,/* 屏幕对比度 */
    M_DUT_NAME,/* 被测品名 */
    
    /********* 测试结果 *****************/
    M_RESULT_FETCH,/* 查询结果信息 */
    M_RESULT_CLEAR,/* 清空结果 */
    M_RESULT_FAIL,/* 查询失败条数 */
    M_RESULT_PASS,/* 查询成功条数 */
    M_RESULT_USED_NUM,/* 查询使用条数 */
    M_RESULT_REMAIN_NUM,/* 查询剩余条数 */
    M_RESULT_TOTAL_NUM,/* 查询总条数 */
    
    /********** 步骤参数设置 ************/
    M_TEST_PORTS,/* 多路扫描端口 */
    M_IR_AUTO_SHIFT,/* IR自动换挡 */
    M_COM_STEPS_CONT,/* 步间连续 */
    M_COM_STEPS_PASS,/* 步间PASS */
    M_COM_DELAY_T,/* 延时时间 */
    M_COM_CHANGE_T,/* 充电时间 */
    M_COM_INTER_T,/* 间隔时间 */
    M_COM_FALL_T,/* 下降时间 */
    M_COM_TEST_T,/* 测试时间 */
    M_COM_RISE_T,/* 上升时间 */
    M_COM_FREQUENCY,/* 输出频率 */
    M_COM_ARC,/* 电弧侦测 */
    M_COM_CURRENT,/* 输出电流 GR */
    M_COM_CHARGE_CURRENT,/* 充电电流 */
    M_COM_REAL_CURRENT,/* 真实电流 */
    M_COM_CUR_LOW,/* 电流下限 */
    M_COM_CUR_HIGH,/* 电流上限 */
    M_COM_CUR_GEAR,/* 电流档位 */
    M_COM_VOLTAGE,/* 输出电压 */
    M_COM_MODE,/* 测试模式 */
    
    M_BBD_OPEN_CHECK,/* BBD开路检测 */
    M_BBD_SHORT_CHECK,/* BBD短路检测 */
    
    /************* 参数编辑 ***************/
    M_SWAP_STEPS,/* 交换测试步 */
    M_MOVE_FORWARD,/* 前移 */
    M_MOVE_BACKWARD,/* 后移 */
    
    /************* 测试命令 ***************/
    M_TEST_MODE,/* 测试模式 */
    M_TEST_FETCH_MSG,/* 测试过程中的数据查询 */
    M_GET_CAP,/* 测试获取BBD电容值 */
    M_GET_OFFSET,/* 测试获取各步的偏移值 */
    M_PLC_STATUS,/* PLC状态 */
    M_TEST_STATUS,/* 测试状态 */
    
    M_DEL_ALL_FILE,/* 删除所有文件 */
    M_STEP_MSG,/* 测试步信息 */
    M_FILE_MSG,/* 文件信息 */
    M_SET_ACTIVE_STEP,/* 读取指定步为当前活动步 */
    M_SET_ACTIVE_FILE,/* 读取指定文件为当前的活动文件 */
    M_SAVE_FILE,/* 当前文件另存为 带参 通过文件结构将文件名与文件编号传过来 */
    M_EDIT_FILE,/* 编辑文件 */
    M_NEW_FILE,/* 新建文件 */
    M_ID_MSG,/* 测试仪标识查询 */
    M_RST,/* 测试仪复位指令 */
    M_TESTER_INFO,/* 仪器信息 */
    M_COMM_BPS,/* 通信波特率 */
    M_COMM_SW_STATUS,/* 通信开关 */
    M_PLC_SW_STATUS,/* PLC开关 */
    M_CYLINDER_CTRL_STATUS,/* 气缸控制状态 */
    
/******************************************************************/
    M_SYS_PAR_USB,    /* 系统参数文件 包含了1个记录 */
    M_SYS_FLAG_USB,   /* 系统标记文件 包含了1个记录 */
    M_FILE_LIST_USB,  /* 文件表目录 包含31个子记录号 分别是 31个文件 */
    
    M_STEP_USED_FLAG_BASE,/* 位图表 */
    M_GROUP_TABLE_BASE,/* 步骤映射表 */
    M_MEM_GROP_BASE_USB,/* 记忆组测试步参数 */
    M_DATA_LOADING_END,/* 数据传输结束 */
    
    M_RESULT_USB,    /* 结果文件号包含n条记录 n为测试仪已存在的记录个数 */
    M_RESULT_INFO,   /* 获取测试仪结果相关信息 */
    M_RESULT_ONE_USB, /* 实时获取结果信息使用 */
    M_SYS_PAR_LEN_USB,/* 查询系统数据长度 */
    M_FILE_STEPS_NUM_USB,/* 查询指定文件的步数 */
    M_GET_TESTER_TYPE_USB,/* 获取测试仪的型号 */
    M_USB_CONNECT_STATUS,/* U盘连接状态 */
    
/******************************************************************/
    M_AUTO_CAL,/* 自动校准 */
	
	M_COM_OFFSET,/* 偏移 */
	
	M_GET_INTERFACE_INFO,/* 获取接口板信息 */
/******************************************************************/
	//校准指令集
	M_CAL_ENTER,/* 进入校准界面 */
	M_CAL_EXIT,/* 退出校准界面 */
	M_CAL_STATUS,/* 获取校准状态 */
	M_CAL_MODE,/* 获取校准模式 */
	M_CAL_RANGE,/* 获取校准量程 */
	M_CAL_START,/* 启动校准输出 */
	M_CAL_STOP,/* 停止校准输出 */
	M_CAL_MEASURE,/* 设置校准测量值 */
	M_CAL_POINTS,/* 获取校准点总个数 */
	M_CAL_LOAD,/* 加载指定的校准点 */
/******************************************************************/
	
}MB_CMD_T;

/* 直流模块指令集 */
enum{
    M_DC_MODULE_CAL_DATA,   /* 校准数据 */
    M_DC_MODULE_DAC,    /* 输出DA */
    M_DC_MODULE_START,  /* 启动测试 */
    M_DC_MODULE_STOP,   /* 停止测试 */
    M_DC_MODULE_TEST_FETCH, /* 获取直流模块的测试信息 电压AD 电流AD 测试状态 ST */
    M_DC_MODULE_VOL_GEAR,   /* 电压档位设定 */
    M_DC_MODULE_TEST_METHOD,/* 设置测试方法 脉冲测试 连续测试 */
    M_DC_MODULE_INFO,   /* 获取直流模块的信息 */
    M_DC_MODULE_SET_DATA,//模块的设置数据包括档位和测试方法...
    M_DC_MODULE_TEST_FETCH_SHORT,//短指令
};
#endif //__MB_CMD_DEFINE_H__

/******************* (C) COPYRIGHT 2016 长盛仪器 *****END OF FILE*******************/

