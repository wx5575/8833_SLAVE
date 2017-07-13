/*
 * Copyright(c) 2014,南京长盛仪器
 * All rights reserved
 * 文件名称：cs99xx_set_par_path.c
 * 摘  要  ：服务于整个参数设置 指定设置参数的路径，即先后顺序
 * 当前版本：V1.0，编写者：王鑫
 * 历史版本：
 * 修改记录：
 *
 */


#include	<string.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"


static void init_more_options_list(MORE_OPTIONS_LIST *l)
{
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
}

static void tail_add_more_opt(MORE_OPTIONS_LIST *l, MORE_OPTIONS_STR *p)
{
    if(p == NULL)
    {
        return;
    }
    
    if(l->tail == NULL)
    {
        l->head = p;
    }
    else
    {
        l->tail->next = p;
        p->prev = l->tail;
    }
    
    l->tail = p;
    /* 组成环形链表 */
    l->tail->next = l->head;
    l->head->prev = l->tail;
    l->size++;
}

static void more_option_list_init(uint8_t x, uint8_t y, uint8_t xo, uint8_t one_line_num, MORE_OPTIONS_LIST *l)
{
    MORE_OPTIONS_STR *find = NULL;
    uint8_t i = 0;
    uint8_t line = 0;
    uint8_t temp = 0;
    uint8_t total_line = 0;/* 总行数 */
    uint8_t last_line = 0;/* 最后一行的个数 */
    
    temp = l->size % one_line_num;
    total_line = l->size / one_line_num + (temp == 0? 0 : 1);
    last_line  = (temp == 0? one_line_num : temp);
    
    for(find = l->head, i = 0; i < l->size; i++, find = find->next)
    {
        find->y = y;
        find->x = x + xo * (i % one_line_num);
        
        find->cur = (i % one_line_num) + 1;
        
        if(find->cur == 1)
        {
            line++;
        }
        
        if(line == total_line)
        {
            find->total = last_line;
        }
        else
        {
            find->total = one_line_num;
        }
    }
}

static void cc_cur_gear_options_way(void)
{
	/* 单步->总步->保持 */
    init_more_options_list(&cc_cur_gear_list);
    
    tail_add_more_opt(&cc_cur_gear_list, &cc_cur_gear_option_pool[CC_CUR_HIGH]);
    tail_add_more_opt(&cc_cur_gear_list, &cc_cur_gear_option_pool[CC_CUR_MID]);
    tail_add_more_opt(&cc_cur_gear_list, &cc_cur_gear_option_pool[CC_CUR_LOW]);
}

static void ir_filtering_deep_options_way(void)
{
	/* 快->中->慢 */
    init_more_options_list(&ir_filtering_deep_list);
    
    tail_add_more_opt(&ir_filtering_deep_list, &ir_filtering_deep_option_pool[IR_FILTERING_DEEP_FAST]);
    tail_add_more_opt(&ir_filtering_deep_list, &ir_filtering_deep_option_pool[IR_FILTERING_DEEP_MID]);
    tail_add_more_opt(&ir_filtering_deep_list, &ir_filtering_deep_option_pool[IR_FILTERING_DEEP_SLOW]);
}
void init_ir_filtering_deep_options_list(void *p_info)
{
    #define POX   40
    DATA_INFO *p = p_info;
    uint8_t one_line_num = 3;/* 每行的个数 */
    MORE_OPTIONS_LIST *l = &ir_filtering_deep_list;
    
    ir_filtering_deep_options_way();/* 添加选项 */
    
    p->user_data = l;/* 将用户的选项地址保存到用户数据中 */
    more_option_list_init(SX, p->y, POX, one_line_num, l);
	
    p->unit[0] = ir_filtering_deep_option_pool[read_par_to_buf(p, NULL)].name[0];
    p->unit[1] = ir_filtering_deep_option_pool[read_par_to_buf(p, NULL)].name[1];
}

void init_cc_cur_gear_options_list(void *p_info)
{
    #define POX   40
    DATA_INFO *p = p_info;
    uint8_t one_line_num = 3;/* 每行的个数 */
    MORE_OPTIONS_LIST *l = &cc_cur_gear_list;
    
    cc_cur_gear_options_way();/* 添加选项 */
    
    p->user_data = l;/* 将用户的选项地址保存到用户数据中 */
    more_option_list_init(PX, p->y, POX, one_line_num, l);
}

static void plc_signal_options_way(void)
{
	/* 单步->总步->保持 */
    init_more_options_list(&plc_signal_list);
    
    tail_add_more_opt(&plc_signal_list, &plc_signal_option_pool[EACH_STEP]);
    tail_add_more_opt(&plc_signal_list, &plc_signal_option_pool[ALL_STEP]);
    
    if(0 == strcmp(g_cur_type->name, "VTC9929")
        || 0 == strcmp(g_cur_type->name, "CS9929EX-CC"))
    {
        tail_add_more_opt(&plc_signal_list, &plc_signal_option_pool[TEST_HOLD]);
    }
}
void init_plc_signal_options_list(void *p_info)
{
    DATA_INFO *p = p_info;
    uint8_t one_line_num = 2;/* 每行的个数 */
    MORE_OPTIONS_LIST *l = &plc_signal_list;
    
    plc_signal_options_way();/* 添加选项 */
    
    p->user_data = l;
    
	more_option_list_init(SX, p->y, SOX, one_line_num, l);
    p->unit[0] = plc_signal_option_pool[read_par_to_buf(p, NULL)].name[0];
    p->unit[1] = plc_signal_option_pool[read_par_to_buf(p, NULL)].name[1];
    
}

void custom_options_way(MORE_OPTIONS_LIST* list, MORE_OPTIONS_STR* opt, uint8_t n)
{
    int32_t i = 0;
    
	/* 单步->总步->保持 */
    init_more_options_list(list);
    
    for(i = 0; i < n; i++)
    {
        tail_add_more_opt(list, &opt[i]);
    }
}


void init_custom_options_list(uint16_t who)
{
    MORE_OPTIONS_STR *find = NULL;
    uint8_t i = 0;
    uint8_t total_line = 0;/* 总行数 */
    uint8_t last_line = 0;/* 最后一行的个数 */
    uint8_t one_line_num = 2;/* 每行的个数 */
    uint8_t temp = 0;
    uint8_t line = 0;
    uint8_t size = 0;
    MORE_OPTIONS_LIST* p_opt_list;
    MORE_OPTIONS_STR * p_opt_pool;
    
    switch(who)
    {
        case GUI_CUSTOM_AMP_SELECT:
            p_opt_list = &amp_type_list;
            p_opt_pool = amp_type_option_pool;
            size = 2;
            break;
        case GUI_CUSTOM_PAR_MEDIUM:
            p_opt_list = &par_medium_list;
            p_opt_pool = par_medium_option_pool;
            size = 2;
            break;
        case GUI_CUSTOM_RES_MEDIUM:
            p_opt_list = &res_medium_list;
            p_opt_pool = res_medium_option_pool;
            size = 2;
            break;
        case GUI_CUSTOM_CAL_MEDIUM:
            p_opt_list = &cal_medium_list;
            p_opt_pool = cal_medium_option_pool;
            size = 2;
            break;
    }
    
    custom_options_way(p_opt_list, p_opt_pool, size);
    
    temp = p_opt_list->size % one_line_num;
    total_line = p_opt_list->size / one_line_num + (temp == 0? 0 : 1);
    last_line  = (temp == 0? one_line_num : temp);
    
    menu_par[who].user_data = p_opt_list;
    
    for(find = p_opt_list->head, i = 0; i < p_opt_list->size; i++, find = find->next)
    {
        find->y = menu_par[who].y;
        find->x = SX + SOX * (i % one_line_num);
        
        find->cur = (i % one_line_num) + 1;
        
        if(find->cur == 1)
        {
            line++;
        }
        
        if(line == total_line)
        {
            find->total = last_line;
        }
        else
        {
            find->total = one_line_num;
        }
    }
}


