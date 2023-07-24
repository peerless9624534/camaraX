/*
 * lux_osd.c
 *
 * 基于Ingenic平台封装的osd模块，包括时间显示、画线等
 * 
 */

#include <lux_osd.h>
#include <imp_osd.h>
#include <imp_encoder.h>
#include <imp_log.h>
#include <imp_common.h>
#include <string.h>
#include <lux_base.h>
#include <stdlib.h>
#include <lux_iniparse.h>
#include <lux_global.h>
#include "lux_ivs.h"
#include "lux_video.h"

#include "bitmapinfo_1440p.h"
#include "bitmapinfo_1080p.h"
#include "bitmapinfo_360p.h"
#include "lux_config.h"
#include "lux_perm_det.h"
// #include "logodata_106x58_bgra.h"
// #include "logodata_382x88_bgra.h"
// #include "logodata_436x236_bgra.h"

#define LUX_OSD_LETTER_NUM 		20

#ifdef CONFIG_PTZ_IPC
#define LUX_OSD_GRP0_LOGO_W 436
#define LUX_OSD_GRP0_LOGO_H 236
#else
#define LUX_OSD_GRP0_LOGO_W 382
#define LUX_OSD_GRP0_LOGO_H 88
#endif
#define LUX_OSD_GRP1_LOGO_W 106
#define LUX_OSD_GRP1_LOGO_H 58


#define LUX_OSD_FONT_GRP0_START_X  10
#define LUX_OSD_FONT_GRP0_START_Y  10
#define LUX_OSD_FONT_GRP1_START_X  5
#define LUX_OSD_FONT_GRP1_START_Y  5

#define LUX_OSD_LOGO_GRP0_START_X  16
#define LUX_OSD_LOGO_GRP0_START_Y (SENSOR_HEIGHT_DEFAULT - LUX_OSD_GRP0_LOGO_H - 12)
#define LUX_OSD_LOGO_GRP1_START_X  8
#define LUX_OSD_LOGO_GRP1_START_Y (SENSOR_HEIGHT_SECOND - LUX_OSD_GRP1_LOGO_H - 6)

#define LUX_OSD_COVER_GRP0_START_X SENSOR_WIDTH_DEFAULT/2-100
#define LUX_OSD_COVER_GRP0_START_Y SENSOR_HEIGHT_DEFAULT/2-100
#define LUX_OSD_COVER_GRP1_START_X SENSOR_WIDTH_SECOND/2-100
#define LUX_OSD_COVER_GRP1_START_Y SENSOR_HEIGHT_SECOND/2-100

#define LUX_OSD_RECT_GRP0_START_X  300
#define LUX_OSD_RECT_GRP0_START_Y  300
#define LUX_OSD_RECT_GRP1_START_X  100
#define LUX_OSD_RECT_GRP1_START_Y  100


#define TAG "lux_osd"

typedef struct
{
    int regionNum;//区域编号
    int lineNum;//线编号
    IVSLine line[LUX_PERMDET_REGION_NUM][MAX_PERM_POINTS_NUM];
}LUX_ISPOSD_DRWALINE_ST;

IMPRgnHandle *g_prHander[LUX_OSD_GRP_NUM];
LUX_COMM_FUNC_SNS_WH g_tSensorWH;

 #ifdef CONFIG_PTZ_IPC
 #define LOGO_SIZE_HD  411584
static uint8_t logodata_436x236_bgra[LOGO_SIZE_HD];
#else
#define LOGO_SIZE_HD  134464
static uint8_t logodata_382x88_bgra[LOGO_SIZE_HD];
#endif
#define LOGO_SIZE_SD  24592
static uint8_t logodata_106x58_bgra[LOGO_SIZE_SD];

LUX_OSD_ATTR_ST g_osdAttr[LUX_OSD_REGION_NUM] =
    {   
        {/*g_osdAttr[0] --- font*/
            .enable = 1,
            .RgnName = "font",
            .OSDRgnAttr =
                {
                    {
                        .type = OSD_REG_PIC,
                        .rect.p0.x = LUX_OSD_FONT_GRP0_START_X,
                        .rect.p0.y = LUX_OSD_FONT_GRP0_START_Y,
                        .rect.p1.x = LUX_OSD_FONT_GRP0_START_X + 20 * OSD_GRP0_REGION_1080P_W - 1, //p0 is start，and p1 well be epual p0+width(or heigth)-1
                        .rect.p1.y = LUX_OSD_FONT_GRP0_START_Y + OSD_GRP0_REGION_1080P_H - 1,
                        .fmt = PIX_FMT_BGRA,
                        .data.picData.pData = NULL,
                    },
                    {
                        .type = OSD_REG_PIC,
                        .rect.p0.x = LUX_OSD_FONT_GRP1_START_X,
                        .rect.p0.y = LUX_OSD_FONT_GRP1_START_Y,
                        .rect.p1.x = LUX_OSD_FONT_GRP1_START_X + 20 * OSD_GRP1_REGION_360P_W - 1, //p0 is start，and p1 well be epual p0+width(or heigth)-1
                        .rect.p1.y = LUX_OSD_FONT_GRP1_START_Y + OSD_GRP1_REGION_360P_H - 1,
                        .fmt = PIX_FMT_BGRA,
                        .data.picData.pData = NULL,
                    },
                },
            .OSDGrpRgnAttr =
                {
                    .show = 0,
                    .gAlphaEn = 1,
                    .fgAlhpa = 0xff,
                    .layer = 3,
                },
        },
        {/*g_osdAttr[1] --- logo*/
            .enable = 1,
            .RgnName = "logo",
            .OSDRgnAttr =
                {
                    {
                        .type = OSD_REG_PIC,
                        .rect.p0.x = LUX_OSD_LOGO_GRP0_START_X,
                        .rect.p0.y = LUX_OSD_LOGO_GRP0_START_Y,
                        .rect.p1.x = LUX_OSD_LOGO_GRP0_START_X + LUX_OSD_GRP0_LOGO_W - 1,
                        .rect.p1.y = LUX_OSD_LOGO_GRP0_START_Y + LUX_OSD_GRP0_LOGO_H - 1,
                        .fmt = PIX_FMT_BGRA,
// #ifdef CONFIG_PTZ_IPC
//                         .data.picData.pData = logodata_436x236_bgra,
// #else
//                         .data.picData.pData = logodata_382x88_bgra,
// #endif
                    },
                    {
                        .type = OSD_REG_PIC,
                        .rect.p0.x = LUX_OSD_LOGO_GRP1_START_X,
                        .rect.p0.y = LUX_OSD_LOGO_GRP1_START_Y,
                        .rect.p1.x = LUX_OSD_LOGO_GRP1_START_X + LUX_OSD_GRP1_LOGO_W - 1,
                        .rect.p1.y = LUX_OSD_LOGO_GRP1_START_Y + LUX_OSD_GRP1_LOGO_H - 1,
                        .fmt = PIX_FMT_BGRA,
                        // .data.picData.pData = logodata_106x58_bgra,
                    },
                },
            .OSDGrpRgnAttr =
                {
                    .show = 0,
                    .gAlphaEn = 1,
                    .fgAlhpa = 0x7f,
                    .layer = 2,
                },
        },
        {/*g_osdAttr[2] --- cover*/
            .enable = 1,
            .RgnName = "cover",
            .OSDRgnAttr =
                {
                    {
                        .type = OSD_REG_COVER,
                        .rect.p0.x = LUX_OSD_COVER_GRP0_START_X,
                        .rect.p0.y = LUX_OSD_COVER_GRP0_START_Y,
                        .rect.p1.x = LUX_OSD_COVER_GRP0_START_X + SENSOR_WIDTH_DEFAULT / 2 - 1 + 50,
                        .rect.p1.y = LUX_OSD_COVER_GRP0_START_Y + SENSOR_HEIGHT_DEFAULT / 2 - 1 + 50,
                        .fmt = PIX_FMT_BGRA,
                        .data.coverData.color = OSD_BLACK,
                    },
                    {
                        .type = OSD_REG_COVER,
                        .rect.p0.x = LUX_OSD_COVER_GRP1_START_X,
                        .rect.p0.y = LUX_OSD_COVER_GRP1_START_Y,
                        .rect.p1.x = LUX_OSD_COVER_GRP1_START_X + SENSOR_WIDTH_SECOND / 2 - 1 + 50,
                        .rect.p1.y = LUX_OSD_COVER_GRP1_START_Y + SENSOR_HEIGHT_SECOND / 2 - 1 + 50,
                        .fmt = PIX_FMT_BGRA,
                        .data.coverData.color = OSD_BLACK,
                    },
                },
            .OSDGrpRgnAttr =
                {
                    .show = 0,
                    .gAlphaEn = 1,
                    .fgAlhpa = 0x7f,
                    .layer = 2,
                },
        },
        {/*g_osdAttr[3] --- rect*/
            .enable = 1,
            .RgnName = "lineRect",//矩形框,用于人形移动识别、人脸识别、周界越界目标识别，红框人形移动、绿框人脸、黄框越界目标 斜线显示多边形周界
            .OSDRgnAttr =
            {
                {
                    .type = OSD_REG_RECT,
                    .rect.p0.x = LUX_OSD_RECT_GRP0_START_X,
                    .rect.p0.y = LUX_OSD_RECT_GRP0_START_Y,
                    .rect.p1.x = LUX_OSD_RECT_GRP0_START_X + 600 - 1,
                    .rect.p1.y = LUX_OSD_RECT_GRP0_START_Y + 300 - 1,
                    .fmt = PIX_FMT_MONOWHITE,
                    .data.lineRectData.color = OSD_GREEN,
                    .data.lineRectData.linewidth = 0,
                },
                {
                    .type = OSD_REG_RECT,
                    .rect.p0.x = LUX_OSD_RECT_GRP1_START_X,
                    .rect.p0.y = LUX_OSD_RECT_GRP1_START_Y,
                    .rect.p1.x = LUX_OSD_RECT_GRP1_START_X + 200 - 1,
                    .rect.p1.y = LUX_OSD_RECT_GRP1_START_Y + 100 - 1,
                    .fmt = PIX_FMT_MONOWHITE,
                    .data.lineRectData.color = OSD_GREEN,
                    .data.lineRectData.linewidth = 0,
                },
            },
            .OSDGrpRgnAttr =
            {
                .show = 0,
                .layer = 1,
                .scalex = 1,
                .scaley = 1,
            },
        },
};

