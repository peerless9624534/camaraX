/*
 * lux_osd.h
 *
 * OSD叠加功能
 * 
 */

#ifndef __LUX_OSD_H__
#define __LUX_OSD_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

#include <comm_def.h>
#include <comm_func.h>
#include <comm_error_code.h>
#include "ivs_common.h"

/*数量*/
#define LUX_OSD_GRP_NUM 		2
#define LUX_LAST_REGION_NUM     3

#define LUX_FACTRECT_NUM_MAX    5   //人脸矩形框最大数量，根据头文件ivs_inf_faceDet.h中的NUM_OF_FACES选定 

#define LUX_HUMANRECT_NUM_MAX   5   //人形矩形框最大数量，根据头文件ivs_inf_personDet.h中的NUM_OF_PERSONS选定
#define LUX_LOITER_NUM_MAX      LUX_HUMANRECT_NUM_MAX   //徘徊人形最大数量

#define LUX_PERM_OBJ_NUM_MAX    8   //周界越界物体矩形框最大数量

#define LUX_UM_AREA_NUM_MAX     6   //物品遗留报警区域最大数量

#define LUX_UM_OBJLEFT_NUM_MAX  8   //物品遗留目标矩形框最大数量
#define LUX_UM_OBJLOSS_NUM_MAX  (LUX_UM_OBJLEFT_NUM_MAX)   //物品遗留目标矩形框最大数量


#define LUX_LINECROSS_LINE_MAX  6   //越线侦测线最大数量
#define LUX_SL_OBJ_NUM_MAX      5   //单线越线最大目标数
#define LUX_LINECROSS_OBJ_MAX   (LUX_LINECROSS_LINE_MAX * LUX_SL_OBJ_NUM_MAX) //越线侦测目标最大数量

#define LUX_POLYGON_PERM_MAX    6   //多边形周界框最大数量
#define LUX_POLYGON_LINE_MAX    16  //多边形周界最大边数
#define LUX_PERMLINE_NUM_MAX    (LUX_POLYGON_PERM_MAX * LUX_POLYGON_LINE_MAX)//斜线最大数量

#define LUX_RECT_NUM_MAX        (LUX_FACTRECT_NUM_MAX + LUX_HUMANRECT_NUM_MAX + LUX_PERM_OBJ_NUM_MAX +\
								 LUX_UM_AREA_NUM_MAX + LUX_UM_OBJLEFT_NUM_MAX + LUX_UM_OBJLOSS_NUM_MAX +\
								 LUX_LOITER_NUM_MAX + LUX_LINECROSS_OBJ_MAX) //osd矩形框最大数量
#define LUX_SLASH_NUM_MAX       (LUX_PERMLINE_NUM_MAX + LUX_LINECROSS_LINE_MAX) //osd斜线最大数量
#define LUX_OSD_REGION_NUM      (LUX_SLASH_NUM_MAX + LUX_RECT_NUM_MAX + LUX_LAST_REGION_NUM) //osd区域数


/*位置*/
#define LUX_FACE_START_POS          LUX_LAST_REGION_NUM
#define LUX_FACE_END_POS            (LUX_FACE_START_POS + LUX_FACTRECT_NUM_MAX)

#define LUX_HUMAN_START_POS         LUX_FACE_END_POS
#define LUX_HUMAN_END_POS           (LUX_HUMAN_START_POS + LUX_HUMANRECT_NUM_MAX)

#define LUX_PERM_OBJ_START_POS      (LUX_HUMAN_END_POS)
#define LUX_PERM_OBJ_END_POS        (LUX_PERM_OBJ_START_POS + LUX_PERM_OBJ_NUM_MAX)

#define LUX_UM_AREA_START_POS       (LUX_PERM_OBJ_END_POS)
#define LUX_UM_AREA_END_POS         (LUX_UM_AREA_START_POS + LUX_UM_AREA_NUM_MAX)

#define LUX_UM_OBJLEFT_START_POS    (LUX_UM_AREA_END_POS)
#define LUX_UM_OBJLEFT_END_POS      (LUX_UM_OBJLEFT_START_POS + LUX_UM_OBJLEFT_NUM_MAX)

#define LUX_UM_OBJLOSS_START_POS    LUX_UM_OBJLEFT_END_POS
#define LUX_UM_OBJLOSS_END_POS      (LUX_UM_OBJLOSS_START_POS + LUX_UM_OBJLOSS_NUM_MAX)

#define LUX_LOITER_START_POS        (LUX_UM_OBJLOSS_END_POS)
#define LUX_LOITER_END_POS          (LUX_LOITER_START_POS + LUX_LOITER_NUM_MAX)