static void init_par_list(void)
{
	par_info_list.head = NULL;
	par_info_list.tail = NULL;
	par_info_list.size = 0;
}
static void tail_insert_option(uint32_t menu_handle, void *data)
{
    DATA_INFO *p;
	
    if(menu_handle >= GUI_MENU_END)
    {
        return;
    }
	
	p = &menu_par[menu_handle];
	p->pData = data;
	
    if(par_info_list.tail == NULL)
    {
        par_info_list.head = p;
    }
    else
    {
        par_info_list.tail->next = p;
        p->prev = par_info_list.tail;
    }
    
    par_info_list.tail = p;
    
    par_info_list.tail->next = par_info_list.head;
    
    par_info_list.size++;
}

static void gr_test_way_vol_drop1(UN_STRUCT *pun)
{
	/* 测试步骤->测试方法->输出电流->电压上限->电压下限->测试时间->间隔时间->步间PASS->步间连续-> */
    init_par_list();
    tail_insert_option(GUI_COM_STEP			, &pun->gr.step);
    tail_insert_option(GUI_GR_TEST_METHOD	, &pun->gr.test_method);
    tail_insert_option(GUI_GR_OUTPUT_CUR	, &pun->gr.testing_cur);
    tail_insert_option(GUI_GR_VOL_H			, &pun->gr.upper_limit);
    tail_insert_option(GUI_GR_VOL_L			, &pun->gr.lower_limit);
    tail_insert_option(GUI_COM_TEST_T		, &pun->gr.testing_time);
    tail_insert_option(GUI_COM_INTER_T		, &pun->gr.interval_time);
    tail_insert_option(GUI_COM_STEPS_PASS	, &pun->gr.steps_pass);
    tail_insert_option(GUI_COM_STEPS_CONT	, &pun->gr.steps_cont);
}
void gr_test_way_vol_drop2(UN_STRUCT *pun)
{
	/* 测试步骤->电压档位->输出电流->电压上限->电压下限->测试时间->间隔时间->步间PASS->步间连续 */
    init_par_list();
    tail_insert_option(GUI_COM_STEP			, &pun->gr.step);
    tail_insert_option(GUI_GR_VOL_GEAR		, &pun->gr.voltage_gear);
    tail_insert_option(GUI_GR_OUTPUT_CUR	, &pun->gr.testing_cur);
    tail_insert_option(GUI_GR_VOL_H			, &pun->gr.upper_limit);
    tail_insert_option(GUI_GR_VOL_L			, &pun->gr.lower_limit);
    tail_insert_option(GUI_COM_TEST_T		, &pun->gr.testing_time);
    tail_insert_option(GUI_COM_INTER_T		, &pun->gr.interval_time);
    tail_insert_option(GUI_COM_STEPS_PASS	, &pun->gr.steps_pass);
    tail_insert_option(GUI_COM_STEPS_CONT	, &pun->gr.steps_cont);
}

static void gr_test_way_1(UN_STRUCT *pun)
{
	/* 测试步骤->测试模式->输出电流->电阻上限->电阻下限->测试时间->间隔时间->步间PASS->步间连续 */
    init_par_list();
    tail_insert_option(GUI_COM_STEP			, &pun->gr.step);
    tail_insert_option(GUI_COM_MODE			, &pun->gr.mode);
    tail_insert_option(GUI_GR_OUTPUT_CUR	, &pun->gr.testing_cur);
    tail_insert_option(GUI_GR_RES_H			, &pun->gr.upper_limit);
    tail_insert_option(GUI_GR_RES_L			, &pun->gr.lower_limit);
	
	if(g_custom_sys_par.offset_set_en)
	{
		tail_insert_option(GUI_COM_RES_OFFSET	, &pun->gr.offset_res);/* 电阻偏移 */
	}
	
    tail_insert_option(GUI_COM_OUTPUT_FREQ	, &pun->gr.output_freq);
    tail_insert_option(GUI_COM_TEST_T		, &pun->gr.testing_time);
    tail_insert_option(GUI_COM_INTER_T		, &pun->gr.interval_time);
    tail_insert_option(GUI_COM_STEPS_PASS	, &pun->gr.steps_pass);
    tail_insert_option(GUI_COM_STEPS_CONT	, &pun->gr.steps_cont);
}
static void dc_gr_test_way_1(UN_STRUCT *pun)
{
	/* 测试步骤->测试模式->输出电流->电阻上限->电阻下限->测试时间->间隔时间->步间PASS->步间连续 */
    init_par_list();
    tail_insert_option(GUI_COM_STEP			, &pun->gr.step);
    tail_insert_option(GUI_COM_MODE			, &pun->gr.mode);
    tail_insert_option(GUI_GR_OUTPUT_CUR	, &pun->gr.testing_cur);
    tail_insert_option(GUI_GR_RES_H			, &pun->gr.upper_limit);
    tail_insert_option(GUI_GR_RES_L			, &pun->gr.lower_limit);
	
	if(g_custom_sys_par.offset_set_en)
	{
		tail_insert_option(GUI_COM_RES_OFFSET	, &pun->gr.offset_res);/* 电阻偏移 */
	}
	
    tail_insert_option(GUI_COM_TEST_T		, &pun->gr.testing_time);
    tail_insert_option(GUI_COM_INTER_T		, &pun->gr.interval_time);
    tail_insert_option(GUI_COM_STEPS_PASS	, &pun->gr.steps_pass);
    tail_insert_option(GUI_COM_STEPS_CONT	, &pun->gr.steps_cont);
}

static void init_gr_par_info(void)
{
	menu_par[GUI_COM_MODE].unit[0] = mode_pool[GR_VOL_DROP_EN][GR];
	menu_par[GUI_COM_MODE].unit[1] = mode_pool[GR_VOL_DROP_EN][GR];
	menu_par[GUI_GR_OUTPUT_CUR].upper = GR_CUR_H;
	menu_par[GUI_GR_OUTPUT_CUR].lower = GR_CUR_L;
    
	if(type_spe.gr_hz_type == HZ_TYPE_GRADE)
	{
		menu_par[GUI_COM_OUTPUT_FREQ].fun = set_frequency;
		menu_par[GUI_COM_OUTPUT_FREQ].type = INT_T;
		menu_par[GUI_COM_OUTPUT_FREQ].lon = 1;
		menu_par[GUI_COM_OUTPUT_FREQ].decs = 0;
		menu_par[GUI_COM_OUTPUT_FREQ].unit[0] = "";
		menu_par[GUI_COM_OUTPUT_FREQ].upper = 9;
		menu_par[GUI_COM_OUTPUT_FREQ].lower = 1;
	}
}

uint8_t get_gr_cur_no_dis_bits(void)
{
    return menu_par[GUI_GR_OUTPUT_CUR].no_dis_bits;
}

int32_t count_gr_cur_no_dis_bits(uint32_t val)
{
    int32_t res = 0;
    
    if(val >= 10000)
    {
        if(menu_par[GUI_GR_OUTPUT_CUR].no_dis_bits != 1)
        {
            menu_par[GUI_GR_OUTPUT_CUR].no_dis_bits = 1;
            res = 1;
        }
    }
    else
    {
        if(menu_par[GUI_GR_OUTPUT_CUR].no_dis_bits != 0)
        {
            menu_par[GUI_GR_OUTPUT_CUR].no_dis_bits = 0;
            res = 1;
        }
    }
    
    return res;
}
static void init_gr_par_list(NODE_STEP *pnode)
{
	int32_t i = 0;
	int32_t j = 0;
	int32_t k = 0;
	int32_t page = 0;
	int32_t last_page_num = 0;
	DATA_INFO *p = NULL;
	UN_STRUCT *pun = NULL;
	
	if(pnode == NULL)
	{
		return;
	}
    
	pun = &pnode->one_step;
	
	if(GR_VOL_DROP_EN == ENABLE)
	{
		gr_test_way_vol_drop1(pun);
	}
	else if(GR_VOL_DIV_SEGMENT_EN == ENABLE)
	{
		gr_test_way_vol_drop2(pun);
	}
    else if(DC_GR_EN == ENABLE)
    {
        dc_gr_test_way_1(pun);
    }
    else
    {
        gr_test_way_1(pun);
    }
	
	for(p=par_info_list.head,i=0, j=1; i==0 || p!=par_info_list.head; p=p->next)
	{
		if(++i > 3)
		{
			i = 1;
			j++;
		}
		p->page = j;
		p->cur = i;
		p->y = GY + (i-1)*OY;
		k++;
	}
	
	page = k/3 + (k%3==0?0:1);
	last_page_num = k%3==0?3:k%3;
    
	for(p=par_info_list.head,i=0, j=1; i==0 || p!=par_info_list.head; p=p->next,i++)
	{
		if(p->page < page)
		{
			p->total = 3;
		}
		else
		{
			p->total = last_page_num;
		}
	}
    
	if(pun == NULL)
	{
		return;
	}
	
	updata_gr_vol_gear_affect_par_info(pun);
    
    if(GR_CUR_H >= 10000)
    {
        menu_par[GUI_GR_OUTPUT_CUR].lon = 6;
        menu_par[GUI_GR_OUTPUT_CUR].decs = 2;
        
        count_gr_cur_no_dis_bits(pun->gr.testing_cur);
    }
    
    init_gr_par_info();
}

