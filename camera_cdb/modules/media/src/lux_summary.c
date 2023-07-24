
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <sys/time.h>
#include <sys/prctl.h>
#include "dirent.h"
#include "sys/stat.h"
#include "lux_summary.h"
#include "math.h"
#define LUX_SUMMARY_MAX_DAYS        7                                       //最大记录天数
#define LUX_SUMMARY_DAY_SECONDS     ( 24*60*60 )                          //一天的秒数
#define LUX_SUMMARY_MAX_TIMES       ( LUX_SUMMARY_MAX_DAYS*LUX_SUMMARY_DAY_SECONDS )       //最大记录秒数

#define LUX_SUMMARU_MAX_STR_SIZE    248                                     //TUYA DP限制的最大字符限制
#define LUX_SUMMARU_MAX_DAY_EVENTS  10                                      //单日最多睡眠次数

typedef struct
{
    UINT_X sleepStartTime;              //睡眠开始时间
    UINT_X sleepStopTime;               //睡眠开始时间
    INT_X sleepLength;                 //睡眠时长
    BOOL_X overDay;                    //是否跨天
}LUX_SUM_SLEEP_INFO;

typedef struct
{
    INT_X day;                          //第几天：0表示今天，1表示昨天...以此类推 
    UINT_X  dayStart;
    INT_X countSleep;                   //记录睡眠次数，达到上限，删除最短
    INT_X totalSleep;                   //总共的睡眠时长
    LUX_SUM_SLEEP_INFO SleepInfo[LUX_SUMMARU_MAX_DAY_EVENTS];
    INT_X countCry;                     //记录哭闹次数
    INT_X countSmile;                   //记录微笑次数
}LUX_SUM_DAY_INFO;


LUX_SUM_DAY_INFO dayInfo[LUX_SUMMARY_MAX_DAYS];
/* 记录事件时间*/
INT_X LUX_SUM_Update_Event(LUX_SUM_EVENT TYPE,UINT_X timeStamp,char *strInfo)
{
    char path[256] = {'\0'};
    char tmp[256] = {'\0'};
    char DateStr[40];
	time_t currTime;
	struct tm *currDate;

    DIR *dir;
    struct dirent *ptr;
    struct stat st;
    //1.删除过期文件
    time(&currTime);
    currTime = (time_t)(currTime - LUX_SUMMARY_MAX_TIMES);
    if(NULL == (dir = opendir(TUYA_EVENTS_TIME_DIR)))
    {
        printf("%s:%d NO SUCH Direct : %s\n",__func__, __LINE__, TUYA_EVENTS_TIME_DIR);
        return -1;
    }
    else
    {
        while ((ptr=readdir(dir)) != NULL)
        {
            if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
                continue;
            else if(ptr->d_type == 8)    ///file
            {
                sprintf(path, "%s%s", TUYA_EVENTS_TIME_DIR,ptr->d_name);
                printf("file name:%s\n",path);
                if(0 == stat(path,&st))
                {
                    printf("st.size = %d\n",(unsigned int)st.st_size);
                    printf("st.st_ctime = %s(%d)\n",ctime(&st.st_ctime),(unsigned int)st.st_ctime);
                    if(st.st_ctime < currTime)
                    {
                        printf("Delate:%s\n",path);
                        remove(path);
                    }
                }
            }
        }
        closedir(dir);
    }
//
//===============================+

    //2.创建写入记录文件
    time(&currTime);
    currTime = currTime + getTimeStampdDiff();  //补足时区差值
    currDate = localtime(&currTime);
    memset(DateStr, 0, 40);
    strftime(DateStr, 40, "%Y_%m_%d", currDate);   /* %H:24小时制 %I:12小时制 */
    sprintf(path, "%s%s.log", TUYA_EVENTS_TIME_DIR,DateStr);
    printf("%s:%d TOTAL PATH : %s\n",__func__, __LINE__, path);

    FILE *fp = fopen(path,"a+");
    if(NULL != fp)
    {
        if(NULL == strInfo)
        {
            sprintf(tmp, "%d_%d\n",TYPE,timeStamp);
        }
        else
        {
            sprintf(tmp, "%d_%d:%s\n",TYPE,timeStamp,strInfo);
        }
        printf(tmp);
        //fseek(fp, 0, SEEK_END);//定位到文件末尾
        fwrite(tmp,strlen(tmp),1,fp);
        fclose(fp);
    }
    return 0;
}