//越界点值转换
VOID_X LUX_OutBoundPoint_Convert(INT_X channel, IMPPoint *pPoint)
{
    if (pPoint == NULL)
    {
        IMP_LOG_ERR(TAG, "LUX_OutBoundPoint_Convert failed, empty point\n");
        return;
    }
    if (pPoint->x < 0) pPoint->x = 0;
    if (pPoint->y < 0) pPoint->y = 0;
    INT_X ret = LUX_ERR, width = 0, height = 0;
    ret = LUX_Video_Fnuc_Get_Resoulution(channel, &width, &height);
    if (ret < 0)
    {
        IMP_LOG_ERR(TAG, "Call func_video_get_resolution fail! channel[%d].\n", channel);
    }
    if (pPoint->x >= width) pPoint->x = width - 1;
    if (pPoint->y >= height) pPoint->y = height - 1;
}

VOID_X LUX_OSD_Scale_Get(DOUBLE_X *scale_w, DOUBLE_X *scale_h)
{
    INT_X ret = LUX_ERR, main_res_w = 0, main_res_h = 0, sub_res_w = 0, sub_res_h = 0;
    ret = LUX_Video_Fnuc_Get_Resoulution(LUX_STREAM_MAIN, &main_res_w, &main_res_h);
    if (ret < 0)
    {
        IMP_LOG_ERR(TAG, "Call LUX_Video_Fnuc_Get_Resoulution fail! channel[%d]\n", LUX_STREAM_MAIN);
    }
    ret = LUX_Video_Fnuc_Get_Resoulution(LUX_STREAM_SUB, &sub_res_w, &sub_res_h);
    if (ret < 0)
    {
        IMP_LOG_ERR(TAG, "Call LUX_Video_Fnuc_Get_Resoulution fail! channel[%d]\n", LUX_STREAM_SUB);
    }
    *scale_w = main_res_w * 1.0 / sub_res_w;
    *scale_h = main_res_h * 1.0 / sub_res_h;
    // printf("main_res_w=%d, main_res_h=%d, sub_res_h=%d, main_res_h=%d, scale_w=%f, scale_h=%f\n", main_res_w, main_res_h, sub_res_w, sub_res_h, *scale_w, *scale_h);
}

/*人脸*/
VOID_X LUX_OSD_FaceRect_Show(facedet_param_output_t* ivsFaceDetRet)
{
    if (ivsFaceDetRet == NULL)
    {
        IMP_LOG_ERR(TAG, "LUX_OSD_FaceRect_Show failed, empty point\n");
        return;
    }
    INT_X i, j;
    INT_X ret = LUX_ERR;
    INT_X pos = LUX_FACE_START_POS;
    INT_X faceCnt = 0;
    
    DOUBLE_X scale_w = 0, scale_h = 0;
    LUX_OSD_Scale_Get(&scale_w, &scale_h);
    if (ivsFaceDetRet->count > LUX_FACTRECT_NUM_MAX)
    {
        ivsFaceDetRet->count = LUX_FACTRECT_NUM_MAX;
    }

    for (i = 0; i < LUX_OSD_GRP_NUM; i++)
    {
        if (i == LUX_STREAM_SUB)
        {
            scale_w = 1;
            scale_h = 1;
        }

        for (j = pos; j < pos + ivsFaceDetRet->count; j++)
        {
            /* 设置OSD区域属性 */
            faceCnt = j - pos;
            if(ivsFaceDetRet->face[faceCnt].class_id == 0)// && ivsFaceDetRet->face[faceCnt].confidence > FACE_DET_CONFIDENCE)
            {
                // printf("face[%d] confidence=[%lf] \n", faceCnt, ivsFaceDetRet->face[faceCnt].confidence);
                g_osdAttr[j].OSDRgnAttr[i].rect.p0.x = ivsFaceDetRet->face[faceCnt].show_box.ul.x * scale_w;
                g_osdAttr[j].OSDRgnAttr[i].rect.p0.y = ivsFaceDetRet->face[faceCnt].show_box.ul.y * scale_h;
                g_osdAttr[j].OSDRgnAttr[i].rect.p1.x = ivsFaceDetRet->face[faceCnt].show_box.br.x * scale_w;
                g_osdAttr[j].OSDRgnAttr[i].rect.p1.y = ivsFaceDetRet->face[faceCnt].show_box.br.y * scale_h;
                LUX_OutBoundPoint_Convert(i, &g_osdAttr[j].OSDRgnAttr[i].rect.p0);
                LUX_OutBoundPoint_Convert(i, &g_osdAttr[j].OSDRgnAttr[i].rect.p1);
                // printf("FaceDet osdGrp[%d] objrect[%d] coordinate value: [%d, %d, %d, %d] \n", i, faceCnt, g_osdAttr[j].OSDRgnAttr[i].rect.p0.x, g_osdAttr[j].OSDRgnAttr[i].rect.p0.y, g_osdAttr[j].OSDRgnAttr[i].rect.p1.x, g_osdAttr[j].OSDRgnAttr[i].rect.p1.y);
                // g_osdAttr[j].OSDRgnAttr[i].data.lineRectData.linewidth = scale;
                ret = IMP_OSD_SetRgnAttr(g_prHander[i][j], &g_osdAttr[j].OSDRgnAttr[i]);
                if (ret != LUX_OK)
                {
                    IMP_LOG_ERR(TAG, "IMP_OSD_SetRgnAttr FaceRect error !\n");
                    return;
                }
                ret = IMP_OSD_ShowRgn(g_prHander[i][j], i, LUX_TRUE);
                if (ret != LUX_OK)
                {
                    IMP_LOG_ERR(TAG, "IMP_OSD_ShowRgn() group%d region%d error\n", i, j);
                    return;
                }
            }
        }
    }
    // printf("\n\n\n");
}

VOID_X LUX_OSD_FaceRect_Miss()
{
    INT_X ret = LUX_ERR;
    INT_X i, j;
    for (i = 0; i < LUX_OSD_GRP_NUM; i++)
    {
        for (j = LUX_FACE_START_POS; j < LUX_FACE_END_POS; j++)
        {
            ret = IMP_OSD_ShowRgn(g_prHander[i][j], i, LUX_FALSE);
            if (ret != LUX_OK)
            {
                IMP_LOG_ERR(TAG, "IMP_OSD_ShowRgn() group%d region%d error\n", i, j);
                return;
            }
        }
    }
}

/*人形移动*/
VOID_X LUX_OSD_PersonRect_Show(persondet_param_output_t* ivsPersonDetRet)
{
    if (ivsPersonDetRet == NULL)
    {
        IMP_LOG_ERR(TAG, "LUX_OSD_PersonRect_Show failed, empty point\n");
        return;
    }
    INT_X ret;
    INT_X i, j;
    INT_X pos = LUX_HUMAN_START_POS;
    INT_X personCnt = 0;
    DOUBLE_X scale_w = 0, scale_h = 0;
    LUX_OSD_Scale_Get(&scale_w, &scale_h);
    if (ivsPersonDetRet->count > LUX_HUMANRECT_NUM_MAX)
    {
        ivsPersonDetRet->count = LUX_HUMANRECT_NUM_MAX;
    }

    for (i = 0; i < LUX_OSD_GRP_NUM; i++)
    {
        if (i == LUX_STREAM_SUB)
        {
            scale_w = 1;
            scale_h = 1;
        }
        for (j = pos; j < pos + ivsPersonDetRet->count; j++)
        {
            /* 设置OSD区域属性 */
            personCnt = j - pos;
            // printf("person[%d] confidence=[%lf]\n", personCnt, ivsPersonDetRet->person[personCnt].confidence);
            // if (ivsPersonDetRet->person[personCnt].confidence > PERSON_DET_CONFIDENCE)
            // {
                g_osdAttr[j].OSDRgnAttr[i].rect.p0.x = ivsPersonDetRet->person[personCnt].show_box.ul.x * scale_w;
                g_osdAttr[j].OSDRgnAttr[i].rect.p0.y = ivsPersonDetRet->person[personCnt].show_box.ul.y * scale_h;
                g_osdAttr[j].OSDRgnAttr[i].rect.p1.x = ivsPersonDetRet->person[personCnt].show_box.br.x * scale_w;
                g_osdAttr[j].OSDRgnAttr[i].rect.p1.y = ivsPersonDetRet->person[personCnt].show_box.br.y * scale_h;
                LUX_OutBoundPoint_Convert(i, &g_osdAttr[j].OSDRgnAttr[i].rect.p0);
                LUX_OutBoundPoint_Convert(i, &g_osdAttr[j].OSDRgnAttr[i].rect.p1);
                // printf("PersonDet osdGrp[%d] objrect[%d] coordinate value: [%d, %d, %d, %d] \n", i, personCnt, g_osdAttr[j].OSDRgnAttr[i].rect.p0.x, g_osdAttr[j].OSDRgnAttr[i].rect.p0.y, g_osdAttr[j].OSDRgnAttr[i].rect.p1.x, g_osdAttr[j].OSDRgnAttr[i].rect.p1.y);
                g_osdAttr[j].OSDRgnAttr[i].data.lineRectData.color = OSD_RED;
                // g_osdAttr[j].OSDRgnAttr[i].data.lineRectData.linewidth = scale;
                ret = IMP_OSD_SetRgnAttr(g_prHander[i][j], &g_osdAttr[j].OSDRgnAttr[i]);
                if (ret != LUX_OK)
                {
                    IMP_LOG_ERR(TAG, "IMP_OSD_SetRgnAttr PersonRect error !\n");
                    return;
                }
                ret = IMP_OSD_ShowRgn(g_prHander[i][j], i, LUX_TRUE);
                if (ret != LUX_OK)
                {
                    IMP_LOG_ERR(TAG, "IMP_OSD_ShowRgn() group%d region%d error\n", i, j);
                    return;
                }
            // }
        }
    }
    // printf("\n\n\n");
}