/* 多路同步模式使用 */
static void acw_set_par_way_1(UN_STRUCT *pun)
{
	/* 测试步骤->测试模式->输出电压->电流档位->电流上限->电流下限->支路电流上限->电流下限->真实电流->电弧侦测->输出频率->
	上升时间->测试时间->下降时间->间隔时间->步间PASS->步间连续->端口设置 */
    init_par_list();
    tail_insert_option(GUI_COM_STEP			, &pun->acw.step);
    tail_insert_option(GUI_COM_MODE			, &pun->acw.mode);
    tail_insert_option(GUI_ACW_OUTPUT_VOL	, &pun->acw.testing_voltage);
    tail_insert_option(GUI_ACW_CUR_GEAR		, &pun->acw.gear_i);
    tail_insert_option(GUI_ACW_CUR_H		, &pun->acw.upper_limit);
    tail_insert_option(GUI_ACW_CUR_L		, &pun->acw.lower_limit);
	
	/* 电流偏移 */
	if(g_custom_sys_par.offset_set_en)
	{
		tail_insert_option(GUI_COM_CUR_OFFSET	, &pun->acw.offset_cur);
	}
	
    tail_insert_option(GUI_ACW_REAL_CUR		, &pun->acw.ac_real_cur);
    tail_insert_option(GUI_ACW_ARC_SUR		, &pun->acw.arc_sur);
    tail_insert_option(GUI_COM_OUTPUT_FREQ	, &pun->acw.output_freq);
    tail_insert_option(GUI_COM_RISE_T		, &pun->acw.rise_time);
    tail_insert_option(GUI_COM_TEST_T		, &pun->acw.testing_time);
    tail_insert_option(GUI_COM_FALL_T		, &pun->acw.fall_time);
    tail_insert_option(GUI_COM_INTER_T		, &pun->acw.interval_time);
    tail_insert_option(GUI_COM_STEPS_PASS	, &pun->acw.steps_pass);
    tail_insert_option(GUI_COM_STEPS_CONT	, &pun->acw.steps_cont);
    if(type_spe.port_num != PORT_NUM0)
        tail_insert_option(GUI_COM_PORT		, &pun->acw.port);
}

static void init_acw_par_info(void)
{
	if(type_spe.hz_type == HZ_TYPE_CONT)/* 连续模式 */
	{
		menu_par[GUI_COM_OUTPUT_FREQ].fun = set_float_par;
		menu_par[GUI_COM_OUTPUT_FREQ].type = FLOAT_T;
		menu_par[GUI_COM_OUTPUT_FREQ].lon = 5;
		menu_par[GUI_COM_OUTPUT_FREQ].decs = 1;
		menu_par[GUI_COM_OUTPUT_FREQ].unit[0] = (const unsigned char*)"Hz";
		menu_par[GUI_COM_OUTPUT_FREQ].upper = 4000;
		menu_par[GUI_COM_OUTPUT_FREQ].lower = 400;
		
		menu_par[GUI_COM_OUTPUT_FREQ].upper_str = (const unsigned char*)"400.0Hz";
		menu_par[GUI_COM_OUTPUT_FREQ].lower_str = (const unsigned char*)"040.0Hz";
	}
    
	switch(g_cur_file->arc_mode)
	{
		case ARC_CUR_MODEL:/* 电流模式 */
			menu_par[GUI_ACW_ARC_SUR].unit[0] = unit_pool[CUR_U_mA];
			menu_par[GUI_ACW_ARC_SUR].type = FLOAT_T;	/* 将数据类型改为浮点数 */
			menu_par[GUI_ACW_ARC_SUR].lon = 5;
			menu_par[GUI_ACW_ARC_SUR].upper = ACW_ARC_H;
			menu_par[GUI_ACW_ARC_SUR].lower = 0;
			menu_par[GUI_ACW_ARC_SUR].upper_str = (const unsigned char*)"20.00mA";
			menu_par[GUI_ACW_ARC_SUR].lower_str = (const unsigned char*)"00.00mA";
			break;
		case ARC_GRADE_MODEL:/* 等级模式 */
			menu_par[GUI_ACW_ARC_SUR].unit[0] = unit_pool[NULL_U_NULL];
			menu_par[GUI_ACW_ARC_SUR].type = INT_T;	/* 将数据类型改为整形数 */
			menu_par[GUI_ACW_ARC_SUR].lon = 1;
			menu_par[GUI_ACW_ARC_SUR].upper = 9;
			menu_par[GUI_ACW_ARC_SUR].lower = 0;
			menu_par[GUI_ACW_ARC_SUR].upper_str = (const unsigned char*)"   9   ";
			menu_par[GUI_ACW_ARC_SUR].lower_str = (const unsigned char*)"   0   ";
			break;
	}
}
static void init_acw_par_list(NODE_STEP *pnode)
{
	int32_t i = 0;
	int32_t j = 0;
	int32_t k = 0;
	int32_t page = 0;
	int32_t last_page_num = 0;
	int32_t flag = 0;
	DATA_INFO *p = NULL;
	UN_STRUCT *pun = NULL;
	
	if(pnode == NULL)
	{
		return;
	}
	pun = &pnode->one_step;
	
    acw_set_par_way_1(pun);
	
	for(p=par_info_list.head,i=0, j=1; i==0 || p!=par_info_list.head; p=p->next)
	{
		if(++i > 3)
		{
			i = 1;
			j++;
		}
        
		p->page = j;
		p->cur = i;
		p->y = GY + (i-1)*OY;
		k++;
        
		if(p->fun == set_all_ports)
		{
			if(p->cur != 1)
			{
				p->cur = 1;
				p->page++;
				p->y = GY;
				flag = 1;
			}
		}
	}
	
	if(flag)
	{
		k--;
		page = k/3 + (k%3==0?0:1);/* page 不包含端口设置那一页 */
		last_page_num = k%3==0?3:k%3;
	}
	else
	{
		page = k/3 + (k%3==0?0:1);
		last_page_num = k%3==0?3:k%3;
	}
	
	for(p=par_info_list.head,i=0, j=1; i==0 || p!=par_info_list.head; p=p->next,i++)
	{
		if(p->page < page)
		{
			p->total = 3;
		}
		else
		{
			p->total = last_page_num;
		}
	}
	if(pun == NULL)
	{
		return;
	}
		
	menu_par[GUI_COM_MODE].unit[0] = mode_pool[GR_VOL_DROP_EN][ACW];
	menu_par[GUI_COM_MODE].unit[1] = mode_pool[GR_VOL_DROP_EN][ACW];
	menu_par[GUI_ACW_OUTPUT_VOL].upper = ACW_VOL_H;
	menu_par[GUI_ACW_OUTPUT_VOL].lower = ACW_VOL_L;
	
    updata_ac_cur_gear_affect_par_info(ACW, pun->acw.gear_i);
}

static void cc_set_par_way_1(UN_STRUCT *pun)
{
	/* 测试步骤->测试模式->输出电压->电流档位->电流上限->电流下限->真实电流->电弧侦测->输出频率->上升时间->测试时间->下降时间->
	间隔时间->步间PASS->步间连续->端口设置 */
    init_par_list();
    tail_insert_option(GUI_COM_STEP			, &pun->cc.step);
    tail_insert_option(GUI_COM_MODE			, &pun->cc.mode);
    tail_insert_option(GUI_CC_CUR_INTENSITY	, &pun->cc.cur_intensity);
    tail_insert_option(GUI_CC_CUR_L			, &pun->cc.lower_limit);
    tail_insert_option(GUI_COM_STEPS_PASS	, &pun->cc.steps_pass);
    tail_insert_option(GUI_COM_STEPS_CONT	, &pun->cc.steps_cont);
    if(type_spe.port_num != PORT_NUM0)
        tail_insert_option(GUI_COM_PORT		, &pun->cc.port);
}

