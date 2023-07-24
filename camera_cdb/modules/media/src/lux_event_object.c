/*******************************************************************************
*文件名   : lux_event_object.c
*作用       : 事件对象实现
*作者       : 唐兆伟
*******************************************************************************/

#include<lux_event_object.h>

/**
 * 描述	:   发送事件
 * 输入参数  : event_info 具体对象，value 具体对象发生的事件
 * 返回参数  : 
 * 说明：
**/
static void set_value(struct event *event_info,enum_event_type value)
{
	event_info->event_value = value;
}

/**
 * 描述	:   获取事件
 * 输入参数  : event_info 具体对象
 * 返回参数  : 具体对象的事件值
 * 说明：
**/
static enum_event_type get_value(struct event *event_info)
{
	return event_info->event_value;
}

/**
 * 描述	:   清除事件
 * 输入参数  : event_info 具体对象
 * 返回参数  : 无
 * 说明：
**/
static void clear_value(struct event *event_info)
{
	event_info->event_value = NONE; 
} 

/**
 * 描述	:   构造事件对象
 * 输入参数  : event_info 具体对象
 * 返回参数  : 无
 * 说明：
**/
void constructor(struct event *event_info)
{
	event_info->event_value =NONE;
	event_info->pclear_value = clear_value;
	event_info->pget_value = get_value;
	event_info->pset_value = set_value;
}