VOID_X LUX_OSD_PersonRect_Miss()
{
    INT_X ret = LUX_ERR;
    INT_X i, j;
    for (i = 0; i < LUX_OSD_GRP_NUM; i++)
    {
        for (j = LUX_HUMAN_START_POS; j < LUX_HUMAN_END_POS; j++)
        {
            ret = IMP_OSD_ShowRgn(g_prHander[i][j], i, LUX_FALSE);
            if (ret != LUX_OK)
            {
                IMP_LOG_ERR(TAG, "IMP_OSD_ShowRgn() group%d region%d error\n", i, j);
                return;
            }
        }
    }
}

/*周界越界目标*/
VOID_X LUX_OSD_PermOBJ_Rect_Show(perm_param_output_t* ivsPermDetRet)
{
    if (ivsPermDetRet == NULL)
    {
        IMP_LOG_ERR(TAG, "LUX_OSD_PermOBJ_Rect_Show failed, empty point\n");
        return;
    }
    INT_X ret;
    INT_X i, j;
    INT_X pos = LUX_PERM_OBJ_START_POS;
    INT_X objCnt = 0;
    DOUBLE_X scale_w = 0, scale_h = 0;
    LUX_OSD_Scale_Get(&scale_w, &scale_h);
    if (ivsPermDetRet->rectcnt > LUX_PERM_OBJ_NUM_MAX)
    {
        ivsPermDetRet->rectcnt = LUX_PERM_OBJ_NUM_MAX;
    }

    for (i = 0; i < LUX_OSD_GRP_NUM; i++)
    {
        if (i == LUX_STREAM_SUB)
        {
            scale_w = 1;
            scale_h = 1;
        }
        for (j = pos; j < pos + ivsPermDetRet->rectcnt; j++)
        {
            /* 设置OSD区域属性 */
            objCnt = j - pos;
            g_osdAttr[j].OSDRgnAttr[i].rect.p0.x = ivsPermDetRet->rects[objCnt].ul.x * scale_w;
            g_osdAttr[j].OSDRgnAttr[i].rect.p0.y = ivsPermDetRet->rects[objCnt].ul.y * scale_h;
            g_osdAttr[j].OSDRgnAttr[i].rect.p1.x = ivsPermDetRet->rects[objCnt].br.x * scale_w;
            g_osdAttr[j].OSDRgnAttr[i].rect.p1.y = ivsPermDetRet->rects[objCnt].br.y * scale_h;
            LUX_OutBoundPoint_Convert(i, &g_osdAttr[j].OSDRgnAttr[i].rect.p0);
            LUX_OutBoundPoint_Convert(i, &g_osdAttr[j].OSDRgnAttr[i].rect.p1);
            // printf("PermDet osdGrp[%d] objectRect[%d] coordinate value: [%d, %d, %d, %d] \n", i, objCnt, g_osdAttr[j].OSDRgnAttr[i].rect.p0.x, g_osdAttr[j].OSDRgnAttr[i].rect.p0.y, g_osdAttr[j].OSDRgnAttr[i].rect.p1.x, g_osdAttr[j].OSDRgnAttr[i].rect.p1.y);
            g_osdAttr[j].OSDRgnAttr[i].data.lineRectData.color = OSD_YELLOW;
            // g_osdAttr[j].OSDRgnAttr[i].data.lineRectData.linewidth = scale;
            ret = IMP_OSD_SetRgnAttr(g_prHander[i][j], &g_osdAttr[j].OSDRgnAttr[i]);
            if (ret != LUX_OK)
            {
                IMP_LOG_ERR(TAG, "IMP_OSD_SetRgnAttr Intrusion Obj Rect error !\n");
                return;
            }
            ret = IMP_OSD_ShowRgn(g_prHander[i][j], i, LUX_TRUE);
            if (ret != LUX_OK)
            {
                IMP_LOG_ERR(TAG, "IMP_OSD_ShowRgn() group%d region%d error\n", i, j);
                return;
            }
        }
    }
}

VOID_X LUX_OSD_PermOBJ_Rect_Miss()
{
    INT_X ret = LUX_ERR;
    INT_X i, j;
    for (i = 0; i < LUX_OSD_GRP_NUM; i++)
    {
        for (j = LUX_PERM_OBJ_START_POS; j < LUX_PERM_OBJ_END_POS; j++)
        {
            ret = IMP_OSD_ShowRgn(g_prHander[i][j], i, LUX_FALSE);
            if (ret != LUX_OK)
            {
                IMP_LOG_ERR(TAG, "IMP_OSD_ShowRgn() group%d region%d error\n", i, j);
                return;
            }
        }
    }
}

/*物品遗留报警框显示*/
VOID_X LUX_OSD_UMDetArea_Rect_Show(P_LUX_UMDET_OBJ_ST ivsUMArea, BOOL_X bShow)
{
    if (ivsUMArea == NULL)
    {
        IMP_LOG_ERR(TAG, "LUX_OSD_UMDetArea_Rect_Show failed, empty point\n");
        return;
    }
    INT_X ret;
    INT_X i, j;
    INT_X pos = LUX_UM_AREA_START_POS;
    INT_X objCnt = 0;
    DOUBLE_X scale_w = 0, scale_h = 0;
    LUX_OSD_Scale_Get(&scale_w, &scale_h);
    if (ivsUMArea->rectcnt > LUX_UM_AREA_NUM_MAX)
    {
        ivsUMArea->rectcnt = LUX_UM_AREA_NUM_MAX;
    }

    for (i = 0; i < LUX_OSD_GRP_NUM; i++)
    {
        if (i == LUX_STREAM_SUB)
        {
            scale_w = 1;
            scale_h = 1;
        }

        for (j = pos; j < pos + ivsUMArea->rectcnt; j++)
        {
            /* 设置OSD区域属性 */
            objCnt = j - pos;
            g_osdAttr[j].OSDRgnAttr[i].rect.p0.x = ivsUMArea->rects[objCnt].ul.x * scale_w;
            g_osdAttr[j].OSDRgnAttr[i].rect.p0.y = ivsUMArea->rects[objCnt].ul.y * scale_h;
            g_osdAttr[j].OSDRgnAttr[i].rect.p1.x = ivsUMArea->rects[objCnt].br.x * scale_w;
            g_osdAttr[j].OSDRgnAttr[i].rect.p1.y = ivsUMArea->rects[objCnt].br.y * scale_h;
            LUX_OutBoundPoint_Convert(i, &g_osdAttr[j].OSDRgnAttr[i].rect.p0);
            LUX_OutBoundPoint_Convert(i, &g_osdAttr[j].OSDRgnAttr[i].rect.p1);
            // printf("UMDet osdGrp[%d] objrect[%d] coordinate value: [%d, %d, %d, %d] \n", i, objCnt, g_osdAttr[j].OSDRgnAttr[i].rect.p0.x, g_osdAttr[j].OSDRgnAttr[i].rect.p0.y, g_osdAttr[j].OSDRgnAttr[i].rect.p1.x, g_osdAttr[j].OSDRgnAttr[i].rect.p1.y);
            g_osdAttr[j].OSDRgnAttr[i].data.lineRectData.color = OSD_RED;
            g_osdAttr[j].OSDRgnAttr[i].data.lineRectData.linewidth = 3;
            ret = IMP_OSD_SetRgnAttr(g_prHander[i][j], &g_osdAttr[j].OSDRgnAttr[i]);
            if (ret != LUX_OK)
            {
                IMP_LOG_ERR(TAG, "IMP_OSD_SetRgnAttr UMDet Obj Rect error !\n");
                return;
            }
            ret = IMP_OSD_ShowRgn(g_prHander[i][j], i, bShow);
            if (ret != LUX_OK)
            {
                IMP_LOG_ERR(TAG, "IMP_OSD_ShowRgn() group%d region%d error\n", i, j);
                return;
            }
        }
    }
}

/*物品遗留目标*/
VOID_X LUX_OSD_UMDetOBJLeft_Rect_Show(P_LUX_UMDET_OBJ_ST ivsUMDetRet)
{
    if (ivsUMDetRet == NULL)
    {
        IMP_LOG_ERR(TAG, "LUX_OSD_UMDetOBJLeft_Rect_Show failed, empty point\n");
        return;
    }
    INT_X ret;
    INT_X i, j;
    INT_X pos = LUX_UM_OBJLEFT_START_POS;
    INT_X objCnt = 0;
    DOUBLE_X scale_w = 0, scale_h = 0;
    LUX_OSD_Scale_Get(&scale_w, &scale_h);
    if (ivsUMDetRet->rectcnt > LUX_UM_OBJLEFT_NUM_MAX)
    {
        ivsUMDetRet->rectcnt = LUX_UM_OBJLEFT_NUM_MAX;
    }

    for (i = 0; i < LUX_OSD_GRP_NUM; i++)
    {
        if (i == LUX_STREAM_SUB)
        {
            scale_w = 1;
            scale_h = 1;
        }

        for (j = pos; j < pos + ivsUMDetRet->rectcnt; j++)
        {
            /* 设置OSD区域属性 */
            objCnt = j - pos;
            g_osdAttr[j].OSDRgnAttr[i].rect.p0.x = ivsUMDetRet->rects[objCnt].ul.x * scale_w;
            g_osdAttr[j].OSDRgnAttr[i].rect.p0.y = ivsUMDetRet->rects[objCnt].ul.y * scale_h;
            g_osdAttr[j].OSDRgnAttr[i].rect.p1.x = ivsUMDetRet->rects[objCnt].br.x * scale_w;
            g_osdAttr[j].OSDRgnAttr[i].rect.p1.y = ivsUMDetRet->rects[objCnt].br.y * scale_h;
            LUX_OutBoundPoint_Convert(i, &g_osdAttr[j].OSDRgnAttr[i].rect.p0);
            LUX_OutBoundPoint_Convert(i, &g_osdAttr[j].OSDRgnAttr[i].rect.p1);
            // printf("UMDet osdGrp[%d] objrect[%d] coordinate value: [%d, %d, %d, %d] \n", i, objCnt, g_osdAttr[j].OSDRgnAttr[i].rect.p0.x, g_osdAttr[j].OSDRgnAttr[i].rect.p0.y, g_osdAttr[j].OSDRgnAttr[i].rect.p1.x, g_osdAttr[j].OSDRgnAttr[i].rect.p1.y);
            g_osdAttr[j].OSDRgnAttr[i].data.lineRectData.color = OSD_YELLOW;
            g_osdAttr[j].OSDRgnAttr[i].data.lineRectData.linewidth = 2;
            ret = IMP_OSD_SetRgnAttr(g_prHander[i][j], &g_osdAttr[j].OSDRgnAttr[i]);
            if (ret != LUX_OK)
            {
                IMP_LOG_ERR(TAG, "IMP_OSD_SetRgnAttr UMDet Obj Rect error !\n");
                return;
            }
            ret = IMP_OSD_ShowRgn(g_prHander[i][j], i, LUX_TRUE);
            if (ret != LUX_OK)
            {
                IMP_LOG_ERR(TAG, "IMP_OSD_ShowRgn() group%d region%d error\n", i, j);
                return;
            }
        }
    }
}