static void init_cc_par_list(NODE_STEP *pnode)
{
	int32_t i = 0;
	int32_t j = 0;
	int32_t k = 0;
	int32_t page = 0;
	int32_t last_page_num = 0;
	int32_t flag = 0;
	DATA_INFO *p = NULL;
	UN_STRUCT *pun = NULL;
	
	if(pnode == NULL)
	{
		return;
	}
	pun = &pnode->one_step;
	
    cc_set_par_way_1(pun);
	
	for(p=par_info_list.head,i=0, j=1; i==0 || p!=par_info_list.head; p=p->next)
	{
		if(++i > 3)
		{
			i = 1;
			j++;
		}
        
		p->page = j;
		p->cur = i;
		p->y = GY + (i-1)*OY;
		k++;
        
		if(p->fun == set_all_ports)
		{
			if(p->cur != 1)
			{
				p->cur = 1;
				p->page++;
                p->y = GY;
				flag = 1;
			}
		}
	}
	
	if(flag)
	{
		k--;/* page 不包含端口设置那一页 */
	}
    
    page = k/3 + (k%3==0?0:1);
    last_page_num = (k%3==0? 3: k%3);
	
	for(p=par_info_list.head,i=0, j=1; i==0 || p!=par_info_list.head; p=p->next,i++)
	{
		if(p->page < page)
		{
			p->total = 3;
		}
        else if(p->fun == set_all_ports)
        {
            p->total = 1;
        }
		else
		{
			p->total = last_page_num;
		}
	}
    
	if(pun == NULL)
	{
		return;
	}
    
	menu_par[GUI_COM_MODE].unit[0]     = mode_pool[GR_VOL_DROP_EN][CC];
	menu_par[GUI_COM_MODE].unit[1]     = mode_pool[GR_VOL_DROP_EN][CC];
	menu_par[GUI_CC_OUTPUT_VOL].upper   = CC_VOL_H;
	menu_par[GUI_CC_OUTPUT_VOL].lower   = CC_VOL_L;
    
	init_cc_cur_gear_options_list(&menu_par[GUI_CC_CUR_INTENSITY]);
    updata_ac_cur_gear_affect_par_info(CC, pun->cc.gear_i);
    menu_par[GUI_CC_CUR_INTENSITY].unit[0] = cc_cur_gear_option_pool[pun->cc.cur_intensity].name[0];
    menu_par[GUI_CC_CUR_INTENSITY].unit[1] = cc_cur_gear_option_pool[pun->cc.cur_intensity].name[1];
}
static void dcw_set_par_way_1(UN_STRUCT *pun)
{
	/* 测试步骤->测试模式->输出电压->电流档位->电流上限->电流下限->充电电流->电弧侦测->延时时间->上升时间->测试时间->下降时间->
	间隔时间->步间PASS->步间连续->测试端口 */
    
    init_par_list();
    tail_insert_option(GUI_COM_STEP			, &pun->dcw.step);
    tail_insert_option(GUI_COM_MODE			, &pun->dcw.mode);
    tail_insert_option(GUI_DCW_OUTPUT_VOL	, &pun->dcw.testing_voltage);
    tail_insert_option(GUI_DCW_CUR_GEAR		, &pun->dcw.gear_i);
    tail_insert_option(GUI_DCW_CUR_H		, &pun->dcw.upper_limit);
    tail_insert_option(GUI_DCW_CUR_L		, &pun->dcw.lower_limit);
	
	/* 电流偏移 */
	if(g_custom_sys_par.offset_set_en)
	{
		tail_insert_option(GUI_COM_CUR_OFFSET, &pun->dcw.offset_cur);
	}
	
    tail_insert_option(GUI_DCW_CHARGE_CUR	, &pun->dcw.charge_cur);
    tail_insert_option(GUI_DCW_ARC_SUR		, &pun->dcw.arc_sur);
    tail_insert_option(GUI_COM_DELAY_T		, &pun->dcw.delay_time);
    tail_insert_option(GUI_COM_RISE_T		, &pun->dcw.rise_time);
    tail_insert_option(GUI_COM_TEST_T		, &pun->dcw.testing_time);
    tail_insert_option(GUI_COM_FALL_T		, &pun->dcw.fall_time);
    tail_insert_option(GUI_COM_INTER_T		, &pun->dcw.interval_time);
    tail_insert_option(GUI_COM_STEPS_PASS	, &pun->dcw.steps_pass);
    tail_insert_option(GUI_COM_STEPS_CONT	, &pun->dcw.steps_cont);
    if(type_spe.port_num != PORT_NUM0)	
        tail_insert_option(GUI_COM_PORT		, &pun->dcw.port);
}
static void dcw_set_par_way_2(UN_STRUCT *pun)
{
	/* 测试步骤->测试模式->输出电压->电流档位->电流上限->电流下限->充电电流->电弧侦测->延时时间->上升时间->测试时间->下降时间->
	间隔时间->步间PASS->步间连续->测试端口 */
    
    init_par_list();
    tail_insert_option(GUI_COM_STEP				, &pun->dcw.step);
    tail_insert_option(GUI_COM_MODE				, &pun->dcw.mode);
    tail_insert_option(GUI_DCW_OUTPUT_VOL		, &pun->dcw.testing_voltage);
    tail_insert_option(GUI_DCW_CUR_GEAR			, &pun->dcw.gear_i);
    tail_insert_option(GUI_DCW_CUR_H			, &pun->dcw.upper_limit);
    tail_insert_option(GUI_DCW_CUR_L			, &pun->dcw.lower_limit);
    tail_insert_option(GUI_DCW_CHARGE_CUR		, &pun->dcw.charge_cur);
			
    tail_insert_option(GUI_DCW_ARC_SUR			, &pun->dcw.arc_sur);
    tail_insert_option(GUI_DCW_OUTPUT_IMPEDANCE	, &pun->dcw.output_impedance);
    
    tail_insert_option(GUI_COM_RISE_T			, &pun->dcw.rise_time);
    tail_insert_option(GUI_COM_STAB_T			, &pun->dcw.stab_time);
    tail_insert_option(GUI_COM_TEST_T			, &pun->dcw.testing_time);
    tail_insert_option(GUI_COM_FALL_T			, &pun->dcw.fall_time);
    tail_insert_option(GUI_COM_DELAY_T			, &pun->dcw.delay_time);
    tail_insert_option(GUI_COM_INTER_T			, &pun->dcw.interval_time);
    tail_insert_option(GUI_COM_STEPS_PASS		, &pun->dcw.steps_pass);
    tail_insert_option(GUI_COM_STEPS_CONT		, &pun->dcw.steps_cont);
    if(type_spe.port_num != PORT_NUM0)		
        tail_insert_option(GUI_COM_PORT			, &pun->dcw.port);
}
static void init_dcw_par_info(void)
{
	switch(g_cur_file->arc_mode)
	{
		case ARC_CUR_MODEL:/* 电流模式 */
			menu_par[GUI_DCW_ARC_SUR].unit[0] = (const unsigned char*)"mA";
			menu_par[GUI_DCW_ARC_SUR].type = 1;	/* 将数据类型改为浮点数 */
			menu_par[GUI_DCW_ARC_SUR].lon = 5;
			menu_par[GUI_DCW_ARC_SUR].upper = DCW_ARC_H;
			menu_par[GUI_DCW_ARC_SUR].lower = 0;
			menu_par[GUI_DCW_ARC_SUR].upper_str = (const unsigned char*)"20.00mA";
			menu_par[GUI_DCW_ARC_SUR].lower_str = (const unsigned char*)"00.00mA";
			break;
		case ARC_GRADE_MODEL:/* 等级模式 */
			menu_par[GUI_DCW_ARC_SUR].unit[0] = (const unsigned char*)"";
			menu_par[GUI_DCW_ARC_SUR].type = 0;	/* 将数据类型改为整形数 */
			menu_par[GUI_DCW_ARC_SUR].lon = 1;
			menu_par[GUI_DCW_ARC_SUR].upper = 9;
			menu_par[GUI_DCW_ARC_SUR].lower = 0;
			menu_par[GUI_DCW_ARC_SUR].upper_str = (const unsigned char*)"   9   ";
			menu_par[GUI_DCW_ARC_SUR].lower_str = (const unsigned char*)"   0   ";
			break;
	}
    
    /* 输出阻抗 */
    if(type_spe.dcw_big_cap_en == ENABLE)
    {
        uint8_t size = output_impedan_opt[type_spe.dcw_output_impedan_opt_index].size;
        
        menu_par[GUI_DCW_OUTPUT_IMPEDANCE].upper = size - 1;        /* 参数上限值 */
    }
}