//获取对应日期的O点时间戳
UINT_X lux_getDayStart(UINT_X time)
{
    UINT_X dayStartTime = time - (time % LUX_SUMMARY_DAY_SECONDS) - getTimeStampdDiff();
    char DateStr[40];
    time_t currTime = time - (time % LUX_SUMMARY_DAY_SECONDS);
    struct tm *currDate;
    memset(DateStr, 0, 40);
    currDate = localtime(&currTime);

    strftime(DateStr, 40, "%Y-%m-%d %H:%M:%S", currDate);   /* %H:24小时制 %I:12小时制 */
    printf ("%s:%d   Day time: %d\n",__func__,__LINE__,time);
    printf ("%s:%d   Day StartTime: %d\n",__func__,__LINE__,dayStartTime);
    printf ("%s:%d   Day StartTime: %s\n",__func__,__LINE__,DateStr);
    return dayStartTime;
}

//获取对应日期的事件信息
INT_X LUX_SUM_GetDayInfo(INT_X Day)
{
    char path[256] = {'\0'};
    char tmp[256] = {'\0'};
    char DateStr[40];
	time_t currTime;
	struct tm *currDate;
    LUX_SUM_DAY_INFO info;

    if(Day >= LUX_SUMMARY_MAX_DAYS)
    {
        printf("%s:%d Day value over Limit : %d\n",__func__, __LINE__,LUX_SUMMARY_MAX_DAYS);

    }
    memset(&info, 0 , sizeof(LUX_SUM_DAY_INFO));
    time(&currTime);

    info.dayStart = lux_getDayStart(currTime - Day * LUX_SUMMARY_DAY_SECONDS);
    info.day = Day;

    currTime = (time_t)(currTime + getTimeStampdDiff() - Day * LUX_SUMMARY_DAY_SECONDS);
    currDate = localtime(&currTime);
    memset(DateStr, 0, 40);
    strftime(DateStr, 40, "%Y_%m_%d", currDate);   /* %H:24小时制 %I:12小时制 */
    sprintf(path, "%s%s.log", TUYA_EVENTS_TIME_DIR,DateStr);
    printf("%s:%d TOTAL PATH : %s\n",__func__, __LINE__, path);

    time(&currTime);

    //第一遍统计除睡眠以外的次数
    FILE *fp = fopen(path,"r+");
    if(NULL == fp)
    {
        printf("%s:%d No such file or busy!!!\n",__func__,__LINE__);
        memcpy(&dayInfo[Day],&info,sizeof(LUX_SUM_DAY_INFO));
        return -1;
    }
    while ( NULL != fgets(tmp,sizeof(tmp),fp))
    {
        INT_X type;
        char tTmp[256] = {'\0'};
        sscanf(tmp, "%d_%s",&type,tTmp);
        switch (type)
        {
            case LUX_SUM_EVENT_BABY_CRY:
                info.countCry++;
                break;
            case LUX_SUM_EVENT_SMILE_DET:
                info.countSmile++;
                break;
            default:
                break;
        }
    }
    fclose(fp);

    //第二遍统计睡眠
    int countSleep = 0;
    LUX_SUM_SLEEP_INFO tmpSleep = 
    {
        .overDay = 0,
        .sleepLength = 0,
        .sleepStartTime = 0,
        .sleepStopTime = 0
    };
    fp = fopen(path,"r+");
    if(NULL == fp)
    {
        printf("%s:%d No such file or busy!!!\n",__func__,__LINE__);
        return -1;
    }
    while ( NULL != fgets(tmp,sizeof(tmp),fp))
    {
        printf(tmp);
        INT_X type;
        UINT_X eventTime;
        INT_X sleepLen;
        char tTmp[256] = {'\0'};
        sscanf(tmp, "%d_%s",&type,tTmp);

        if(LUX_SUM_EVENT_SLEEP_STOP == type)
        {
            sscanf(tTmp, "%d:%d",&eventTime,&sleepLen);
        }
        else if(LUX_SUM_EVENT_SLEEP_START == type)
        {
            sscanf(tTmp, "%d",&eventTime);
        }
        else
        {
            continue;   //过滤非睡眠记录
        }
        if((0 == countSleep) && (LUX_SUM_EVENT_SLEEP_STOP == type))
        {
            UINT_X tmpstartTime = eventTime - sleepLen;
            if(tmpstartTime < info.dayStart)
            {
                tmpSleep.overDay = 1;
                tmpSleep.sleepStartTime = info.dayStart;
                tmpSleep.sleepStopTime = eventTime;
                tmpSleep.sleepLength = eventTime - info.dayStart;    //0点开始
            }
            else
            {
                tmpSleep.overDay = 0;
                tmpSleep.sleepStartTime = tmpstartTime;
                tmpSleep.sleepStopTime = eventTime;
                tmpSleep.sleepLength = sleepLen;
            }
        }
        else
        {
            if(LUX_SUM_EVENT_SLEEP_START == type)
            {
                tmpSleep.sleepStartTime = eventTime;
                eventTime = 0;
                long offset = ftell(fp);
                BOOL_X findEnd = LUX_FALSE;
                if ( NULL != fgets(tmp,sizeof(tmp),fp))
                {
                    do
                    {  
                        printf(tmp);
                        sscanf(tmp, "%d_%s",&type,tTmp);
                        if(LUX_SUM_EVENT_SLEEP_STOP == type)
                        {
                            findEnd = 1;
                            sscanf(tTmp, "%d:%d",&eventTime,&sleepLen);
                            break;
                        }
                        else if (LUX_SUM_EVENT_SLEEP_KEEP == type)
                        {
                            sscanf(tTmp, "%d",&eventTime);
                        }
                        else if  (LUX_SUM_EVENT_SLEEP_START == type)
                        {
                            fseek(fp,offset,SEEK_SET);
                            break;
                        }
                        offset = ftell(fp);
                    }while ( NULL != fgets(tmp,sizeof(tmp),fp));
                }
                if(findEnd == LUX_TRUE)
                {
                    int delta = (int)(eventTime - sleepLen) - (int)tmpSleep.sleepStartTime;
                    if(( delta > 300))   //差值大于5分钟，只补5分钟
                    {
                        sleepLen = sleepLen + 300;
                    }
                    //使用 End 和Length 重新计算开始时间
                    tmpSleep.sleepStopTime = eventTime;
                    tmpSleep.sleepLength = sleepLen;
                    tmpSleep.sleepStartTime = tmpSleep.sleepStopTime - tmpSleep.sleepLength;
                }
                else //找不到SleepEnd
                {
                    if(eventTime != 0)
                    {
                    tmpSleep.overDay = 0;
                    tmpSleep.sleepStopTime = eventTime;
                    tmpSleep.sleepLength = tmpSleep.sleepStopTime - tmpSleep.sleepStartTime;
                    }
                    else
                    {
                        tmpSleep.sleepLength = 0;
                    }
                }
            }
        }
        if(tmpSleep.sleepLength == 0)   //有STart 无STOP 或 KEEP的 睡眠 跳过
        {
            //do nothing;
        }
        else if(countSleep < LUX_SUMMARU_MAX_DAY_EVENTS)
        {
            info.SleepInfo[countSleep].overDay = tmpSleep.overDay;
            info.SleepInfo[countSleep].sleepStartTime = tmpSleep.sleepStartTime;
            info.SleepInfo[countSleep].sleepStopTime = tmpSleep.sleepStopTime;
            info.SleepInfo[countSleep].sleepLength = tmpSleep.sleepLength;
            countSleep++;
        }
        else   //替换最短的睡眠时间
        {
            int i = 0;
            int minLen = tmpSleep.sleepLength;
            int minIdx = LUX_SUMMARU_MAX_DAY_EVENTS;
            for(i = 0; i < LUX_SUMMARU_MAX_DAY_EVENTS ; i++)
            {
                if(minLen >info.SleepInfo[i].sleepLength)
                {
                    minLen = info.SleepInfo[i].sleepLength;
                    minIdx = i;
                }
            }
            if(minIdx <LUX_SUMMARU_MAX_DAY_EVENTS)
            {
                info.SleepInfo[minIdx].overDay = tmpSleep.overDay;
                info.SleepInfo[minIdx].sleepLength = tmpSleep.sleepLength;
                info.SleepInfo[minIdx].sleepStartTime = tmpSleep.sleepStartTime;
                info.SleepInfo[minIdx].sleepStopTime = tmpSleep.sleepStopTime;
            }
        }
        memset(&tmpSleep, 0, sizeof(tmpSleep));
    }
    fclose(fp);
    int i = 0;

    info.countSleep = countSleep;
    if(info.countSleep >= LUX_SUMMARU_MAX_DAY_EVENTS ) info.countSleep = LUX_SUMMARU_MAX_DAY_EVENTS;
    printf("%s:%d countSleep %d\n",__func__,__LINE__,info.countSleep);
    for(i = 0 ; i < info.countSleep; i++)
    {
        printf("%s:%d sleepLength %d\n",__func__,__LINE__,info.SleepInfo[i].sleepLength);
        info.totalSleep = info.totalSleep + info.SleepInfo[i].sleepLength;
        printf("%s:%d totalSleep %d\n",__func__,__LINE__,info.totalSleep);
    }
    memcpy(&dayInfo[Day],&info,sizeof(LUX_SUM_DAY_INFO));
    return 0;
}


