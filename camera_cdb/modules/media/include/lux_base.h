/*
 * lux_base.h
 *
 * 基础功能、通用功能函数头文件
 */

#ifndef __LUX_BASE_H__
#define __LUX_BASE_H__

#include <jwhead.h>

#include <semaphore.h>
#include <pthread.h>

 //#include <ivs_inf_move.h>
 //#include <ivs_inf_personDet.h>

#include <comm_def.h>
#include <comm_func.h>
//#include "lux_video.h"

/* 编码通道控制流 */
#define LUX_VENC_GRP_NUM 2  /* 编码模块group数 */
#define LUX_VENC_CHN_NUM 2  /* 每个 group 可用的channel数 */

/* 编码通道数据流 */
#define LUX_VENC_CHN_TMPBUF_LEN (512 * 1024)
#define LUX_VENC_CHN_STREAM_CNT (8)

/* 线程、线程锁 */
typedef pthread_mutex_t OSI_MUTEX;

typedef enum
{
    LUX_ENC_SLICE_SI = 4,         /**< AVC SI Slice */
    LUX_ENC_SLICE_SP = 3,         /**< AVC SP Slice */
    LUX_ENC_SLICE_GOLDEN = 3,     /**< Golden Slice */
    LUX_ENC_SLICE_I = 2,          /**< I Slice (can contain I blocks) */
    LUX_ENC_SLICE_P = 1,          /**< P Slice (can contain I and P blocks) */
    LUX_ENC_SLICE_B = 0,          /**< B Slice (can contain I, P and B blocks) */
    LUX_ENC_SLICE_CONCEAL = 6,    /**< Conceal Slice (slice was concealed) */
    LUX_ENC_SLICE_SKIP = 7,       /**< Skip Slice */
    LUX_ENC_SLICE_REPEAT = 8,     /**< Repeat Slice (repeats the content of its reference) */
    LUX_ENC_SLICE_MAX_ENUM,       /**< sentinel */
} LUX_ENCODE_SLICE_TYPE;

/* 编码模块通道属性 */
typedef struct
{
    INT_X       chnId;      /* 通道ID号 */
    BOOL_X      bValid;     /* 通道是否有效，不使用此通道置为 FALSE，静态属性，不可更改 */
    BOOL_X      bStart;     /* 通道开启状态，TRUE 表示已开始编码 */
    OSI_MUTEX   chnMux;
    LUX_STREAM_CHANNEL_EN streamChn; /* 码流通道通道出流模式 */

    VOID_X* pTmpBuf;           /* 临时缓存，sdk获取的码流数据回头时，拼接使用 */
    UINT_X streamIdx;          /* 当前使用的 streamTmp 索引 */
    IMPEncoderStream streamTmp[LUX_VENC_CHN_STREAM_CNT]; /* 码流缓存个数，依赖于sdk定义的编码缓存个数 */
    IMPEncoderChnAttr chnAttr; /* 通道属性（IMP） */
} ENCODER_CHN_ATTR_ST;

/* 编码模块通道属性 */
typedef struct
{
    INT_X grpId;                     /* 组ID号 */
    INT_X             bindSrcChan;   /* 编码通道要绑定的源通道 */
    LUX_MODULS_DEV_EN bindSrcModule; /* 编码通道要绑定的源模块 */

    ENCODER_CHN_ATTR_ST encChnAttr[LUX_VENC_CHN_NUM]; /* grpId 下的通道，不同grp的chn号也是递增形式 */
} ENCODER_GRP_ATTR_ST;

typedef  struct LUX_ENCODE_STREAM
{
    UINT_X chnId;
    VOID_X* pAddr;
    UINT_X streamLen;
    LUX_ENCODE_SLICE_TYPE type;
    UINT_X   pts;
    UINT64_X timeStamp;
    VOID_X* pPrivData;    /* 私有数据，不对外使用 */
}LUX_ENCODE_STREAM_ST;

/* 编码模块属性结构体 */
typedef struct LUX_VIDEO_ENC_ATTR
{
    BOOL_X  bInit;  /* 模块初始化标识 */
    ENCODER_GRP_ATTR_ST encGrpAttr[LUX_VENC_GRP_NUM];
} LUX_VIDEO_ENC_ATTR_ST;




/**
 * IMP帧图像信息定义.
 */