VOID_X LUX_OSD_UMDetOBJLeft_Rect_Miss()
{
    INT_X ret = LUX_ERR;
    INT_X i, j;
    for (i = 0; i < LUX_OSD_GRP_NUM; i++)
    {
        for (j = LUX_UM_OBJLEFT_START_POS; j < LUX_UM_OBJLEFT_END_POS; j++)
        {
            ret = IMP_OSD_ShowRgn(g_prHander[i][j], i, LUX_FALSE);
            if (ret != LUX_OK)
            {
                IMP_LOG_ERR(TAG, "IMP_OSD_ShowRgn() group%d region%d error\n", i, j);
                return;
            }
        }
    }
}

/*物品丢失目标*/
VOID_X LUX_OSD_UMDetOBJLoss_Rect_Show(P_LUX_UMDET_OBJ_ST ivsUMLoss, BOOL_X bShow)
{
    if (ivsUMLoss == NULL)
    {
        IMP_LOG_ERR(TAG, "LUX_OSD_UMDetOBJLoss_Rect_Show failed, empty point\n");
        return;
    }
    INT_X ret;
    INT_X i, j;
    INT_X pos = LUX_UM_OBJLOSS_START_POS;
    INT_X objCnt = 0;
    DOUBLE_X scale_w = 0, scale_h = 0;
    LUX_OSD_Scale_Get(&scale_w, &scale_h);
    if (ivsUMLoss->rectcnt > LUX_UM_OBJLOSS_NUM_MAX)
    {
        ivsUMLoss->rectcnt = LUX_UM_OBJLOSS_NUM_MAX;
    }

    for (i = 0; i < LUX_OSD_GRP_NUM; i++)
    {
        if (i == LUX_STREAM_SUB)
        {
            scale_w = 1;
            scale_h = 1;
        }

        for (j = pos; j < pos + ivsUMLoss->rectcnt; j++)
        {
            /* 设置OSD区域属性 */
            objCnt = j - pos;
            g_osdAttr[j].OSDRgnAttr[i].rect.p0.x = ivsUMLoss->rects[objCnt].ul.x * scale_w;
            g_osdAttr[j].OSDRgnAttr[i].rect.p0.y = ivsUMLoss->rects[objCnt].ul.y * scale_h;
            g_osdAttr[j].OSDRgnAttr[i].rect.p1.x = ivsUMLoss->rects[objCnt].br.x * scale_w;
            g_osdAttr[j].OSDRgnAttr[i].rect.p1.y = ivsUMLoss->rects[objCnt].br.y * scale_h;
            LUX_OutBoundPoint_Convert(i, &g_osdAttr[j].OSDRgnAttr[i].rect.p0);
            LUX_OutBoundPoint_Convert(i, &g_osdAttr[j].OSDRgnAttr[i].rect.p1);
            // printf("UMDet osdGrp[%d] objrect[%d] coordinate value: [%d, %d, %d, %d] \n", i, objCnt, g_osdAttr[j].OSDRgnAttr[i].rect.p0.x, g_osdAttr[j].OSDRgnAttr[i].rect.p0.y, g_osdAttr[j].OSDRgnAttr[i].rect.p1.x, g_osdAttr[j].OSDRgnAttr[i].rect.p1.y);
            g_osdAttr[j].OSDRgnAttr[i].data.lineRectData.color = OSD_GREEN;
            g_osdAttr[j].OSDRgnAttr[i].data.lineRectData.linewidth = 2;
            ret = IMP_OSD_SetRgnAttr(g_prHander[i][j], &g_osdAttr[j].OSDRgnAttr[i]);
            if (ret != LUX_OK)
            {
                IMP_LOG_ERR(TAG, "IMP_OSD_SetRgnAttr UMDet Obj Rect error !\n");
                return;
            }
            ret = IMP_OSD_ShowRgn(g_prHander[i][j], i, bShow);
            if (ret != LUX_OK)
            {
                IMP_LOG_ERR(TAG, "IMP_OSD_ShowRgn() group%d region%d error\n", i, j);
                return;
            }
        }
    }
}

VOID_X LUX_OSD_UMDetOBJLoss_Rect_ChangeColor(P_LUX_UMDET_OBJ_ST ivsUMDetRet, IMPOsdColour color)
{
    if (ivsUMDetRet == NULL)
    {
        IMP_LOG_ERR(TAG, "LUX_OSD_UMDetOBJLoss_Rect_Show failed, empty point\n");
        return;
    }
    INT_X ret;
    INT_X i, j;
    INT_X pos = LUX_UM_OBJLOSS_START_POS;
    INT_X objCnt = 0;
    DOUBLE_X scale_w = 0, scale_h = 0;
    LUX_OSD_Scale_Get(&scale_w, &scale_h);
    if (ivsUMDetRet->rectcnt > LUX_UM_OBJLOSS_NUM_MAX)
    {
        ivsUMDetRet->rectcnt = LUX_UM_OBJLOSS_NUM_MAX;
    }

    for (i = 0; i < LUX_OSD_GRP_NUM; i++)
    {
        if (i == LUX_STREAM_SUB)
        {
            scale_w = 1;
            scale_h = 1;
        }

        for (j = pos; j < pos + ivsUMDetRet->rectcnt; j++)
        {
            /* 设置OSD区域属性 */
            objCnt = j - pos;
            g_osdAttr[j].OSDRgnAttr[i].data.lineRectData.color = color;
            // printf("++++ OSD osdGrp[%d] objrect[%d] coordinate value: [%d, %d, %d, %d] ++++\n", j, objCnt, g_osdAttr[j].OSDRgnAttr[i].rect.p0.x, g_osdAttr[j].OSDRgnAttr[i].rect.p0.y, g_osdAttr[j].OSDRgnAttr[i].rect.p1.x, g_osdAttr[j].OSDRgnAttr[i].rect.p1.y);
            // printf("++++ UMDet osdGrp[%d] objrect[%d] coordinate value: [%d, %d, %d, %d] ++++\n", j, objCnt, (INT_X)ivsUMDetRet->rects[objCnt].ul.x * scale_w, (INT_X)ivsUMDetRet->rects[objCnt].ul.y * scale_h, (INT_X)ivsUMDetRet->rects[objCnt].br.x * scale_w, (INT_X)ivsUMDetRet->rects[objCnt].br.y * scale_h);
            ret = IMP_OSD_SetRgnAttr(g_prHander[i][j], &g_osdAttr[j].OSDRgnAttr[i]);
            if (ret != LUX_OK)
            {
                IMP_LOG_ERR(TAG, "IMP_OSD_SetRgnAttr UMDet Obj Rect error !\n");
                return;
            }
            ret = IMP_OSD_ShowRgn(g_prHander[i][j], i, LUX_TRUE);
            if (ret != LUX_OK)
            {
                IMP_LOG_ERR(TAG, "IMP_OSD_ShowRgn() group%d region%d error\n", i, j);
                return;
            }
        }
    }
}



/*徘徊*/
VOID_X LUX_OSD_LoiterRect_Show(persondet_param_output_t* LioterDetRet)
{
    if (LioterDetRet == NULL)
    {
        IMP_LOG_ERR(TAG, "LUX_OSD_LoiterRect_Show failed, empty point\n");
        return;
    }
    INT_X ret;
    INT_X i, j;
    INT_X pos = LUX_LOITER_START_POS;
    INT_X lioterCnt = 0;
    DOUBLE_X scale_w = 0, scale_h = 0;
    LUX_OSD_Scale_Get(&scale_w, &scale_h);
    if (LioterDetRet->count > LUX_LOITER_NUM_MAX)
    {
        LioterDetRet->count = LUX_LOITER_NUM_MAX;
    }

    for (i = 0; i < LUX_OSD_GRP_NUM; i++)
    {
        if (i == LUX_STREAM_SUB)
        {
            scale_w = 1;
            scale_h = 1;
        }
        for (j = pos; j < pos + LioterDetRet->count; j++)
        {
            /* 设置OSD区域属性 */
            lioterCnt = j - pos;
            // printf("person[%d] confidence=[%lf] \n", lioterCnt, LioterDetRet->person[lioterCnt].confidence);
            // if (LioterDetRet->person[lioterCnt].confidence > PERSON_DET_CONFIDENCE)
            // {
                g_osdAttr[j].OSDRgnAttr[i].rect.p0.x = LioterDetRet->person[lioterCnt].show_box.ul.x * scale_w;
                g_osdAttr[j].OSDRgnAttr[i].rect.p0.y = LioterDetRet->person[lioterCnt].show_box.ul.y * scale_h;
                g_osdAttr[j].OSDRgnAttr[i].rect.p1.x = LioterDetRet->person[lioterCnt].show_box.br.x * scale_w;
                g_osdAttr[j].OSDRgnAttr[i].rect.p1.y = LioterDetRet->person[lioterCnt].show_box.br.y * scale_h;
                LUX_OutBoundPoint_Convert(i, &g_osdAttr[j].OSDRgnAttr[i].rect.p0);
                LUX_OutBoundPoint_Convert(i, &g_osdAttr[j].OSDRgnAttr[i].rect.p1);
                // printf("LoiterDet osdGrp[%d] objrect[%d] coordinate value: [%d, %d, %d, %d] \n", i, lioterCnt, g_osdAttr[j].OSDRgnAttr[i].rect.p0.x, g_osdAttr[j].OSDRgnAttr[i].rect.p0.y, g_osdAttr[j].OSDRgnAttr[i].rect.p1.x, g_osdAttr[j].OSDRgnAttr[i].rect.p1.y);
                g_osdAttr[j].OSDRgnAttr[i].data.lineRectData.color = OSD_RED;
                g_osdAttr[j].OSDRgnAttr[i].data.lineRectData.linewidth = 3;
                ret = IMP_OSD_SetRgnAttr(g_prHander[i][j], &g_osdAttr[j].OSDRgnAttr[i]);
                if (ret != LUX_OK)
                {
                    IMP_LOG_ERR(TAG, "IMP_OSD_SetRgnAttr LoiterRect error !\n");
                    return;
                }
                ret = IMP_OSD_ShowRgn(g_prHander[i][j], i, LUX_TRUE);
                if (ret != LUX_OK)
                {
                    IMP_LOG_ERR(TAG, "IMP_OSD_ShowRgn() group%d region%d error\n", i, j);
                    return;
                }
            // }
        }
    }
}