#define LUX_CROSSLINE_OBJ_START_POS (LUX_LOITER_END_POS)
#define LUX_CROSSLINE_OBJ_END_POS   (LUX_CROSSLINE_OBJ_START_POS + LUX_LINECROSS_OBJ_MAX)

#define LUX_LINECROSS_START_POS     LUX_CROSSLINE_OBJ_END_POS
#define LUX_LINECROSS_END_POS       (LUX_LINECROSS_START_POS + LUX_LINECROSS_LINE_MAX)

#define LUX_PERMSLASH_START_POS     LUX_LINECROSS_END_POS
#define LUX_PERMSLASH_END_POS       (LUX_PERMSLASH_START_POS + LUX_PERMLINE_NUM_MAX)


typedef struct {
	UINT_X enable;
	PCHAR_X RgnName;
	IMPOSDRgnAttr OSDRgnAttr[LUX_OSD_GRP_NUM];
	IMPOSDGrpRgnAttr OSDGrpRgnAttr;
}LUX_OSD_ATTR_ST;

typedef struct
{
    INT_X lineNum;//线编号
    IVSLine line;
}LUX_OSD_SLASHLINE_ST;

typedef struct
{
    IVSRect rects[LUX_UM_OBJLEFT_NUM_MAX];         /**< 物品遗留丢失目标的矩形信息 */
    INT_X rectcnt;                              /**< 物品遗留丢失目标的数量 */
}LUX_UMDET_OBJ_ST, *P_LUX_UMDET_OBJ_ST;

typedef struct
{
	IVSRect box[LUX_LINECROSS_OBJ_MAX];         /**< 越线目标的矩形信息 */
    INT_X boxcnt;                              /**< 越线目标的数量 */
}LUX_CROSSLINEDET_OBJ_ST, *P_LUX_CROSSLINEDET_OBJ_ST;


typedef struct
{
	INT_X slashCnt;//斜线数
	IVSLine slash[LUX_LINECROSS_LINE_MAX];
}LUX_M_SLASH_ST, *P_LUX_M_SLASH_ST;

VOID_X LUX_OSD_CrossLine_OBJ_Show(P_LUX_CROSSLINEDET_OBJ_ST CLDetRet);

VOID_X LUX_OSD_CrossLine_OBJ_Miss();

VOID_X LUX_OSD_LineCross_Show(P_LUX_M_SLASH_ST pLine, BOOL_X bShow);

VOID_X LUX_OSD_UMDetArea_Rect_Show(P_LUX_UMDET_OBJ_ST ivsUMArea, BOOL_X bShow);

VOID_X LUX_OSD_UMDetOBJLeft_Rect_Show(P_LUX_UMDET_OBJ_ST ivsUMDetRet);

VOID_X LUX_OSD_UMDetOBJLeft_Rect_Miss();

VOID_X LUX_OSD_UMDetOBJLoss_Rect_Show(P_LUX_UMDET_OBJ_ST ivsUMLoss, BOOL_X bShow);

VOID_X LUX_OSD_UMDetOBJLoss_Rect_ChangeColor(P_LUX_UMDET_OBJ_ST ivsUMDetRet, IMPOsdColour color);

VOID_X LUX_OSD_PermPolygon_Show(BOOL_X bShow);

VOID_X LUX_ISPOSD_PermRect_Draw(BOOL_X enable);

VOID_X LUX_OSD_PermOBJ_Rect_Miss();

VOID_X LUX_OSD_PersonRect_Miss();

VOID_X LUX_OSD_FaceRect_Miss();

/**
 * @description: 设置创建的几个组区域是否显示，
 * @return     ：0 设置成功. 非0 设置失败
 */
INT_X LUX_OSD_Show(BOOL_X bShow);

/**
 * @description: 获取sensor的像素宽度和高度，重置主码流各OSD区域的位置参数
* @return	  ：0 成功. 非0 失败
*/
INT_X  LUX_OSD_SetRgnPositionParams(int rgnNum);

/**
 * @description: 创建OSD区域、注册OSD区域到OSD组、设置OSD组区域属性和区域属性，打开OSD开关.
 * @return     ：返回指向OSD创建区域的指针
 */
INT_X *LUX_OSD_CreateRgn(int grpNum);

/**
 * @description: 创建OSD的bgramap更新线程，
 * @return     ：0 成功. 非0 失败
 */
INT_X LUX_OSD_Time_Stamp_Update();

/**
 * @description: 创建OSD组.
 * @returnreturn 0 成功. 非0 失败
 * @remarks 系统初始化后时间戳自动被初始化。系统去初始化后时间戳失效.
 */
INT_X LUX_OSD_CreateGroup(INT_X grpNum);

/**
 * @description: OSD始化
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_OSD_Init();

/**
 * @description: OSD初始化
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_OSD_Exit();


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __LUX_OSD_H__ */