static void init_dcw_par_list(NODE_STEP *pnode)
{
	int32_t i = 0;
	int32_t j = 0;
	int32_t k = 0;
	int32_t page = 0;
	int32_t last_page_num = 0;
	DATA_INFO *p = NULL;
	int32_t flag = 0;
	UN_STRUCT *pun = NULL;
	
	if(pnode == NULL)
	{
		return;
	}
	pun = &pnode->one_step;
	
    if(type_spe.dcw_big_cap_en == ENABLE)
    {
        dcw_set_par_way_2(pun);
    }
    else
    {
        dcw_set_par_way_1(pun);
    }
	
	for(p=par_info_list.head,i=0, j=1; i==0 || p!=par_info_list.head; p=p->next)
	{
		if(++i > 3)
		{
			i = 1;
			j++;
		}
		
		p->page = j;
		p->cur = i;
		p->y = GY + (i-1)*OY;
		k++;
		if(p->fun == set_all_ports)
		{
			if(p->cur != 1)
			{
				p->cur = 1;
				p->page++;
				p->y = GY;
				flag = 1;
			}
		}
	}
	
	if(flag)
	{
		k--;
		page = k/3 + (k%3==0?0:1);/* page 不包含端口设置那一页 */
		last_page_num = k%3==0?3:k%3;
	}
	else
	{
		page = k/3 + (k%3==0?0:1);
		last_page_num = k%3==0?3:k%3;
	}
	
	for(p=par_info_list.head,i=0, j=1; i==0 || p!=par_info_list.head; p=p->next,i++)
	{
		
		if(p->fun == set_all_ports)
		{
			p->total = 1;
		}
		else
		{
			if(p->page < page)
			{
				p->total = 3;
			}
			else
			{
				p->total = last_page_num;
			}
		}
	}
	if(pun == NULL)
	{
		return;
	}
	
	menu_par[GUI_COM_MODE].unit[0] = mode_pool[GR_VOL_DROP_EN][DCW];
	menu_par[GUI_COM_MODE].unit[1] = mode_pool[GR_VOL_DROP_EN][DCW];
	menu_par[GUI_DCW_OUTPUT_VOL].upper = DCW_VOL_H;
	menu_par[GUI_DCW_OUTPUT_VOL].lower = DCW_VOL_L;
	
	init_dcw_par_info();
    updata_dc_cur_gear_affect_par_info(DCW, pun->dcw.gear_i);
}

static void ir_set_par_way_1(UN_STRUCT *pun)
{
	/* 测试步骤->测试模式->输出电压->自动换档->电阻上限->电阻下限->上升时间->测试时间->延时时间->
	间隔时间->步间PASS->步间连续->测试端口 */
    
    init_par_list();
    tail_insert_option(GUI_COM_STEP			,&pun->ir.step);
    tail_insert_option(GUI_COM_MODE			,&pun->ir.mode);
    tail_insert_option(GUI_IR_OUTPUT_VOL	,&pun->ir.testing_voltage);
    tail_insert_option(GUI_IR_AUTO_GEAR		,&pun->ir.auto_shift_gears);
    tail_insert_option(GUI_IR_RES_H			,&pun->ir.upper_limit);
    tail_insert_option(GUI_IR_RES_L			,&pun->ir.lower_limit);
		
    tail_insert_option(GUI_COM_RISE_T		,&pun->ir.rise_time);
    tail_insert_option(GUI_COM_TEST_T		,&pun->ir.testing_time);
    tail_insert_option(GUI_COM_DELAY_T		,&pun->ir.delay_time);
    tail_insert_option(GUI_COM_INTER_T		,&pun->ir.interval_time);
    tail_insert_option(GUI_COM_STEPS_PASS	,&pun->ir.steps_pass);
    tail_insert_option(GUI_COM_STEPS_CONT	,&pun->ir.steps_cont);
    if(type_spe.port_num != PORT_NUM0)
        tail_insert_option(GUI_COM_PORT		,&pun->ir.port);
}

static void init_ir_par_info(void)
{
    if(g_cur_type->num == CS9922BX_DZ)
    {
        menu_par[GUI_IR_RES_H].range_flag = 4;
        menu_par[GUI_IR_RES_H].lon = 7;
        menu_par[GUI_IR_RES_H].decs = 1;
        
        menu_par[GUI_IR_RES_L].range_flag = 4;
        menu_par[GUI_IR_RES_L].lon = 7;
        menu_par[GUI_IR_RES_L].decs = 1;
        menu_par[GUI_IR_RES_L].lower = 10;
    }
}
static void init_ir_par_list(NODE_STEP *pnode)
{
	int32_t i = 0;
	int32_t j = 0;
	int32_t k = 0;
	int32_t page = 0;
	int32_t last_page_num = 0;
	DATA_INFO *p = NULL;
	int32_t flag = 0;
	UN_STRUCT *pun = NULL;
	
	if(pnode == NULL)
	{
		return;
	}
	pun = &pnode->one_step;
	
	ir_set_par_way_1(pun);
	
	for(p=par_info_list.head,i=0, j=1; i==0 || p!=par_info_list.head; p=p->next)
	{
		if(++i > 3)
		{
			i = 1;
			j++;
		}
		
		p->page = j;
		p->cur = i;
		p->y = GY + (i-1)*OY;
		k++;
		if(p->fun == set_all_ports)
		{
			if(p->cur != 1)
			{
				p->cur = 1;
				p->page++;
				p->y = GY;
				flag = 1;
			}
		}
	}
	
	if(flag)
	{
		k--;
		page = k/3 + (k%3==0?0:1);/* page 不包含端口设置那一页 */
		last_page_num = k%3==0?3:k%3;
	}
	else
	{
		page = k/3 + (k%3==0?0:1);
		last_page_num = k%3==0?3:k%3;
	}
	
	for(p=par_info_list.head,i=0, j=1; i==0 || p!=par_info_list.head; p=p->next,i++)
	{
		
		if(p->fun == set_all_ports)
		{
			p->total = 1;
		}
		else
		{
			if(p->page < page)
			{
				p->total = 3;
			}
			else
			{
				p->total = last_page_num;
			}
		}
	}
	if(pun == NULL)
	{
		return;
	}
	
	menu_par[GUI_COM_MODE].unit[0] 	= mode_pool[GR_VOL_DROP_EN][IR];
	menu_par[GUI_COM_MODE].unit[1] 	= mode_pool[GR_VOL_DROP_EN][IR];
	menu_par[GUI_IR_OUTPUT_VOL].upper 	= IR_VOL_H;
	menu_par[GUI_IR_OUTPUT_VOL].lower 	= IR_VOL_L;
	menu_par[GUI_IR_RES_H].upper		= IR_RES_H;
	menu_par[GUI_IR_RES_L].lower		= IR_RES_L;
    
  	init_ir_par_info();
}
static void bbd_set_par_way_1(UN_STRUCT *pun)
{
	/* 测试步骤->测试模式->输出电压->开路检测->短路检测->测试时间->间隔时间->步间PASS->步间连续->测试端口 */
    init_par_list();
    tail_insert_option(GUI_COM_STEP				, &pun->bbd.step);
    tail_insert_option(GUI_COM_MODE				, &pun->bbd.mode);
    tail_insert_option(GUI_BBD_OUTPUT_VOL		, &pun->bbd.testing_voltage);
    tail_insert_option(GUI_BBD_OPEN_CHECK		, &pun->bbd.open_ratio);
    tail_insert_option(GUI_BBD_SHORT_CHECK		, &pun->bbd.short_ratio);
    
	/* 输出频率 */
//     if(g_cur_type->num != CS9927LB)
//     {
//         tail_insert_option(GUI_COM_OUTPUT_FREQ	, &pun->bbd.output_freq);
//     }
    
    tail_insert_option(GUI_COM_STEPS_PASS		, &pun->bbd.steps_pass);
    tail_insert_option(GUI_COM_STEPS_CONT		, &pun->bbd.steps_cont);
    if(type_spe.port_num != PORT_NUM0)
        tail_insert_option(GUI_COM_PORT			, &pun->bbd.port);
}

