#ifndef LUX_EVENT_OBJECT_H
#define LUX_EVENT_OBJECT_H

#ifdef __cplusplus
extern "C"
{
#endif

/*事件类型*/
typedef enum
{
    NONE=0,
    VCA_CROWD,

}enum_event_type;

/*事件对象*/
typedef	struct event
{
	enum_event_type event_value;
	void (*pset_value)(struct event *event_info,enum_event_type value);
	enum_event_type (*pget_value)(struct event *event_info);
	void (*pclear_value)(struct event *event_info);
	
}struct_event_info;

/*构造函数*/
void constructor(struct event *event_info);

#ifdef __cplusplus
}
#endif
#endif