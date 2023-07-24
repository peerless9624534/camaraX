
#ifndef __RTSP_API_H__
#define __RTSP_API_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */
/**************************************************************************************/

typedef struct 
{
    int type;
    char *p_buf;
    unsigned int size;
    unsigned long long int pts;
    unsigned long long int timestamp;
} RTSP_STREAM_S;

void RTSP_Server_Init();
void RTSP_Server_Exit();

int RTSP_Send2Client_Sync(RTSP_STREAM_S *pstStream);
int RTSP_Send2Client_Async(RTSP_STREAM_S *pstStream);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
#endif /* End of #ifndef __SAMPLE_COMMON_H__ */