typedef struct {
    int index;			/**< 帧序号 */
    int pool_idx;		/**< 帧所在的Pool的ID */

    UINT_X width;			/**< 帧宽 */
    UINT_X height;			/**< 帧高 */
    UINT_X pixfmt;			/**< 帧的图像格式 */
    UINT_X size;			/**< 帧所占用空间大小 */

    UINT_X phyAddr;	/**< 帧的物理地址 */
    UINT_X virAddr;	/**< 帧的虚拟地址 */

    void* pool;
    INT64_X timeStamp;	/**< 帧的时间戳 */
    UINT_X priv[0];	/* 私有数据 */
} LUX_DUMP_NV12_PIC_INFO_ST;

/*视频源模块通道属性*/
typedef struct
{
    UINT_X index; //0 for main channel ,1 for second channel
    UINT_X enable;
    OSI_MUTEX mux;
    IMPFSChnAttr fsChnAttr;
} LUX_FSRC_CHN_ATTR_ST;

/*视频源模块属性*/
typedef struct LUX_FSRC_ATTR
{
    BOOL_X bInit;
    LUX_FSRC_CHN_ATTR_ST fsChn[FS_CHN_NUM];
} LUX_FSRC_ATTR_ST;



#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

    /* base模块通用错误 */
#define BASE_COMM_ERR (-1)
#define alignment_down(a, size) (a & (~(size-1)) )
#define alignment_up(a, size)   ((a+size-1) & (~ (size-1)))

/*计时器个数*/
#define     LUX_BASE_TIEMR_NUM      16



/* 信号量 */
#define SEM_NO_WAIT      (0)    /* [in]  尝试获取，获取不到立刻返回 */
#define SEM_TRY_FAILED   (-2)   /* [out] 尝试获取，未获取到返回码 */

#define SEM_FOREVER      (-1)   /* [in]  阻塞式等待 */
#define SEM_WAIT_TIMEOUT (-3)   /* [out] 阻塞等待超时的返回码 */

    typedef sem_t OSI_SEM;

    /* 函数定义 */

    void app_itoa(int value, char cBuf[8]);
    /**
     * @description: 创建一个线程,并设置分离属性
     * @param [in] func：需要创建的线程函数
     * @param [in] arg：线程函数输入参数，可以为NULL
     * @return 0：成功，-1：失败；
     */
    int Task_CreateThread(void* func, void* arg);

    /**
     * @description: 创建互斥锁
     * @param [out] pMutex：互斥锁handle
     * @return 0：成功，-1：失败；
     */
    int Thread_Mutex_Create(OSI_MUTEX* pMutex);

    /**
     * @description: 销毁互斥锁
     * @param [in] pMutex：handle
     * @return 0：成功，-1：失败；
     */
    int Thread_Mutex_Destroy(OSI_MUTEX* pMutex);

    /**
     * @description: 互斥锁加锁
     * @param [in] pMutex：handle
     * @return 0：成功，-1：失败；
     */
    int Thread_Mutex_Lock(OSI_MUTEX* pMutex);

    /**
     * @description: 互斥锁解锁
     * @param [in] pMutex：handle
     * @return 0：成功，-1：失败；
     */
    int Thread_Mutex_UnLock(OSI_MUTEX* pMutex);

    /**
     * @description: 创建信号量,仅限单进程可用，多进程不可用
     * @param [in] value：创建资源个数，为0可当作互斥锁使用，大于0当作资源锁使用
     * @param [out] pSem：信号量handle
     * @return 0：成功，-1：失败
     */
    int Semaphore_Create(unsigned int value, OSI_SEM* pSem);

    /**
     * @description: 销毁信号量
     * @param [in] pSem：信号量handle
     * @return 0：成功，-1：失败
     */
    int Semaphore_Destroy(OSI_SEM* pSem);

    /**
     * @description: 释放信号量，+1操作
     * @param [in] pSem：信号量handle
     * @return 0：成功，-1：失败
     */
    int Semaphore_Post(OSI_SEM* pSem);

    /**
     * @description: 获取信号量，-1操作
     * @param [in] pSem：信号量handle
     * @param [in] waitMs：等待时间，-1：死等，0：立即返回，大于0：超时时间
     * @return 0：成功，-1：失败
     */
    int Semaphore_Pend(OSI_SEM* pSem, int waitMs);

    /**
     * @description: 保存时区时间差
     * @param void
     * @return [out] 时间：单位:S
     */
    void setTimeStampdDiff(int time_zone);

    /**
     * @description: 获取时区时间差
     * @param void
     * @return [out] 时间：单位:S
     */
    int getTimeStampdDiff(void);

    /**
     * @description: 获取当前时间戳
     * @param void
     * @return [out] 时间：单位:S
     */
    unsigned int gettimeStampS(void);

    /**
     * @description: 获取系统启动时间
     * @param void
     * @return [out] 时间：单位:S
     */
    unsigned int getTime_getS(void);

    /**
     * @description: 获取系统启动时间
     * @param void
     * @return [out] 时间：单位:ms
     */
    unsigned int getTime_getMs(void);

    /**
     * @description: 获取系统启动时间
     * @param void
     * @return [out] 时间：单位:us
     */
    unsigned long int getTime_getUs(void);