//A类指令处理函数
//可用指令A0,A1,A2,A3,A4,A5,A6,A7,AA
//RET:
INT_X LUX_SUM_GetInfoStringA(CHAR_X *cmd ,CHAR_X *retStr)
{
    UINT_X nowTime = gettimeStampS();       //当前时间戳 //(gettimeStampS() % LUX_SUMMARY_DAY_SECONDS);       //今天的第几秒
    INT_X totalCry = 0;                     //记录哭闹次数
    INT_X totalSmile = 0;                   //记录微笑次数
    INT_X totalSleep = 0;                   //总共的睡眠时长 分钟

    memset(retStr,0,LUX_SUMMARU_MAX_STR_SIZE);
    CHAR_X strForLog[1024];
    memset(strForLog,0,1024);
    
    if(cmd[1] == 'A')
    {
        int i = 0;
        for(i = 0 ; i < LUX_SUMMARY_MAX_DAYS; i++)   //前N天的全部数据 //不含今天
        {
            LUX_SUM_GetDayInfo(i);
        }
        for(i = 0 ; i < LUX_SUMMARY_MAX_DAYS ; i++)
        {
            totalCry = totalCry + dayInfo[i].countCry;
            totalSmile = totalSmile + dayInfo[i].countSmile;
            totalSleep = totalSleep + dayInfo[i].totalSleep;
        }
        sprintf(retStr,"%X,%s,%X,%X,%X",nowTime,cmd,totalCry,totalSmile,totalSleep);
        sprintf(strForLog,"nowTime%d,cmd%s,totalCry%d,totalSmile%d,totalSleep%d",nowTime,cmd,totalCry,totalSmile,totalSleep);
        for(i = 0 ; i < LUX_SUMMARY_MAX_DAYS; i++) 
        {
            char tmp[24] ={'\0'};
            sprintf(tmp,",%X",dayInfo[i].totalSleep);
            if(strlen(retStr) + strlen(tmp) < LUX_SUMMARU_MAX_STR_SIZE)
            {
                strcat(retStr,tmp);
            }
            char tmp2[48] ={'\0'};
            sprintf(tmp2,",Day%d:%d",i,dayInfo[i].totalSleep);
            strcat(strForLog,tmp2);
        }
    }
    else
    {
        int i = 0;
        int idx = (int)(cmd[1]-'0');
        LUX_SUM_GetDayInfo(idx);
        
        UINT_X dayStart = dayInfo[idx].dayStart;
        totalCry = dayInfo[idx].countCry;
        totalSmile = dayInfo[idx].countSmile;
        totalSleep = dayInfo[idx].totalSleep;
        sprintf(retStr,"%X,%s,%X,%X,%X,%X",nowTime,cmd,dayStart,totalCry,totalSmile,totalSleep);
        sprintf(strForLog,"nowTime%d,cmd%s,totalCry%d,totalSmile%d,totalSleep%d",nowTime,cmd,totalCry,totalSmile,totalSleep);
        for(i = 0 ; i < dayInfo[idx].countSleep; i++)       //记录全部时间
        {
            char tmp[24] ={'\0'};
            sprintf(tmp,",%X_%X",(dayInfo[idx].SleepInfo[i].sleepStartTime - dayInfo[idx].dayStart),dayInfo[idx].SleepInfo[i].sleepLength);
            if(strlen(retStr) + strlen(tmp) < LUX_SUMMARU_MAX_STR_SIZE)
            {
                strcat(retStr,tmp);
            }
            else
            {
                printf("%s:%d  OVER LEN :%d\n",__func__,__LINE__,LUX_SUMMARU_MAX_STR_SIZE);
            }
            char tmp2[48] ={'\0'};
            sprintf(tmp2,",%d_%d",(dayInfo[idx].SleepInfo[i].sleepStartTime - dayInfo[idx].dayStart),dayInfo[idx].SleepInfo[i].sleepLength);
            strcat(strForLog,tmp2);
        }
    }
    printf("%s:%d  retStr:%s\n",__func__,__LINE__,retStr);
    printf("%s:%d  strForLog:%s\n",__func__,__LINE__,strForLog);

    return 0;
}
//获取数据整理字符
//可用指令A0,A1,A2,A3,A4,A5,A6,A7,AA
//RET:
INT_X LUX_SUM_DP_CMD(CHAR_X *cmd,CHAR_X *tuyaStr)
{
    CHAR_X retStr[LUX_SUMMARU_MAX_STR_SIZE];
    memset(retStr,0,LUX_SUMMARU_MAX_STR_SIZE);
    switch (cmd[0])
    {
    case 'A':
        {
            LUX_SUM_GetInfoStringA(cmd, retStr);
        }
        break;
    
    default:
        printf("%s:%dunknow type",__func__,__LINE__);
        break;
    }
    strcpy(tuyaStr,retStr);
    return 0;
}