VOID_X LUX_OSD_LoiterRect_Miss()
{
    INT_X ret = LUX_ERR;
    INT_X i, j;
    for (i = 0; i < LUX_OSD_GRP_NUM; i++)
    {
        for (j = LUX_LOITER_START_POS; j < LUX_LOITER_END_POS; j++)
        {
            ret = IMP_OSD_ShowRgn(g_prHander[i][j], i, LUX_FALSE);
            if (ret != LUX_OK)
            {
                IMP_LOG_ERR(TAG, "IMP_OSD_ShowRgn() group%d region%d error\n", i, j);
                return;
            }
        }
    }
}

/*越线目标*/
VOID_X LUX_OSD_CrossLine_OBJ_Show(P_LUX_CROSSLINEDET_OBJ_ST CLDetRet)
{
    if (CLDetRet == NULL)
    {
        IMP_LOG_ERR(TAG, "LUX_OSD_CrossLine_OBJ_Show failed, empty point\n");
        return;
    }
    INT_X ret;
    INT_X i, j;
    INT_X pos = LUX_CROSSLINE_OBJ_START_POS;
    INT_X ClObjCnt = 0;
    DOUBLE_X scale_w = 0, scale_h = 0;
    LUX_OSD_Scale_Get(&scale_w, &scale_h);
    if (CLDetRet->boxcnt > LUX_LINECROSS_OBJ_MAX)
    {
        CLDetRet->boxcnt = LUX_LINECROSS_OBJ_MAX;
    }

    for (i = 0; i < LUX_OSD_GRP_NUM; i++)
    {
        if (i == LUX_STREAM_SUB)
        {
            scale_w = 1;
            scale_h = 1;
        }
        for (j = pos; j < pos + CLDetRet->boxcnt; j++)
        {
            /* 设置OSD区域属性 */
            ClObjCnt = j - pos;
            g_osdAttr[j].OSDRgnAttr[i].rect.p0.x = CLDetRet->box[ClObjCnt].ul.x * scale_w;
            g_osdAttr[j].OSDRgnAttr[i].rect.p0.y = CLDetRet->box[ClObjCnt].ul.y * scale_h;
            g_osdAttr[j].OSDRgnAttr[i].rect.p1.x = CLDetRet->box[ClObjCnt].br.x * scale_w;
            g_osdAttr[j].OSDRgnAttr[i].rect.p1.y = CLDetRet->box[ClObjCnt].br.y * scale_h;
            LUX_OutBoundPoint_Convert(i, &g_osdAttr[j].OSDRgnAttr[i].rect.p0);
            LUX_OutBoundPoint_Convert(i, &g_osdAttr[j].OSDRgnAttr[i].rect.p1);
            printf("CrossLineDet osdGrp[%d] objrect[%d] coordinate value: [%d, %d, %d, %d] \n", i, ClObjCnt, g_osdAttr[j].OSDRgnAttr[i].rect.p0.x, g_osdAttr[j].OSDRgnAttr[i].rect.p0.y, g_osdAttr[j].OSDRgnAttr[i].rect.p1.x, g_osdAttr[j].OSDRgnAttr[i].rect.p1.y);
            g_osdAttr[j].OSDRgnAttr[i].data.lineRectData.color = OSD_GREEN;
            g_osdAttr[j].OSDRgnAttr[i].data.lineRectData.linewidth = 3;
            ret = IMP_OSD_SetRgnAttr(g_prHander[i][j], &g_osdAttr[j].OSDRgnAttr[i]);
            if (ret != LUX_OK)
            {
                IMP_LOG_ERR(TAG, "IMP_OSD_SetRgnAttr LoiterRect error !\n");
                return;
            }
            ret = IMP_OSD_ShowRgn(g_prHander[i][j], i, LUX_TRUE);
            if (ret != LUX_OK)
            {
                IMP_LOG_ERR(TAG, "IMP_OSD_ShowRgn() group%d region%d error\n", i, j);
                return;
            }
        }
    }
}

VOID_X LUX_OSD_CrossLine_OBJ_Miss()
{
    INT_X ret = LUX_ERR;
    INT_X i, j;
    for (i = 0; i < LUX_OSD_GRP_NUM; i++)
    {
        for (j = LUX_CROSSLINE_OBJ_START_POS; j < LUX_CROSSLINE_OBJ_END_POS; j++)
        {
            ret = IMP_OSD_ShowRgn(g_prHander[i][j], i, LUX_FALSE);
            if (ret != LUX_OK)
            {
                IMP_LOG_ERR(TAG, "IMP_OSD_ShowRgn() group%d region%d error\n", i, j);
                return;
            }
        }
    }
}

/*osd 多边形周界*/
VOID_X LUX_OSD_PermSlash_Show(LUX_OSD_SLASHLINE_ST* pLineInfo, BOOL_X bShow)
{
    INT_X i, ret = LUX_ERR;
    DOUBLE_X scale_w = 0, scale_h = 0;
    LUX_OSD_Scale_Get(&scale_w, &scale_h);
    INT_X LinePos = LUX_PERMSLASH_START_POS + pLineInfo->lineNum;
    if (LinePos > LUX_PERMSLASH_END_POS)
    {
        LinePos = LUX_PERMSLASH_END_POS;
    }
    for (i = 0; i < LUX_OSD_GRP_NUM; i++)
    {
        if (i == LUX_STREAM_SUB)
        {
            scale_w = 1;
            scale_h = 1;
        }
        g_osdAttr[LinePos].OSDRgnAttr[i].type = OSD_REG_SLASH;
        g_osdAttr[LinePos].OSDRgnAttr[i].data.lineRectData.color = OSD_IPU_BLUE;
        g_osdAttr[LinePos].OSDRgnAttr[i].data.lineRectData.linewidth = 5;
        g_osdAttr[LinePos].OSDRgnAttr[i].rect.p0.x = pLineInfo->line.p0.x * scale_w;
        g_osdAttr[LinePos].OSDRgnAttr[i].rect.p0.y = pLineInfo->line.p0.y * scale_h;
        g_osdAttr[LinePos].OSDRgnAttr[i].rect.p1.x = pLineInfo->line.p1.x * scale_w;
        g_osdAttr[LinePos].OSDRgnAttr[i].rect.p1.y = pLineInfo->line.p1.y * scale_h;
        LUX_OutBoundPoint_Convert(0, &g_osdAttr[LinePos].OSDRgnAttr[i].rect.p0);
        LUX_OutBoundPoint_Convert(0, &g_osdAttr[LinePos].OSDRgnAttr[i].rect.p1);

        ret = IMP_OSD_SetRgnAttr(g_prHander[i][LinePos], &g_osdAttr[LinePos].OSDRgnAttr[i]);
        if (ret != LUX_OK)
        {
            IMP_LOG_ERR(TAG, "IMP_OSD_SetRgnAttr Intrusion Obj Rect error !\n");
            return;
        }

        ret = IMP_OSD_ShowRgn(g_prHander[i][LinePos], i, bShow);
        if (ret != LUX_OK)
        {
            IMP_LOG_ERR(TAG, "IMP_OSD_ShowRgn() group%d region%d error\n");
            return;
        }
        printf("channel[%d]: startPoint(x:%d, y:%d), endPoint(x:%d, y:%d) \n", i,
                g_osdAttr[LinePos].OSDRgnAttr[i].rect.p0.x,
                g_osdAttr[LinePos].OSDRgnAttr[i].rect.p0.y,
                g_osdAttr[LinePos].OSDRgnAttr[i].rect.p1.x,
                g_osdAttr[LinePos].OSDRgnAttr[i].rect.p1.y);
    }
}

VOID_X LUX_OSD_PermPolygon_Show(BOOL_X bShow)
{
    INT_X i, j, lineCnt = 0, ret = LUX_ERR;
    LUX_OSD_SLASHLINE_ST LineInfo;
    for (i = 0; i < g_permdet_attr.in_param.permcnt; i++)
    {
        for (j = 0; j < g_permdet_attr.in_param.perms[i].pcnt; j++)
        {
            printf("region[%d], [%d]th Slash\n", i, j);
            memset(&LineInfo, 0, sizeof(LUX_OSD_SLASHLINE_ST));
            LineInfo.line.p0 = *(g_permdet_attr.in_param.perms[i].p + j);
            if (j != g_permdet_attr.in_param.perms[i].pcnt - 1)
            {
                LineInfo.line.p1 = *(g_permdet_attr.in_param.perms[i].p + j + 1);
            }
            else
            {
                LineInfo.line.p1 = *(g_permdet_attr.in_param.perms[i].p);
            }
            LineInfo.lineNum = lineCnt;
            lineCnt++;
            LUX_OSD_PermSlash_Show(&LineInfo, bShow);
        }
    }
}