//static void bbd_set_par_way_2(UN_STRUCT *pun)
//{
//	/* 测试步骤->测试模式->输出电压->开路检测->短路检测->测试时间->间隔时间->步间PASS->步间连续->测试端口 */
//    init_par_list();
//    tail_insert_option(GUI_COM_STEP				, &pun->bbd.step);
//    tail_insert_option(GUI_COM_MODE				, &pun->bbd.mode);
//    tail_insert_option(GUI_BBD_OUTPUT_VOL		, &pun->bbd.testing_voltage);
//    tail_insert_option(GUI_BBD_CAP_H			, &pun->bbd.upper_limit);
//    tail_insert_option(GUI_BBD_CAP_L			, &pun->bbd.lower_limit);
//	
//    tail_insert_option(GUI_COM_TEST_T			, &pun->bbd.testing_time);
//    
//	/* 输出频率 */
////     if(g_cur_type->num != CS9927LB)
////     {
////         tail_insert_option(GUI_COM_OUTPUT_FREQ	, &pun->bbd.output_freq);
////     }
//    
//    tail_insert_option(GUI_COM_STEPS_PASS		, &pun->bbd.steps_pass);
//    tail_insert_option(GUI_COM_STEPS_CONT		, &pun->bbd.steps_cont);
//    if(type_spe.port_num != PORT_NUM0)
//        tail_insert_option(GUI_COM_PORT			, &pun->bbd.port);
//}
static void init_bbd_par_list(NODE_STEP *pnode)
{
	int32_t i = 0;
	int32_t j = 0;
	int32_t k = 0;
	int32_t page = 0;
	int32_t last_page_num = 0;
	DATA_INFO *p = NULL;
	int32_t flag = 0;
	UN_STRUCT *pun = NULL;
	
	if(pnode == NULL)
	{
		return;
	}
	pun = &pnode->one_step;
	
	/* 使用电容上下限而不是开短路比例 */
//     if(g_cur_type->num == CS9927LB)
// 	{
// 		bbd_set_par_way_2(pun);
// 	}
// 	/* 正常情况 */
// 	else
	{
		bbd_set_par_way_1(pun);
	}
	
	for(p=par_info_list.head,i=0, j=1; i==0 || p!=par_info_list.head; p=p->next)
	{
		if(++i > 3)
		{
			i = 1;
			j++;
		}
		
		p->page = j;
		p->cur = i;
		p->y = GY + (i-1)*OY;
		k++;
		if(p->fun == set_all_ports)
		{
			if(p->cur != 1)
			{
				p->cur = 1;
				p->page++;
				flag = 1;
				p->y = GY;
			}
		}
	}
	
	if(flag)
	{
		k--;
		page = k/3 + (k%3==0?0:1);/* page 不包含端口设置那一页 */
		last_page_num = k%3==0?3:k%3;
	}
	else
	{
		page = k/3 + (k%3==0?0:1);
		last_page_num = k%3==0?3:k%3;
	}
	
	for(p=par_info_list.head,i=0, j=1; i==0 || p!=par_info_list.head; p=p->next,i++)
	{
		
		if(p->fun == set_all_ports)
		{
			p->total = 1;
		}
		else
		{
			if(p->page < page)
			{
				p->total = 3;
			}
			else
			{
				p->total = last_page_num;
			}
		}
	}
	if(pun == NULL)
	{
		return;
	}
	
	menu_par[GUI_COM_MODE].unit[0] = mode_pool[GR_VOL_DROP_EN][BBD];
	menu_par[GUI_COM_MODE].unit[1] = mode_pool[GR_VOL_DROP_EN][BBD];
	menu_par[GUI_BBD_OUTPUT_VOL].upper = BBD_VOL_H;
	menu_par[GUI_BBD_OUTPUT_VOL].lower = BBD_VOL_L;
	
	if(pun->bbd.cap_value * 5 < 10)
	{
		menu_par[GUI_BBD_CAP_H].upper = pun->bbd.cap_value * 1000 * 5;
		menu_par[GUI_BBD_CAP_H].lower = pun->bbd.cap_value * 1000 * 1;
		menu_par[GUI_BBD_CAP_H].decs = 3;
		
		menu_par[GUI_BBD_CAP_L].upper = pun->bbd.cap_value * 1000 * 1;
		menu_par[GUI_BBD_CAP_L].lower = 0;
		menu_par[GUI_BBD_CAP_L].decs = 3;
	}
	else if(pun->bbd.cap_value * 5 < 100)
	{
		menu_par[GUI_BBD_CAP_H].upper = pun->bbd.cap_value * 100 * 5;
		menu_par[GUI_BBD_CAP_H].lower = pun->bbd.cap_value * 100 * 1;
		menu_par[GUI_BBD_CAP_H].decs = 2;
		
		menu_par[GUI_BBD_CAP_L].upper = pun->bbd.cap_value * 100 * 1;
		menu_par[GUI_BBD_CAP_L].lower = 0;
		menu_par[GUI_BBD_CAP_L].decs = 2;
	}
	else if(pun->bbd.cap_value * 5 < 1000)
	{
		menu_par[GUI_BBD_CAP_H].upper = pun->bbd.cap_value * 10 * 5;
		menu_par[GUI_BBD_CAP_H].lower = pun->bbd.cap_value * 10 * 1;
		menu_par[GUI_BBD_CAP_H].decs = 1;
		
		menu_par[GUI_BBD_CAP_L].upper = pun->bbd.cap_value * 10 * 1;
		menu_par[GUI_BBD_CAP_L].lower = 0;
		menu_par[GUI_BBD_CAP_L].decs = 1;
	}
	else
	{
		menu_par[GUI_BBD_CAP_H].upper = pun->bbd.cap_value * 1 * 5;
		menu_par[GUI_BBD_CAP_H].lower = pun->bbd.cap_value * 1 * 1;
		menu_par[GUI_BBD_CAP_H].decs = 0;
		
		menu_par[GUI_BBD_CAP_L].upper = pun->bbd.cap_value * 1 * 1;
		menu_par[GUI_BBD_CAP_L].lower = 0;
		menu_par[GUI_BBD_CAP_L].decs = 0;
	}
}
static void sys_set_par_way_1(SYS_PAR 	*p_sys)
{
    /* 液晶对比度->余量提示->结果保存->溢出覆盖->列表显示->自检允许->语言选择->失败继续->GFI保护->测试端口
    ->测试信号->蜂鸣开关->检验日期->编号规则 */
    init_par_list();
    tail_insert_option(GUI_SYS_CONTRAST		, &p_sys->contrast);
    tail_insert_option(GUI_SYS_REMAIN_HINT	, &p_sys->allowance);
    tail_insert_option(GUI_SYS_RES_SAVE		, &p_sys->is_save_res);
    tail_insert_option(GUI_SYS_VOER_COVER	, &p_sys->is_overflow_cover);
    
    tail_insert_option(GUI_SYS_LIST_DIS		, &p_sys->is_table_dis);
    tail_insert_option(GUI_SYS_SELF_CHECK	, &p_sys->is_self_check);
    tail_insert_option(GUI_SYS_LANGUAGE		, &p_sys->language);
    tail_insert_option(GUI_SYS_FAIL_CONT	, &p_sys->is_falt_continue);
    tail_insert_option(GUI_SYS_FAIL_MODE	, &p_sys->fail_mode);
    
    /* PLC 信号使能 */
    if(type_spe.plc_signal_en == ENABLE)
    {
        tail_insert_option(GUI_SYS_PLC_SIGNAL, &p_sys->plc_signal);
    }
    
    tail_insert_option(GUI_SYS_OUTPUT_DELAY	, &p_sys->output_delay);
	
    tail_insert_option(GUI_SYS_GFI_PROT		, &p_sys->is_gfi_protect);
    tail_insert_option(GUI_SYS_TEST_PORT	, &p_sys->test_method);
    tail_insert_option(GUI_SYS_TEST_SIGNAL	, &p_sys->test_level);
    tail_insert_option(GUI_SYS_BUZZER_SW	, &p_sys->buzzer_sw);
    
    /* 静音功能使能 */
    if(type_spe.silent_en == ENABLE)
    {
        tail_insert_option(GUI_SYS_SILENT_SW, &p_sys->silent_sw);
    }
	
    tail_insert_option(GUI_SYS_CHECK_DATE	, &p_sys->data_of_survey);
    tail_insert_option(GUI_SYS_NO_RULE		, &p_sys->num_rule);
    
	if(g_custom_sys_par.ir_gear_hold == SYS_SW_ON)
	{
		tail_insert_option(GUI_SYS_IR_GEAR_HOLD	, &p_sys->ir_gear_hold);
	}
	
	if(g_custom_sys_par.ir_speed_sw == SYS_SW_ON)
	{
		tail_insert_option(GUI_SYS_IR_SPEED	, &p_sys->ir_speed_sw);
	}
	
	par_info_list.head->prev = par_info_list.tail;//形成环形链表
}
static void sys_set_par_way_for_single_gr(SYS_PAR 	*p_sys)
{
    /* 液晶对比度->余量提示->结果保存->溢出覆盖->列表显示->自检允许->语言选择->失败继续
    ->测试信号->蜂鸣开关->检验日期->编号规则 */
    
    init_par_list();
    tail_insert_option(GUI_SYS_CONTRAST		, &p_sys->contrast);
    tail_insert_option(GUI_SYS_REMAIN_HINT	, &p_sys->allowance);
    tail_insert_option(GUI_SYS_RES_SAVE		, &p_sys->is_save_res);
    tail_insert_option(GUI_SYS_VOER_COVER	, &p_sys->is_overflow_cover);
    
    tail_insert_option(GUI_SYS_LIST_DIS		, &p_sys->is_table_dis);
    tail_insert_option(GUI_SYS_SELF_CHECK	, &p_sys->is_self_check);
    tail_insert_option(GUI_SYS_LANGUAGE		, &p_sys->language);
    tail_insert_option(GUI_SYS_FAIL_CONT	, &p_sys->is_falt_continue);
	
    tail_insert_option(GUI_SYS_TEST_SIGNAL	, &p_sys->test_level);
    tail_insert_option(GUI_SYS_BUZZER_SW	, &p_sys->buzzer_sw);
    
    if(type_spe.silent_en == ENABLE)
    {
        tail_insert_option(GUI_SYS_SILENT_SW, &p_sys->silent_sw);
    }
    
    tail_insert_option(GUI_SYS_CHECK_DATE	, &p_sys->data_of_survey);
    tail_insert_option(GUI_SYS_NO_RULE		, &p_sys->num_rule);
    
	par_info_list.head->prev = par_info_list.tail;//形成环形链表
}

