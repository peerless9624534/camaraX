#ifndef __LUX_NV2JPEG_H__
#define __LUX_NV2JPEG_H__

#include <comm_def.h>
#include "lux_video.h"

// INT_X LUX_Jpeg_Init(INT_X Chn_Id);
// INT_X LUX_Jpeg_Deinit(INT_X Chn_Id);
INT_X LUX_Jpeg_Init(void);
INT_X LUX_Jpeg_Deinit(void);
INT_X LUX_Jpeg_Encode(INT_X Chn_Id,uint8_t *pollingBuffer,UINT_X Buffersize,LUX_ENCODE_STREAM_ST *picInfo);
INT_X LUX_Jpeg_FreeStream(INT_X Chn_Id,LUX_ENCODE_STREAM_ST *pPicInfo);

/**
 * @brief 
 * 创建影集
 */
INT_X LUX_Jpeg_Album_Init();
INT_X LUX_Jpeg_Album_DeInit();
INT_X LUX_Jpeg_Album_Start();
INT_X LUX_Jpeg_Album_Stop();
BOOL_X LUX_Jpeg_Album_IsStart();
#endif
