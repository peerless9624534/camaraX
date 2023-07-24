/*
 * comm_func.c
 *
 * 公用函数实现
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "comm_def.h"
#include "comm_func.h"
#include "lux_sys.h"
#include "comm_error_code.h"
#include "zbar.h"
#include <lux_isp.h>
#include <jwhead.h>

LUX_COMM_ATTR_ST g_commAttr;


/**
 * @description: 添加模块绑定参数到全局信息表，模块初始化完成后一次性绑定
 *              各个模块需要从源端绑定时，调用此接口配置绑定信息
 * @param [in] pBind 模块绑定参数
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_COMM_Bind_AddModule(LUX_MODULES_BIND_ST *pBind)
{
    if (NULL == pBind)
    {
        printf("%s:%d null ptr!\n", __func__, __LINE__);
        return LUX_ERR;
    }
    INT_X i = 0;
    UINT_X ret = LUX_ERR;

    for (i = 0; i < LUX_MODULES_BIND_NUM; i++)
    {
        if (!g_commAttr.binds[i].bUsed)
        {
            memcpy(&g_commAttr.binds[i], pBind, sizeof(LUX_MODULES_BIND_ST));
            g_commAttr.binds[i].bUsed = LUX_TRUE;
            g_commAttr.bindNum++;
            ret = LUX_OK;
            break;
        }
    }

    return ret;
}

/**
 * @description: 建立模块绑定连接
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_COMM_Bind_Connect()
{
    INT_X i = 0;
    UINT_X ret = LUX_OK;

    if (!g_commAttr.bindNum)
    {
        printf("%s:%d bindNum error:%#x\n", __func__, __LINE__, g_commAttr.bindNum);
        return ret;
    }

    for (i = 0; i < g_commAttr.bindNum; i++)
    {
        /* 调用系统绑定 */
        ret = LUX_SYS_Bind(&g_commAttr.binds[i]);
        if (LUX_OK != ret)
        {
            printf("%s:%d bind error:%#x\n", __func__, __LINE__, ret);
            //return LUX_ERR;
        }
    }

    return ret;
}

/**
 * @description: 解除模块绑定连接
 * @return 0 成功，非零失败，返回错误码
 */
INT_X LUX_COMM_Bind_DisConnect()
{
    INT_X i = 0;
    UINT_X ret = LUX_OK;

    if (!g_commAttr.bindNum)
    {
        printf("%s:%d bindNum error:%#x\n", __func__, __LINE__, g_commAttr.bindNum);
        return ret;
    }

    for (i = 0; i < g_commAttr.bindNum; i++)
    {
        ret = LUX_SYS_UnBind(&g_commAttr.binds[i]);
        if (LUX_OK != ret)
        {
            printf("%s:%d bind error:%#x\n", __func__, __LINE__, ret);
            //return LUX_ERR;
        }
    }

    return ret;
}

/**
 * @description: 二维码解析
 * @return 0 成功，-1 失败
 */
INT_X LUX_COMM_Zbar_Parse(void *y8data, int w, int h, char *pQrCode)
{
    if (NULL == y8data || NULL == pQrCode || 0 == w || 0 == h)
    {
        printf("pqrcode param ERR width:%d height:%d \n", w, h);
        return LUX_PARM_NULL_PTR;
    }
    int ret = LUX_OK;
    zbar_image_t *image = NULL;
    zbar_image_scanner_t *scanner = NULL;

    do
    {
        uint32_t srcfmt = (('Y' & 0xff) | (('8' & 0xff) << 8) | (('0' & 0xff) << 16) | (('0' & 0xff) << 24));
        scanner = zbar_image_scanner_create();
        image = zbar_image_create();
        zbar_image_set_userdata(image, NULL);
        zbar_image_set_format(image, srcfmt);
        zbar_image_set_size(image, w, h);
        zbar_image_set_data(image, y8data, w * h, (zbar_image_cleanup_handler_t*)zbar_image_get_userdata);
        zbar_scan_image(scanner, image);
        const zbar_symbol_t *symbol = zbar_image_first_symbol(image);
        if (NULL == symbol)
        {
            ret = LUX_COMM_ZBAR_NOT_FOUND;
            break;
        }

        for (; symbol; symbol = zbar_symbol_next(symbol))
        {
            zbar_symbol_type_t typ = zbar_symbol_get_type(symbol);
            const char *data = zbar_symbol_get_data(symbol);
            if (data != NULL)
            {
                strncpy(pQrCode, data, zbar_symbol_get_data_length(symbol));
                printf("decoded %s symbol \"%s\"\n", zbar_get_symbol_name(typ), data);
            }
        }
    } while(0);

    zbar_image_destroy(image);
    zbar_image_scanner_destroy(scanner);

    return ret;
}


/**
 * @description: 根据sensor的名字找到对应的sensor配置参数
 * @param [in] sensorName  sensor的名字
 * @param [out] pOutSensorWH  sensor的宽高
 * @return 0 成功， 非零失败，返回错误码
 */
 INT_X LUX_COMM_GetSensorWH(PCHAR_X sensorName, LUX_COMM_FUNC_SNS_WH *pOutSensorWH)
{	
	if(NULL == sensorName)
	{
		printf("%s:%d LUX_COMM_GetSensorWH failed\n", __func__, __LINE__);
		return LUX_PARM_NULL_PTR;
	}

    INT_X ret = LUX_ERR;
    LUX_ISP_SENSOR_INFO_ST pSensorInfor;

    ret = LUX_ISP_GetSensorInfo(sensorName, &pSensorInfor);
    if(LUX_OK != ret)
    {
        printf("%s:%d LUX_ISP_GetSensorInfo fail return error[0x%x]\n", __func__, __LINE__, ret);
        return LUX_ERR;
    }

    pOutSensorWH->width  = pSensorInfor.snsWidth;
    pOutSensorWH->height = pSensorInfor.snsHeight;

    return LUX_OK;
}

/**
 * @description: 解析报警区域字符串
 * @param [in] str  报警区域字符串
 * @param [in] key  键
 * @param [out] value  解析出的键值
 * @return 0 成功， 非零失败，返回错误码
 */
int LUX_COMM_ParseStr(char *str, char *key, int *value)
{
    int i, j = 0;
    int len = 0;
    char tmpStr[32] = {0};
    char strValue[32] = {0};

    len = strlen(str);
    for(i = 0; i < len; i++)
    {
        if('\"' == str[i])
        {
            for(j = 0; str[++i] != '\"'; j++)
            {
                tmpStr[j] = str[i];
            }

            tmpStr[j] = '\0';
            j = 0;
            if(!strcmp(tmpStr, key))
            {
                for(j = 0; str[i++] != ':'; j++);

                for(j = 0; str[i] != ','; j++)
                {
                    if('}'== str[i+1] && '}'== str[i+2])
                    {
                        strValue[j] = str[i++];
                        j++;
                        break;
                    }
                    strValue[j] = str[i++];
                } 
                
                strValue[j] = '\0';
                *value = atoi(strValue);
                return 0;
            }
        }

    }
    printf("%s:[%d]Not found [%s] value !\n",__FUNCTION__, __LINE__, key);
    return -1;
}