/*越界侦测报警线*/
VOID_X LUX_OSD_CrossSlash_Show(LUX_OSD_SLASHLINE_ST* pLineInfo, BOOL_X bShow)
{
    INT_X i, ret = LUX_ERR;
    DOUBLE_X scale_w = 0, scale_h = 0;
    LUX_OSD_Scale_Get(&scale_w, &scale_h);
    INT_X LinePos = LUX_LINECROSS_START_POS + pLineInfo->lineNum;
    if (LinePos > LUX_LINECROSS_END_POS)
    {
        LinePos = LUX_LINECROSS_END_POS;
    }
    for (i = 0; i < LUX_OSD_GRP_NUM; i++)
    {
        if (i == LUX_STREAM_SUB)
        {
            scale_w = 1;
            scale_h = 1;
        }
        g_osdAttr[LinePos].OSDRgnAttr[i].type = OSD_REG_SLASH;
        g_osdAttr[LinePos].OSDRgnAttr[i].data.lineRectData.color = OSD_IPU_RED;
        g_osdAttr[LinePos].OSDRgnAttr[i].data.lineRectData.linewidth = 5;
        g_osdAttr[LinePos].OSDRgnAttr[i].rect.p0.x = pLineInfo->line.p0.x * scale_w;
        g_osdAttr[LinePos].OSDRgnAttr[i].rect.p0.y = pLineInfo->line.p0.y * scale_h;
        g_osdAttr[LinePos].OSDRgnAttr[i].rect.p1.x = pLineInfo->line.p1.x * scale_w;
        g_osdAttr[LinePos].OSDRgnAttr[i].rect.p1.y = pLineInfo->line.p1.y * scale_h;
        LUX_OutBoundPoint_Convert(0, &g_osdAttr[LinePos].OSDRgnAttr[i].rect.p0);
        LUX_OutBoundPoint_Convert(0, &g_osdAttr[LinePos].OSDRgnAttr[i].rect.p1);

        ret = IMP_OSD_SetRgnAttr(g_prHander[i][LinePos], &g_osdAttr[LinePos].OSDRgnAttr[i]);
        if (ret != LUX_OK)
        {
            IMP_LOG_ERR(TAG, "IMP_OSD_SetRgnAttr Intrusion Obj Rect error !\n");
            return;
        }

        ret = IMP_OSD_ShowRgn(g_prHander[i][LinePos], i, bShow);
        if (ret != LUX_OK)
        {
            IMP_LOG_ERR(TAG, "IMP_OSD_ShowRgn() group%d region%d error\n");
            return;
        }
        printf("channel[%d]: startPoint(x:%d, y:%d), endPoint(x:%d, y:%d) \n", i,
                g_osdAttr[LinePos].OSDRgnAttr[i].rect.p0.x,
                g_osdAttr[LinePos].OSDRgnAttr[i].rect.p0.y,
                g_osdAttr[LinePos].OSDRgnAttr[i].rect.p1.x,
                g_osdAttr[LinePos].OSDRgnAttr[i].rect.p1.y);
    }
}

VOID_X LUX_OSD_LineCross_Show(P_LUX_M_SLASH_ST pLine, BOOL_X bShow)
{
    INT_X i, ret = LUX_ERR;
    LUX_OSD_SLASHLINE_ST LineInfo;
    for (i = 0; i < pLine->slashCnt; i++)
    {
        printf("LineCross: [%d]th Slash\n", i);
        memset(&LineInfo, 0, sizeof(LUX_OSD_SLASHLINE_ST));
        LineInfo.line = pLine->slash[i];
        LineInfo.lineNum = i;
        LUX_OSD_CrossSlash_Show(&LineInfo, bShow);
    }
}

/*ISPOSD 矩形周界*/
VOID_X LUX_ISPOSD_PermRect_Draw(BOOL_X enable)
{
    int ret = LUX_OK;
    int i = 0, j = 0;
    DOUBLE_X scale_w = 0, scale_h = 0;
    LUX_OSD_Scale_Get(&scale_w, &scale_h);
    IMPISPDrawAttr IspDrawAttr;
    IMPPoint convPoints[2];
    memset(&convPoints, 0, sizeof(IMPPoint) * 2);
    memset(&IspDrawAttr, 0, sizeof(IMPISPDrawAttr));

    for (i = 0;i < 2;i++)
    {
        if (i == LUX_STREAM_SUB)
        {
            scale_w = 1;
            scale_h = 1;
        }

        for (j = 0; j < g_permdet_attr.in_param.permcnt; j++)
        {
            IspDrawAttr.draw_chx[i][j].type = IMP_ISP_DRAW_WIND;/*矩形*/
            IspDrawAttr.draw_chx[i][j].draw_cfg.wind.wind_enable = enable;
            if (enable)
            {
                convPoints[0].x = g_permdet_attr.in_param.perms[j].p[0].x * scale_w;//左上角
                convPoints[0].y = g_permdet_attr.in_param.perms[j].p[0].y * scale_h;
                convPoints[1].x = g_permdet_attr.in_param.perms[j].p[2].x * scale_w;//右下角
                convPoints[1].y = g_permdet_attr.in_param.perms[j].p[2].y * scale_h;
                LUX_OutBoundPoint_Convert(i, &convPoints[0]);
                LUX_OutBoundPoint_Convert(i, &convPoints[1]);
                IspDrawAttr.draw_chx[i][j].draw_cfg.wind.wind_alpha = 4;/*范围为[0,4]*/
                IspDrawAttr.draw_chx[i][j].draw_cfg.wind.wind_color.mask_argb.r_value = 0;
                IspDrawAttr.draw_chx[i][j].draw_cfg.wind.wind_color.mask_argb.g_value = 0;
                IspDrawAttr.draw_chx[i][j].draw_cfg.wind.wind_color.mask_argb.b_value = 255;
                IspDrawAttr.draw_chx[i][j].draw_cfg.wind.wind_left = convPoints[0].x;
                IspDrawAttr.draw_chx[i][j].draw_cfg.wind.wind_top = convPoints[0].y;
                IspDrawAttr.draw_chx[i][j].draw_cfg.wind.wind_width = convPoints[1].x - IspDrawAttr.draw_chx[i][j].draw_cfg.wind.wind_left;
                IspDrawAttr.draw_chx[i][j].draw_cfg.wind.wind_height = convPoints[1].y - IspDrawAttr.draw_chx[i][j].draw_cfg.wind.wind_top;
                IspDrawAttr.draw_chx[i][j].draw_cfg.wind.wind_line_width = 5;
                printf("channel[%d] rect[%d](x:%d,xlen:%d, y:%d, ylen:%d) \n", i, j,
                IspDrawAttr.draw_chx[i][j].draw_cfg.wind.wind_left,
                IspDrawAttr.draw_chx[i][j].draw_cfg.wind.wind_top,
                IspDrawAttr.draw_chx[i][j].draw_cfg.wind.wind_width,
                IspDrawAttr.draw_chx[i][j].draw_cfg.wind.wind_height
                );
            }
        }
        ret = IMP_ISP_SetDrawAttr(0, &IspDrawAttr);
        if (ret != LUX_OK)
        {
            IMP_LOG_ERR(TAG, "[%s][%d]IMP_ISP_SetDrawAttr err\n", __func__, __LINE__);
        }
    }
}

/**
 * @description: 设置创建的几个组区域是否显示，
 * @return     ：0 设置成功. 非0 设置失败
 */
INT_X LUX_OSD_Show(BOOL_X bShow)
{
	int ret;
	int i, j;
	for (i = 0; i < LUX_OSD_GRP_NUM; i++) {
		for (j = 0; j < LUX_OSD_REGION_NUM; j++) {
			if (g_osdAttr[j].enable) {
                if (j > 2)
                {
                    continue;//默认不显示
                }
                
                ret = IMP_OSD_ShowRgn(g_prHander[i][j], i, bShow);
                if (ret != 0) {
					IMP_LOG_ERR(TAG, "IMP_OSD_ShowRgn() group%d region%d error\n", i ,j);
					return LUX_ERR;
				}
			}
		}
	}

	return LUX_OK;
}