#define     LUX_BASE_QUE_MAXSIZE     64
#define     LUX_BASE_RECT_NUM_MAX    8

    /**
     * @description: 获取EventTime的值，时间戳
     * @return unsigned int
     */
    unsigned int GetEventTime();
    /**
     * @description: 设置当前时间为EventTime的值，时间戳
     * @return void
     */
    void SetEventTime();

    /******************************************************************
     *                                                                *
     *                         队列基本操作函数                        *
     *                                                                *
     ******************************************************************/
    typedef enum
    {
        LUX_Base_QUE_NONE,
        LUX_BASE_PERSONDET_QUE_EN,
        LUX_BASE_FACEDET_QUE_EN,
        LUX_BASE_PERMDET_QUE_EN,
        LUX_BASE_QUE_BUTTON,

    }LUX_BASE_ALGOQUE_TYPE_EN;

    /*点的定义*/
    typedef struct {
        int x;    /**< 横向的坐标值 */
        int y;    /**< 纵向的坐标值 */

    }LUX_BASE_IVSPOINT_ST;

    /* 矩形框的定义*/
    typedef struct {
        LUX_BASE_IVSPOINT_ST  ul;    /**< 左上角点坐标 */
        LUX_BASE_IVSPOINT_ST  br;    /**< 右下角点坐标 */

    }LUX_BASE_IVSRECT_ST;

    typedef struct
    {
        int x; /**< 横向的坐标值 */
        int y; /**< 纵向的坐标值 */
    } LUX_IVS_Point;
    typedef struct
    {
        LUX_IVS_Point ul; /**< 左上角点坐标 */
        LUX_IVS_Point br; /**< 右下角点坐标 */
    } LUX_IVS_Rect;

    typedef struct
    {
        LUX_IVS_Rect box; /**reserved*/
        LUX_IVS_Rect show_box; /**< 人形区域坐标 */
        LUX_IVS_Rect pred_box; /**< 预验人形区域坐标 */
        float confidence; /**< 人形检测结果的置信度 */
        int id;
    } LUX_PERSON_INFO;

    typedef struct {
        LUX_IVS_Point det_rects[200]; /**< 检测出移动区域的宫格坐标 */
        int area[200];/**< 移动区域的area */
    }LUX_BASE_MOVE_REGINFO_ST;


    typedef void(*CallBackFunc)(void* pCallBackInfo);
    /*消息数据*/
    typedef struct
    {
        LUX_BASE_ALGOQUE_TYPE_EN   type;

        bool ivsStart[LUX_IVS_ALGO_NUM];                    /*该算法是否可用*/
        /*MOVE DETECT*/
        int   retRoi;                                       /*是否探测到*/
        int detcount;                                       /*检测出移动区域的宫格数量*/
        LUX_BASE_MOVE_REGINFO_ST    rectInfo;               /*检测初移动区域信息*/
        /*PERSON DETECT*/
        int rectcnt;                                        /*识别人形数量*/
        LUX_PERSON_INFO person[20];                         /*识别出的人形信息*/
        /*PERSON TRACKER*/
        void* IVSResult;                                    /*IVS算法处理结果*/
        void* pCallBackInfo;                                /*回调函数入口*/
        CallBackFunc pCallBackFunc;                         /*回调函数*/

    }LUX_BASE_IVSDATA_ST;


    /*队列结构体*/
    typedef struct
    {
        LUX_BASE_IVSDATA_ST IVSDetData[LUX_BASE_QUE_MAXSIZE];
        int front;
        int rear;

    }LUX_BASE_IVSQUE_ST;

    /**
     * @description: 消息队列初始化
     * @return 0 成功，非零失败
     */
    int InitQueue();

    /**
     * @description: 消息队列长度
     * @param [in] pQue  队列
     * @return 0 成功，非零失败
     */
    int QueueLength(LUX_BASE_IVSQUE_ST* pQue);

    /**
     * @description: 消息入队
     * @param [in] pQue  队列
     * @param [in] pInData 出队数据
     * @return 0 成功，非零失败
     */
    int PushQueue(LUX_BASE_IVSQUE_ST* pQue, LUX_BASE_IVSDATA_ST* pInData);

    /**
     * @description: 消息出队
     * @param [in] pQue  队列
     * @param [out] pOutData 出队数据
     * @return 0 成功，非零失败
     */
    int PopQueue(LUX_BASE_IVSQUE_ST* pQue, LUX_BASE_IVSDATA_ST* pOutData);

    /*执行系统指令 */
    int LUX_BASE_System_CMD(char* pCmd);
    int LUX_BASE_System_CMD_Ex(const char* pCmd, char *out);

    /* 检测相关文件的是否与版本匹配 */
    int LUX_BASE_Check_File_Vertion();

    /**
     * @description: 抓取高清图片
     * @param [in] picType 抓取照片的
     * @param [out] pPicInfo 抓取的图片数据
     * @return 0 成功，非零失败
     */
    int LUX_BASE_CapturePic_HD(LUX_STREAM_CHANNEL_EN picType, LUX_ENCODE_STREAM_ST* pPicInfo);


    /**
     * @description: 释放图片
     * @param [in] IvsResult IVS处理结果
     * @return 0 成功，非零失败
     */
    int LUX_BASE_ReleasePic_HD(LUX_STREAM_CHANNEL_EN picType, LUX_ENCODE_STREAM_ST* pPicInfo);
    /**
     * @description: 抓取图片
     * @param [in] picType 抓取照片的
     * @param [out] pPicInfo 抓取的图片数据
     * @return 0 成功，非零失败
     */
    int LUX_BASE_CapturePic(LUX_STREAM_CHANNEL_EN picType, LUX_ENCODE_STREAM_ST* pPicInfo);

    /**
     * @description: 释放图片
     * @param [in] IvsResult IVS处理结果
     * @return 0 成功，非零失败
     */
    int LUX_BASE_ReleasePic(LUX_STREAM_CHANNEL_EN picType, LUX_ENCODE_STREAM_ST* pPicInfo);



    /******************************************************************
     *                                                                *
     *                         计时器                                  *
     *                                                                *
     ******************************************************************/
     /*计时器绑定回调函数类型*/
    typedef void* (pFuncTimerProcessCB)(void* args);

    /*计时器结构体*/
    typedef struct
    {
        int bTimeStartEnable;           /*计时器开始执行时间间隔使能，开始执行时间间隔指的是多久后再执行process函数*/
        int     timeStartValue;         /*计时器开始执行时间间隔*/
        int     bIntervalEnable;        /*计时器循环使能*/
        unsigned int    timeInterval;   /*计时器循环时间间隔，单位：ms*/
        pFuncTimerProcessCB* pTimerProcess; /*计时器处理函数*/
        void* cbArgs;

    }LUX_BASE_TIMER_PARAM;

    /**
     * @description: 计时器初始化
     * @return 0 成功，非零失败
     */
    int LUX_BASE_TimeInit();

    /**
     * @description: 创建计时器
     * @param [in] pParam 计时器参数
     * @return 成功返回计时器ID, 失败返回-1
     */
    int LUX_BASE_TimerCreate(LUX_BASE_TIMER_PARAM* pParam);

    /**
     * @description: 使能计时器
     * @param [in] timerID 计时器id
     * @return 0 成功，非零失败
     */
    int LUX_BASE_TimerStart(int timerID);

    /**
     * @description: 销毁计时器
     * @param [in] timerID 计时器id
     * @return 0 成功，非零失败
     */
    int LUX_BASE_TimerDestroy(int timerID);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __LUX_BASE_H__ */