static void sys_set_par_way_2(SYS_PAR 	*p_sys)
{
    /* 液晶对比度->余量提示->结果保存->溢出覆盖->列表显示->自检允许->语言选择->失败继续
    ->测试信号->蜂鸣开关->检验日期->编号规则 */
    
    init_par_list();
    tail_insert_option(GUI_SYS_CONTRAST		, &p_sys->contrast);
    tail_insert_option(GUI_SYS_REMAIN_HINT	, &p_sys->allowance);
    tail_insert_option(GUI_SYS_RES_SAVE		, &p_sys->is_save_res);
    tail_insert_option(GUI_SYS_VOER_COVER	, &p_sys->is_overflow_cover);
    
    tail_insert_option(GUI_SYS_LIST_DIS		, &p_sys->is_table_dis);
    tail_insert_option(GUI_SYS_SELF_CHECK	, &p_sys->is_self_check);
    tail_insert_option(GUI_SYS_LANGUAGE		, &p_sys->language);
    tail_insert_option(GUI_SYS_FAIL_CONT	, &p_sys->is_falt_continue);
    
    tail_insert_option(GUI_SYS_GFI_PROT		, &p_sys->is_gfi_protect);
    tail_insert_option(GUI_SYS_TEST_PORT	, &p_sys->test_method);
    tail_insert_option(GUI_SYS_TEST_SIGNAL	, &p_sys->test_level);
    tail_insert_option(GUI_SYS_BUZZER_SW	, &p_sys->buzzer_sw);
    
    if(type_spe.silent_en == ENABLE)
    {
        tail_insert_option(GUI_SYS_SILENT_SW	, &p_sys->silent_sw);
    }
    
    tail_insert_option(GUI_SYS_CHECK_DATE	, &p_sys->data_of_survey);
    tail_insert_option(GUI_SYS_NO_RULE		, &p_sys->num_rule);
    
	par_info_list.head->prev = par_info_list.tail;//形成环形链表
}
static void sys_set_par_way_cs9929ax_f(SYS_PAR 	*p_sys)
{
    /* 液晶对比度->余量提示->结果保存->溢出覆盖->列表显示->自检允许->语言选择->失败继续->GFI保护->测试端口
    ->测试信号->蜂鸣开关->检验日期->编号规则 */
    init_par_list();
    tail_insert_option(GUI_SYS_CONTRAST		, &p_sys->contrast);
    tail_insert_option(GUI_SYS_REMAIN_HINT	, &p_sys->allowance);
    tail_insert_option(GUI_SYS_RES_SAVE		, &p_sys->is_save_res);
    tail_insert_option(GUI_SYS_VOER_COVER	, &p_sys->is_overflow_cover);
    
    tail_insert_option(GUI_SYS_LIST_DIS		, &p_sys->is_table_dis);
    tail_insert_option(GUI_SYS_SELF_CHECK	, &p_sys->is_self_check);
    tail_insert_option(GUI_SYS_LANGUAGE		, &p_sys->language);
    
    tail_insert_option(GUI_SYS_GFI_PROT		, &p_sys->is_gfi_protect);
    tail_insert_option(GUI_SYS_TEST_PORT	, &p_sys->test_method);
    tail_insert_option(GUI_SYS_TEST_SIGNAL	, &p_sys->test_level);
    tail_insert_option(GUI_SYS_BUZZER_SW	, &p_sys->buzzer_sw);
    
    if(type_spe.silent_en == ENABLE)
    {
        tail_insert_option(GUI_SYS_SILENT_SW, &p_sys->silent_sw);
    }
    
    tail_insert_option(GUI_SYS_CHECK_DATE	, &p_sys->data_of_survey);
    tail_insert_option(GUI_SYS_NO_RULE		, &p_sys->num_rule);
    
	par_info_list.head->prev = par_info_list.tail;//形成环形链表
}
void init_sys_par_list(SYS_PAR 	*p_sys)
{
	int32_t i = 0;
	int32_t j = 0;
	int32_t k = 0;
	int32_t page = 0;
	int32_t last_page_num = 0;
	DATA_INFO *p = NULL;
	
	int32_t page_num = 4;/* 每页的参数个数 */
    
    if(g_cur_type->num == CS9929AX_F)
    {
        sys_set_par_way_cs9929ax_f(p_sys);
    }
    else if(type_spe.single_gr)
    {
        sys_set_par_way_for_single_gr(p_sys);
    }
    /* 单直流压降模式 */
    else if(ENABLE == GR_VOL_DROP_EN)
	{
		sys_set_par_way_2(p_sys);
	}
    /* 普通模式 */
	else
	{
		sys_set_par_way_1(p_sys);
	}
	
	for(p=par_info_list.head, i = 0, j = 1; i==0 || p!=par_info_list.head; p=p->next)
	{
		if(++i > page_num)
		{
			i = 1;
			j++;
		}
		
		if(par_info_list.tail == p)
		{
			j = j;
		}
        
		p->page = j;
		p->cur = i;
		p->y = 3 + (i - 1) * 13;
		k++;
	}
	
	page = k / page_num + (k % page_num == 0 ? 0 : 1);
	last_page_num = k % page_num == 0? page_num : k % page_num;
	
	for(p=par_info_list.head,i=0; i==0 || p!=par_info_list.head; p=p->next,i++)
	{
		if(p->page < page)
		{
			p->total = page_num;
		}
		else
		{
			p->total = last_page_num;
		}
	}
	
	if(p_sys == NULL)
	{
		return;
	}
    
    init_plc_signal_options_list(&menu_par[GUI_SYS_PLC_SIGNAL]);/* 初始化PLC信号选项链表 */
	init_ir_filtering_deep_options_list(&menu_par[GUI_SYS_IR_SPEED]);/* 初始化IR测试速度选项链表 */
	
}
void custom_par_way_1(CUSTOM_SYS_PAR *p_custom)
{
    init_par_list();
    tail_insert_option(GUI_CUSTOM_AMP_SELECT	, &p_custom->amp_select);
    tail_insert_option(GUI_CUSTOM_AMP_TYPE		, &p_custom->amp_type);
    tail_insert_option(GUI_CUSTOM_PAR_MEDIUM	, &p_custom->par_medium);
    tail_insert_option(GUI_CUSTOM_RES_MEDIUM	, &p_custom->res_medium);
    tail_insert_option(GUI_CUSTOM_CAL_MEDIUM	, &p_custom->cal_medium);
    tail_insert_option(GUI_CUSTOM_BUZZER_EN		, &p_custom->buzzer_en);
    tail_insert_option(GUI_CUSTOM_MUTE_SW		, &p_custom->mute_sw);
    tail_insert_option(GUI_CUSTOM_LEADING_SW	, &p_custom->leading_sw);
    tail_insert_option(GUI_CUSTOM_IR_G_HOLD_SW	, &p_custom->ir_gear_hold);
    tail_insert_option(GUI_CUSTOM_IR_SPEED_SW	, &p_custom->ir_speed_sw);
    tail_insert_option(GUI_CUSTOM_OFFSET_SW		, &p_custom->offset_set_en);
    
	par_info_list.head->prev = par_info_list.tail;//形成环形链表
}