static void *LUX_OSD_Time_Stamp_Update_Thread(void *p)
{
	int ret;

	/*generate time*/
	char DateStr[40];
	time_t currTime;
	struct tm *currDate;
	unsigned i = 0, j = 0;
	void *grp0DateData = NULL;
	void *grp1DateData = NULL;
	IMPOSDRgnAttrData grp0RgnAttrData;
	IMPOSDRgnAttrData grp1RgnAttrData;
	UINT_X *grp0TimeStampData = NULL;	
    BOOL_X bShow = LUX_TRUE;

	if (g_tSensorWH.width == 2560) {
		grp0TimeStampData = malloc(LUX_OSD_LETTER_NUM * OSD_GRP0_REGION_1440P_H * OSD_GRP0_REGION_1440P_W * sizeof(UINT_X));
	}else{
		grp0TimeStampData = malloc(LUX_OSD_LETTER_NUM * OSD_GRP0_REGION_1080P_H * OSD_GRP0_REGION_1080P_W * sizeof(UINT_X));	
	}
	if (grp0TimeStampData == NULL) {
		IMP_LOG_ERR(TAG, "malloc grp0TimeStampData error\n");
		return NULL;
	}

	UINT_X * grp1TimeStampData = malloc(LUX_OSD_LETTER_NUM * OSD_GRP1_REGION_360P_H * OSD_GRP1_REGION_360P_W * sizeof(UINT_X));
	if (grp1TimeStampData == NULL) {
		IMP_LOG_ERR(TAG, "malloc grp1TimeStampData error\n");
		return NULL;
	}

    /* 读取配置 */
    
    // ret = LUX_INIParse_GetBool(LUX_CONFIG_FILE, "image_config", "watermark_onoff", &bShow);
    // if (0 != ret)
    // {
    //     IMP_LOG_ERR(TAG, "LUX_INIParse_GetBool OSD_watermark_onoff failed!, error num [0x%x] ", ret);
    // }
    bShow = g_configData->image_config.watermark_onoff;
    ret = LUX_OSD_Show(bShow);
    if (ret < 0) {
		IMP_LOG_ERR(TAG, "OSD show error\n");
		return NULL;
	}

	while(1) 
    {
        if (0 == g_configData->image_config.osd_show_time)
        {
            if (TRUE == bShow)
            {
                bShow = FALSE;
                ret = LUX_OSD_Show(bShow);
            }
            sleep(1);
            continue;
        }

        if (FALSE == bShow)
        {
            bShow = TRUE;
            LUX_OSD_Show(bShow);
        }

        int grp0_penpos_t = 0;
        int grp0_fontadv = 0;
        int grp1_penpos_t = 0;
        int grp1_fontadv = 0;

        time(&currTime);
        currTime = currTime + getTimeStampdDiff();
        currDate = localtime(&currTime);
        memset(DateStr, 0, 40);
        strftime(DateStr, 40, "%Y-%m-%d %H:%M:%S", currDate);   /* %H:24小时制 %I:12小时制 */
        for (i = 0; i < LUX_OSD_LETTER_NUM; i++) {
            switch(DateStr[i]) {
                case '0' ... '9':
                    if (g_tSensorWH.width == 2560) {
                        grp0DateData = (void *)gBitmap_1440p[DateStr[i] - '0'].pdata;
                        grp0_fontadv = gBitmap_1440p[DateStr[i] - '0'].width;
                        grp0_penpos_t += gBitmap_1440p[DateStr[i] - '0'].width;
                    }else{
                        grp0DateData = (void *)gBitmap_1080p[DateStr[i] - '0'].pdata;
                        grp0_fontadv = gBitmap_1080p[DateStr[i] - '0'].width;
                        grp0_penpos_t += gBitmap_1080p[DateStr[i] - '0'].width;

                    }
                    grp1DateData = (void *)gBitmap_360p[DateStr[i] - '0'].pdata;
                    grp1_fontadv = gBitmap_360p[DateStr[i] - '0'].width;
                    grp1_penpos_t += gBitmap_360p[DateStr[i] - '0'].width;
                    break;
                case '-':
                    if (g_tSensorWH.width == 2560) {
                        grp0DateData = (void *)gBitmap_1440p[10].pdata;
                        grp0_fontadv = gBitmap_1440p[10].width;
                        grp0_penpos_t += gBitmap_1440p[10].width;
                    }else{
                        grp0DateData = (void *)gBitmap_1080p[10].pdata;
                        grp0_fontadv = gBitmap_1080p[10].width;
                        grp0_penpos_t += gBitmap_1080p[10].width;
                    }
                    grp1DateData = (void *)gBitmap_360p[10].pdata;
                    grp1_fontadv = gBitmap_360p[10].width;
                    grp1_penpos_t += gBitmap_360p[10].width;
                    break;
                case ' ':
                    if (g_tSensorWH.width == 2560) {
                        grp0DateData = (void *)gBitmap_1440p[11].pdata;
                        grp0_fontadv = gBitmap_1440p[11].width;
                        grp0_penpos_t += gBitmap_1440p[11].width;							
                    }else{
                        grp0DateData = (void *)gBitmap_1080p[11].pdata;
                        grp0_fontadv = gBitmap_1080p[11].width;
                        grp0_penpos_t += gBitmap_1080p[11].width;
                    }
                    grp1DateData = (void *)gBitmap_360p[11].pdata;
                    grp1_fontadv = gBitmap_360p[11].width;
                    grp1_penpos_t += gBitmap_360p[11].width;	
                    break;
                case ':':
                    if (g_tSensorWH.width == 2560) {
                        grp0DateData = (void *)gBitmap_1440p[12].pdata;
                        grp0_fontadv = gBitmap_1440p[12].width;
                        grp0_penpos_t += gBitmap_1440p[12].width;
                    }else{
                        grp0DateData = (void *)gBitmap_1080p[12].pdata;
                        grp0_fontadv = gBitmap_1080p[12].width;
                        grp0_penpos_t += gBitmap_1080p[12].width;
                    }
                    grp1DateData = (void *)gBitmap_360p[12].pdata;
                    grp1_fontadv = gBitmap_360p[12].width;
                    grp1_penpos_t += gBitmap_360p[12].width;
                    break;
                default:
                    break;
            }
            if (g_tSensorWH.width == 2560) {
                for (j = 0; j < OSD_GRP0_REGION_1440P_H; j++) {
                    memcpy((void *)((UINT_X *)grp0TimeStampData + j*LUX_OSD_LETTER_NUM*OSD_GRP0_REGION_1440P_W + grp0_penpos_t),
                            (void *)((UINT_X *)grp0DateData + j*grp0_fontadv), grp0_fontadv*sizeof(UINT_X));
                }
            }else{
                for (j = 0; j < OSD_GRP0_REGION_1080P_H; j++) {
                    memcpy((void *)((UINT_X *)grp0TimeStampData + j*LUX_OSD_LETTER_NUM*OSD_GRP0_REGION_1080P_W + grp0_penpos_t),
                            (void *)((UINT_X *)grp0DateData + j*grp0_fontadv), grp0_fontadv*sizeof(UINT_X));
                }
            }
            
            for (j = 0; j < OSD_GRP1_REGION_360P_H; j++) {
                memcpy((void *)((UINT_X *)grp1TimeStampData + j*LUX_OSD_LETTER_NUM*OSD_GRP1_REGION_360P_W + grp1_penpos_t),
                        (void *)((UINT_X *)grp1DateData + j*grp1_fontadv), grp1_fontadv*sizeof(UINT_X));
            }
        }
                
        grp0RgnAttrData.picData.pData = grp0TimeStampData;
        grp1RgnAttrData.picData.pData = grp1TimeStampData;

        
        if (g_osdAttr[0].enable) {
            if (LUX_OK != IMP_OSD_UpdateRgnAttrData(g_prHander[0][0], &grp0RgnAttrData)) {
                IMP_LOG_ERR(TAG, "IMP_OSD_UpdateRgnAttrData() group0 error\n");
            }
            if (LUX_OSD_GRP_NUM == 2) {
                if (LUX_OK != IMP_OSD_UpdateRgnAttrData(g_prHander[1][0], &grp1RgnAttrData)) {
                    IMP_LOG_ERR(TAG, "IMP_OSD_UpdateRgnAttrData() group1 error\n");
                }
            }
        }
		sleep(1);
	}

	free(grp0TimeStampData);
	free(grp1TimeStampData);

	return NULL;
}


/**
 * @description: 创建OSD的bgramap更新线程
 * @return     ：0 成功. 非0 失败
 */
INT_X LUX_OSD_Time_Stamp_Update()
{
	INT_X ret = LUX_ERR;

	ret = Task_CreateThread(LUX_OSD_Time_Stamp_Update_Thread, NULL);
	if (LUX_OK != ret) {
		IMP_LOG_ERR(TAG, "Task_CreateThread failed.\n");
		return LUX_ERR;
	}
	return LUX_OK;
}

/**
 * @description: 获取sensor的像素宽度和高度，重置主码流各OSD区域的位置参数
* @return	  ：0 成功. 非0 失败
*/
INT_X LUX_OSD_SetRgnPositionParams(int rgnNum)
{
	CHAR_X sensorName[64] = {0};
	INT_X ret = LUX_ERR;

	/*读取配置文件的sensor名字*/
    strcpy(sensorName, g_sensor_name);
    //ret = LUX_INIParse_GetString(LUX_CONFIG_FILE, "sensor", "name", sensorName);
	//if(LUX_OK != ret) {
	//	IMP_LOG_ERR(TAG, "LUX_INIParse_GetString failed,error [0x%x]\n",ret);
	//	return LUX_ERR;
	//}

	/*获取镜头分辨率大小*/
	ret = LUX_COMM_GetSensorWH(sensorName, &g_tSensorWH);
	if(LUX_OK != ret) {
		IMP_LOG_ERR(TAG, "LUX_INIParse_GetString failed,error [0x%x]\n",ret);
		return LUX_ERR;
	}

	/*配置主码流OSD区域的位置参数*/
	switch (rgnNum)	
	{
        case 0:
        {
            g_osdAttr[0].OSDRgnAttr[0].rect.p0.x = 10;
            g_osdAttr[0].OSDRgnAttr[0].rect.p0.y = 10;
            if (g_tSensorWH.width == 2560) {
                g_osdAttr[0].OSDRgnAttr[0].rect.p1.x = g_osdAttr[0].OSDRgnAttr[0].rect.p0.x + 20 * OSD_GRP0_REGION_1440P_W - 1;
                g_osdAttr[0].OSDRgnAttr[0].rect.p1.y = g_osdAttr[0].OSDRgnAttr[0].rect.p0.y + OSD_GRP0_REGION_1440P_H - 1;

            }else{
                g_osdAttr[0].OSDRgnAttr[0].rect.p1.x = g_osdAttr[0].OSDRgnAttr[0].rect.p0.x + 20 * OSD_GRP0_REGION_1080P_W- 1;
                g_osdAttr[0].OSDRgnAttr[0].rect.p1.y = g_osdAttr[0].OSDRgnAttr[0].rect.p0.y + OSD_GRP0_REGION_1080P_H - 1;
            }
            break;
        }
        case 1:
        {
            g_osdAttr[1].OSDRgnAttr[0].rect.p0.x = LUX_OSD_LOGO_GRP0_START_X;
            g_osdAttr[1].OSDRgnAttr[0].rect.p0.y = LUX_OSD_LOGO_GRP0_START_Y;
            g_osdAttr[1].OSDRgnAttr[0].rect.p1.x = g_osdAttr[1].OSDRgnAttr[0].rect.p0.x + LUX_OSD_GRP0_LOGO_W - 1;
            g_osdAttr[1].OSDRgnAttr[0].rect.p1.y = g_osdAttr[1].OSDRgnAttr[0].rect.p0.y + LUX_OSD_GRP0_LOGO_H - 1;
            break;
        }
        case 2:
        {
            g_osdAttr[2].OSDRgnAttr[0].rect.p0.x =g_tSensorWH.width / 2 - 100;
            g_osdAttr[2].OSDRgnAttr[0].rect.p0.y = g_tSensorWH.height / 2 - 100;
            g_osdAttr[2].OSDRgnAttr[0].rect.p1.x = g_osdAttr[2].OSDRgnAttr[0].rect.p0.x + g_tSensorWH.width/2-1+50;
            g_osdAttr[2].OSDRgnAttr[0].rect.p1.y = g_osdAttr[2].OSDRgnAttr[0].rect.p0.y + g_tSensorWH.height/2-1+50;
            break;
        }
        // case 3:
        // {
        //     g_osdAttr[3].OSDRgnAttr[0].rect.p0.x = 300;
        //     g_osdAttr[3].OSDRgnAttr[0].rect.p0.y = 300;
        //     g_osdAttr[3].OSDRgnAttr[0].rect.p1.x = 300 + 600 - 1;
        //     g_osdAttr[3].OSDRgnAttr[0].rect.p1.y = 300 + 300 - 1;
        //     break;
        // }
        // default:
        // {
        //     IMP_LOG_ERR(TAG, "Invaild rgnNum%d\n", rgnNum);
        //     return LUX_ERR;
        //     break;
        // }
	}

	return LUX_OK;
}


/**
 * @description: 创建&注册OSD区域到OSD组、设置OSD组区域属性和区域属性、打开OSD开关.
 * @return     ：返回指向OSD创建区域的地址
 */
INT_X *LUX_OSD_CreateRgn(int grpNum)
{
	int ret;
	IMPRgnHandle *pRgnHander[LUX_OSD_GRP_NUM];
	IMPRgnHandle RgnHanderNum[LUX_OSD_REGION_NUM];

	INT_X i, j;

	pRgnHander[grpNum] = malloc(LUX_OSD_REGION_NUM * sizeof(IMPRgnHandle));
	if (pRgnHander[grpNum] <= 0) {
		IMP_LOG_ERR(TAG, "malloc() prHander[%d] error !\n", grpNum);
		return NULL;
	}

    /*矩形框区域 OSD*/
    for (j = 1; j < LUX_RECT_NUM_MAX + LUX_PERMLINE_NUM_MAX; j++)
    {
        memcpy(&g_osdAttr[j + LUX_LAST_REGION_NUM], &g_osdAttr[LUX_LAST_REGION_NUM], sizeof(LUX_OSD_ATTR_ST));
    }

	for (i = 0; i < LUX_OSD_REGION_NUM; i++) {
		if (g_osdAttr[i].enable)
		{
			/* 创建OSD区域 */
			RgnHanderNum[i] = IMP_OSD_CreateRgn(NULL);
			if (RgnHanderNum[i] == INVHANDLE) {
				IMP_LOG_ERR(TAG, "IMP_OSD_CreateRgn error !\n");
				return NULL;
			}

			//query osd rgn create status
			IMPOSDRgnCreateStat stStatus;
			memset(&stStatus,0x0,sizeof(IMPOSDRgnCreateStat));
			ret = IMP_OSD_RgnCreate_Query(RgnHanderNum[i],&stStatus);
			if(ret < 0){
				IMP_LOG_ERR(TAG, "IMP_OSD_RgnCreate_Query error !\n");
				return NULL;
			}

			/* 注册OSD区域至OSD组中 */
			ret = IMP_OSD_RegisterRgn(RgnHanderNum[i], grpNum, NULL);
			if (ret < 0) {
				IMP_LOG_ERR(TAG, "IVS IMP_OSD_RegisterRgn failed\n");
				return NULL;
			}

			//query osd rgn register status
			IMPOSDRgnRegisterStat stRigStatus;
			memset(&stRigStatus,0x0,sizeof(IMPOSDRgnRegisterStat));
			ret = IMP_OSD_RgnRegister_Query(RgnHanderNum[i], grpNum, &stRigStatus);
			if (ret < 0) {
				IMP_LOG_ERR(TAG, "IMP_OSD_RgnRegister_Query failed\n");
				return NULL;
			}

			/* 根据sensor信息配置主码流OSD区域位置参数 */
			ret = LUX_OSD_SetRgnPositionParams(i);
			if (ret < 0) {
				IMP_LOG_ERR(TAG, "IMP_OSD_SetRgnAttr error !\n");
				return NULL;
			}
			/* 设置OSD区域属性 */
			ret = IMP_OSD_SetRgnAttr(RgnHanderNum[i], &g_osdAttr[i].OSDRgnAttr[grpNum]);
			if (ret < 0) {
				IMP_LOG_ERR(TAG, "IMP_OSD_SetRgnAttr error !\n");
				return NULL;
			}

			/* 设置OSD组区域属性 */
			if (IMP_OSD_SetGrpRgnAttr(RgnHanderNum[i], grpNum, &g_osdAttr[i].OSDGrpRgnAttr) < 0) {
				IMP_LOG_ERR(TAG, "IMP_OSD_SetGrpRgnAttr error !\n");
				return NULL;
			}
		}

	}

	/* 启动OSD */
	ret = IMP_OSD_Start(grpNum);
	if (ret < 0) {
		IMP_LOG_ERR(TAG, "IMP_OSD_Start TimeStamp, Logo, Cover and lineRect error !\n");
		return NULL;
	}

	for (i =0; i < LUX_OSD_REGION_NUM; i++) {
		pRgnHander[grpNum][i] = RgnHanderNum[i];
	}

	return pRgnHander[grpNum];
}

/**
 * @description: 创建OSD组.
 * @returnreturn 0 成功. 非0 失败
 */
INT_X LUX_OSD_CreateGroup(INT_X grpNum)
{
    return IMP_OSD_CreateGroup(grpNum);
}

/**
 * @description: OSD模块初始化 读取水印文件
 * @return 0 成功，非0 失败
 */
INT_X LUX_OSD_Init()
{
	INT_X i = 0;
	INT_X ret = LUX_ERR;
    LUX_MODULES_BIND_ST bind;
    FILE *fp = NULL;
#ifdef CONFIG_PTZ_IPC
    fp = fopen("/system/bin/logo/logodata_436x236.bin","rb");
    if(fp != NULL)
    {
        memset(logodata_436x236_bgra,0,LOGO_SIZE_HD);
        fread(logodata_436x236_bgra, 1, LOGO_SIZE_HD, fp);
        fclose(fp);
    }
    g_osdAttr[1].OSDRgnAttr[0].data.picData.pData = logodata_436x236_bgra;
#else
    fp = fopen("/system/bin/logo/logodata_382x88.bin","rb");
    if(fp != NULL)
    {
        memset(logodata_382x88_bgra,0,LOGO_SIZE_HD);
        fread(logodata_382x88_bgra, 1, LOGO_SIZE_HD, fp);
        fclose(fp);
    }

    g_osdAttr[1].OSDRgnAttr[0].data.picData.pData = logodata_382x88_bgra;
#endif
    fp = fopen("/system/bin/logo/logodata_106x58.bin","rb");
    if(fp != NULL)
    {
        memset(logodata_106x58_bgra,0,LOGO_SIZE_SD);
        fread(logodata_106x58_bgra, 1, LOGO_SIZE_SD, fp);
        fclose(fp);
    }
    g_osdAttr[1].OSDRgnAttr[1].data.picData.pData = logodata_106x58_bgra;


    for (i = 0; i < LUX_OSD_GRP_NUM; i++)
    {
		/* Step.1 create group */
		ret = LUX_OSD_CreateGroup(i);
	    if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "IMP_OSD_CreateGroup(%d) error!\n", i);
            return LUX_OSD_CREATE_GROUP_ERR;
        }
		
		/* Step.2 create osd region */	
		g_prHander[i] = LUX_OSD_CreateRgn(i);
		if (g_prHander[i] <= 0)
        {
            IMP_LOG_ERR(TAG, "LUX_OSD_CreateRgn(%d) error!\n", i);
            return LUX_OSD_CREATE_REGION_ERR;			
		}

        /* 绑定osd通道到源模块通道 */
        memset(&bind, 0, sizeof(bind));
        bind.srcChan   = 0;
        bind.srcModule = (i ? LUX_MODULS_IVS : LUX_MODULS_FSRC); /* osd通道0绑定framesource0，osd通道1绑定ivs通道0（此处通道的概念对应君正组的概念） */
        bind.dstChan   = i;
        bind.dstModule = LUX_MODULS_OSD;
        ret = LUX_COMM_Bind_AddModule(&bind);
        if (LUX_OK != ret)
        {
            IMP_LOG_ERR(TAG, "LUX_COMM_Bind_AddModule (%d) osd error!\n", i);
            return LUX_VIDEO_CHAN_CREATE_ERR;
        }
    }

	return LUX_OK;
}


/**
 * @description: OSD模块去初始化
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_OSD_Exit()
{
	INT_X i, j;
	INT_X ret = LUX_ERR;

	for (i = 0; i < LUX_OSD_GRP_NUM; i++) {
		for (j = 0; j < LUX_OSD_REGION_NUM; j++) {
			if (g_osdAttr[j].enable) {
				/*设置显示区域为不显示*/
				ret = IMP_OSD_ShowRgn(g_prHander[i][j], i, 0);
				if (ret < 0) {
					IMP_LOG_ERR(TAG, "IMP_OSD_ShowRgn close timeStamp error\n");
					return LUX_ERR;
				}	

				/* 注销OSD区域 */
				ret = IMP_OSD_UnRegisterRgn(g_prHander[i][j], i);
				if (ret < 0) {
					IMP_LOG_ERR(TAG, "IMP_OSD_UnRegisterRgn timeStamp error\n");
					return LUX_ERR;
				}

				IMP_OSD_DestroyRgn(g_prHander[i][j]);
				
			}
		}

		ret = IMP_OSD_DestroyGroup(i);
		if (ret < 0) {
			IMP_LOG_ERR(TAG, "IMP_OSD_DestroyGroup(0) error\n");
			return LUX_ERR;
		}

		free(g_prHander[i]);
		g_prHander[i] = NULL;
	}
	

	return LUX_OK;
}