const uint8_t *get_cur_amp_type_name(uint8_t language)
{
    uint8_t temp = read_par_to_buf(&menu_par[GUI_CUSTOM_AMP_TYPE], NULL);
    
    if(language == CHINESE)
    {
        return amp_type_option_pool[temp].name[0];
    }
    else
    {
        return amp_type_option_pool[temp].name[1];
    }
}

const uint8_t *get_cur_par_medium_name(uint8_t language)
{
    uint8_t temp = read_par_to_buf(&menu_par[GUI_CUSTOM_PAR_MEDIUM], NULL);
    
    if(language == CHINESE)
    {
        return par_medium_option_pool[temp].name[0];
    }
    else
    {
        return par_medium_option_pool[temp].name[1];
    }
}
const uint8_t *get_cur_res_medium_name(uint8_t language)
{
    uint8_t temp = read_par_to_buf(&menu_par[GUI_CUSTOM_RES_MEDIUM], NULL);
    
    if(language == CHINESE)
    {
        return res_medium_option_pool[temp].name[0];
    }
    else
    {
        return res_medium_option_pool[temp].name[1];
    }
}
const uint8_t *get_cur_cal_medium_name(uint8_t language)
{
    uint8_t temp = read_par_to_buf(&menu_par[GUI_CUSTOM_CAL_MEDIUM], NULL);
    
    if(language == CHINESE)
    {
        return cal_medium_option_pool[temp].name[0];
    }
    else
    {
        return cal_medium_option_pool[temp].name[1];
    }
}
/*
 * 函数名：init_custom_par_list
 * 描述  ：初始化定制参数链表
 * 输入  ：void* p_arg 没有用
 * 输出  ：无
 * 返回  ：无
 */
void init_custom_par_list(CUSTOM_SYS_PAR *p_custom)
{
	int32_t i = 0;
	int32_t j = 0;
	int32_t k = 0;
	int32_t page = 0;
	int32_t last_page_num = 0;
	DATA_INFO *p = NULL;
	
	int32_t page_num = 4;/* 每页的参数个数 */
    
    custom_par_way_1(p_custom);
	
	for(p=par_info_list.head, i = 0, j = 1; i==0 || p!=par_info_list.head; p=p->next)
	{
		if(++i > page_num)
		{
			i = 1;
			j++;
		}
        
		p->page = j;
		p->cur = i;
		p->y = 3 + (i - 1) * 13;
		k++;
	}
	
	page = k / page_num + (k % page_num == 0 ? 0 : 1);
	last_page_num = k % page_num == 0? page_num : k % page_num;
	
	for(p=par_info_list.head,i=0; i==0 || p!=par_info_list.head; p=p->next,i++)
	{
		if(p->page < page)
		{
			p->total = page_num;
		}
		else
		{
			p->total = last_page_num;
		}
	}
	
	if(p_custom == NULL)
	{
		return;
	}
    
    init_custom_options_list(GUI_CUSTOM_AMP_SELECT);
    init_custom_options_list(GUI_CUSTOM_PAR_MEDIUM);
    init_custom_options_list(GUI_CUSTOM_RES_MEDIUM);
    init_custom_options_list(GUI_CUSTOM_CAL_MEDIUM);
    
    menu_par[GUI_CUSTOM_AMP_SELECT].unit[0] = get_cur_amp_type_name(CHINESE);
    menu_par[GUI_CUSTOM_AMP_SELECT].unit[1] = get_cur_amp_type_name(ENGLISH);
    
    menu_par[GUI_CUSTOM_PAR_MEDIUM].unit[0] = get_cur_par_medium_name(CHINESE);
    menu_par[GUI_CUSTOM_PAR_MEDIUM].unit[1] = get_cur_par_medium_name(ENGLISH);
    
    menu_par[GUI_CUSTOM_RES_MEDIUM].unit[0] = get_cur_res_medium_name(CHINESE);
    menu_par[GUI_CUSTOM_RES_MEDIUM].unit[1] = get_cur_res_medium_name(ENGLISH);
    
    menu_par[GUI_CUSTOM_CAL_MEDIUM].unit[0] = get_cur_cal_medium_name(CHINESE);
    menu_par[GUI_CUSTOM_CAL_MEDIUM].unit[1] = get_cur_cal_medium_name(ENGLISH);
    
}
static void init_com_par(void)
{
	menu_par[GUI_COM_TEST_T].upper_str = (const unsigned char*)"999.9s";
	menu_par[GUI_COM_TEST_T].lower_str = (const unsigned char*)"0,0.3s";
	menu_par[GUI_COM_TEST_T].lower = 3;
    
    if(g_cur_file->arc_mode == ARC_CUR_MODEL)
    {
        menu_par[GUI_ACW_ARC_SUR].lon = 5;
        menu_par[GUI_ACW_ARC_SUR].decs = 2;
        menu_par[GUI_ACW_ARC_SUR].range_flag = 0;
        
        menu_par[GUI_DCW_ARC_SUR].lon = 5;
        menu_par[GUI_DCW_ARC_SUR].decs = 2;
        menu_par[GUI_DCW_ARC_SUR].range_flag = 0;
    }
    else
    {
        menu_par[GUI_ACW_ARC_SUR].lon = 1;
        menu_par[GUI_ACW_ARC_SUR].decs = 0;
        menu_par[GUI_ACW_ARC_SUR].range_flag = 3;
        
        menu_par[GUI_DCW_ARC_SUR].lon = 1;
        menu_par[GUI_DCW_ARC_SUR].decs = 0;
        menu_par[GUI_DCW_ARC_SUR].range_flag = 3;
    }
    
	switch(g_cur_file->work_mode)
	{
		case N_MODE:
			menu_par[GUI_COM_INTER_T].name[0] = (const unsigned char*)"间隔时间";
			menu_par[GUI_COM_INTER_T].name[1] = (const unsigned char*)"Interval";
			break;
		case G_MODE:
			menu_par[GUI_COM_INTER_T].name[0] = (const unsigned char*)"缓变时间";
			menu_par[GUI_COM_INTER_T].name[1] = (const unsigned char*)"Change";
			break;
	}
}
void init_par_info_list(NODE_STEP *p_node)
{
	switch(p_node->one_step.com.mode)
	{
		case ACW:
			init_acw_par_list(p_node);
            init_acw_par_info();
			break;
		case DCW:
			init_dcw_par_list(p_node);
			break;
		case IR:
			init_ir_par_list(p_node);
			break;
		case GR:
			init_gr_par_list(p_node);
			break;
		case BBD:
			init_bbd_par_list(p_node);
            init_acw_par_info();
			break;
        case CC:
            init_cc_par_list(p_node);
            break;
	}
	
	init_com_par();
}
/******************* (C) COPYRIGHT 2014 长盛仪器 *****END OF FILE*******************/
