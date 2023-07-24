#include "lux_record.h"
#include "lux_global.h"

#include <sys/statvfs.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <jwhead.h>
#include <setjmp.h>
#include <signal.h>

int ftruncateEx(int fd, off_t length)
{
	int ret = -1;
	int i = 0;

	for (i = 0; i < 5; i++)
	{
		if (ftruncate(fd, length) == 0)
		{
			ret = 0;
			break;
		}
		else
		{
			MYERROR("ftruncate occur error:[%d][%s], try ftruncate again \n", errno, strerror(errno));
			ret = -1;
		}

		usleep(10*1000);
	}

	return ret;
}

static int mkdirEx(const char* dirName)
{
	int i = 0;
	int ret = -1;
	do
	{
		ret = mkdir(dirName, 0777);
		if (ret != 0)
		{
			MYERROR("mkdir:[%s] failed, err:[%d][%s] \n", dirName, errno, strerror(errno));
			usleep(10*1000);
		}
		else
		{
			break;
		}

	} while (i++ < 3);

	return ret;
}

static ssize_t writeEx(int fd, const void* buf, size_t count)
{
	ssize_t writeCntOnce = 0;
	size_t leftBytes = count;
	do
	{
		writeCntOnce = write(fd, buf+(count-leftBytes), leftBytes);
		
		if (-1 == writeCntOnce)
		{
			MYERROR("write file[fd:%d bytes:%lu] occur error:[%d][%s] \n", fd, leftBytes, errno, strerror(errno));
			break;
		}
		else if (0 == writeCntOnce)
		{
			MYWARN("write file[fd:%d bytes:%lu] return 0 \n", fd, leftBytes);
			//???
		}
		else if (writeCntOnce > 0 && writeCntOnce <= leftBytes)
		{
			leftBytes -= writeCntOnce;
		}
		else
		{
			MYERROR("write file[fd:%d bytes:%lu] return %ld \n", fd, leftBytes, writeCntOnce);
			break;
		}
		
	} while(leftBytes != 0);

	return (count - leftBytes);
}

static int stat_space(const char* dir, uint64_t* freeSpace, uint64_t* totalSpace)
{
	struct statvfs sdinfo;
	memset(&sdinfo, 0x00, sizeof(struct statvfs));
	int ret = statvfs(dir, &sdinfo);
	if (0 == ret)
	{
		uint64_t block_size = sdinfo.f_bsize;
		*totalSpace = sdinfo.f_blocks * block_size;
		*freeSpace = sdinfo.f_bfree * block_size;
		return 0;
	}
	else
	{
		MYERROR("statvfs error:[%d][%s]\n", errno, strerror(errno));
		return -1;
	}
}

E_SD_STATUS LUX_ipc_sd_get_status(VOID)
{
	FILE* fp = fopen(LINUX_SD_DEV_FILE, "rb");
	if (!fp)
	{
		return SD_STATUS_NOT_EXIST;
	}
	fclose(fp);

	strcpy(s_mmcblk_name_p1, LINUX_SD_DEV_FILE);
	if (0 == access(LINUX_SD_DEV_FILE"p1", F_OK)) //Default node name information
	{
		strcat(s_mmcblk_name_p1, "p1");
	}

	fp = fopen(LINUX_MOUNT_INFO_FILE, "rb");
	if (fp)
	{
		memset(s_mounts_info, 0, sizeof(s_mounts_info));
		fread(s_mounts_info, 1, MAX_MOUNTS_INFO_LEN, fp);
		fclose(fp);
		char* mmcblk_name_start = strstr(s_mounts_info, LINUX_SD_DEV_FILE "p1"); //Confirm the final node name in Mount information
		char* mmcblk_name_end = strstr(s_mounts_info, SD_MOUNT_PATH);
		if (mmcblk_name_start && mmcblk_name_end)
		{
			int mmcblk_name_len = mmcblk_name_end - mmcblk_name_start;
			if (mmcblk_name_len >= MAX_MMC_NAME_LEN)
			{
				return SD_STATUS_ABNORMAL;
			}
			strncpy(s_mmcblk_name_p1, mmcblk_name_start, mmcblk_name_len);
			s_mmcblk_name_p1[mmcblk_name_len] = '\0';
			if (isformatted == 1)
			{
				isformatted = 0;
			}
		}
		//There are device nodes but no mount information. Generally, the card format is incorrect and report abnormal.
		else
		{
			if (isformatted == 0)
			{
				isformatted = 1;
				//tuya_ipc_sd_format();     /* 避免客户数据被删除，暂不启用 */
			}
			return SD_STATUS_ABNORMAL;
		}
		//If the mount information of the SD card is not at the end and there is a ro mount behind it, there will be a problem.
		if (NULL != strstr(mmcblk_name_start, "ro,"))
		{
			return SD_STATUS_NORMAL; /* warning:此处报错修正为正常，解决挂载remount-ro问题，经测试读写正常 */
		}
		if (NULL == strstr(mmcblk_name_start, "vfat"))
		{
			return SD_STATUS_ABNORMAL;
		}
		if (access(LINUX_SD_DEV_FILE "p1", 0))
		{
			return SD_STATUS_ABNORMAL;
		}

		return SD_STATUS_NORMAL;
	}
	else
	{
		return SD_STATUS_UNKNOWN;
	}
}

void LUX_ipc_sd_format(void)
{
	char format_cmd[256] = { 0 };
	char buffer[512] = { 0 };
	MYWARN("sd format begin\n");
	snprintf(format_cmd, 256, "fuser -m %s;umount %s;mkfs.vfat -n %s %s -s 1024;mkdir -p %s;mount -t auto %s %s;",
			 SD_MOUNT_PATH, s_mmcblk_name_p1, SD_MOUNT_PATH, s_mmcblk_name_p1, SD_MOUNT_PATH, s_mmcblk_name_p1, SD_MOUNT_PATH);
	MYTRACE("execute: [%s]\n", format_cmd);
	FILE* pp = popen(format_cmd, "r");
	if (NULL != pp)
	{
		fgets(buffer, sizeof(buffer), pp);
		printf("%s\n", buffer);
		pclose(pp);
	}
	else
	{
		MYERROR("format_sd_card failed\n");
	}
	MYWARN("sd format end\n");
}

void LUX_ipc_sd_remount(void)
{
	char format_cmd[128] = { 0 };
	char buffer[512] = { 0 };
	E_SD_STATUS status = SD_STATUS_UNKNOWN;

	status = LUX_ipc_sd_get_status();
	if (SD_STATUS_NORMAL == status)
	{
		MYWARN("sd don't need to remount!\n");
		return;
	}
	MYTRACE("remount_sd_card ..... \n");

	snprintf(format_cmd, 128, "umount %s;sleep 1;mount -t auto %s %s -o rw,errors=continue;", s_mmcblk_name_p1, s_mmcblk_name_p1, SD_MOUNT_PATH);
	FILE* pp = popen(format_cmd, "r");
	if (NULL != pp)
	{
		fgets(buffer, sizeof(buffer), pp);
		printf("%s\n", buffer);
		pclose(pp);
	}
	else
	{
		MYERROR("remount_sd_card failed\n");
	}
}

int bubble_sort(int data[], int num)
{
	int n = num;
	int m = n;            //因为每进行一次第一轮循环，需要排序的数据都要“--”，因此定义变量m=n；
	int i, j;
	int temp;
	for (i = 0; i < n; i++) {
		int exchange = 0;           //记录这一轮会不会有数据的交换；
		for (j = 0; j < m - 1; j++) {
			if (data[j] > data[j + 1]) {
				temp = data[j];
				data[j] = data[j + 1];
				data[j + 1] = temp;
				exchange = 1;
			}
		}
		m--;
		if (!exchange)  //若没有数据的交换，则数据已经排列完毕，跳出循环；
			break;
	}
}

uint64_t get_sdcard_free_space()
{
	uint64_t freespace = 0;
	pthread_mutex_lock(g_lux_record->freespace_mutex);
	freespace = g_lux_record->free_space;
	pthread_mutex_unlock(g_lux_record->freespace_mutex);

	return freespace;
}

int set_sdcard_free_space(uint64_t val)
{
	pthread_mutex_lock(g_lux_record->freespace_mutex);
	g_lux_record->free_space = val;
	pthread_mutex_unlock(g_lux_record->freespace_mutex);

	return 0;
}

int LUX_Record_Init()
{
	printf("enter LUX_Record_Init() \n");
	int ret;

#if 0  //only for test sdcard loop delete
	printf("test write record file by year/month/day/filename \n");
	int year, month, day;
	char dir[128];
	char* basePath = "/system/sdcard/DCIM";
	mkdir(basePath, 0777);
	char pbuf[10*1024];
	int exit_loop = 0;
	for (year = 1920; year < 2020; year++)
	{
		for (month = 1; month <= 12; month++)
		{
			for (day = 1; day <= 31; day++)
			{
				sprintf(dir, "%s/%04d", basePath, year);
				ret = mkdir(dir, 0777);

				sprintf(dir, "%s/%04d/%02d", basePath, year, month);
				ret = mkdir(dir, 0777);

				sprintf(dir, "%s/%04d/%02d/%02d", basePath, year, month, day);
				ret = mkdir(dir, 0777);

				sprintf(dir, "%s/%04d/%02d/%02d/%04d%02d%02d000001.mp4", basePath, year, month, day, year, month, day);

				if (access(dir, 0) != 0)
				{
					int fp1 = open(dir, O_RDWR | O_CREAT | O_SYNC, S_IWUSR | S_IRUSR | S_IWGRP | S_IRGRP);
					int k;
					for (k = 0; k < 200; k++)
					{
						write(fp1, pbuf, 10 * 1024);
						usleep(5000);
					}
					//flush(fp1);
					close(fp1);

					usleep(10*1000);

					printf("create file[%s] success! \n", dir);

					uint64_t total_size, free_size;
					ret = stat_space(basePath, &free_size, &total_size);
					printf("free space[MB]:%llu\n", free_size / 1024 / 1024);

					if (free_size < 6 * 1024 * 1024)
					{
						exit_loop = 1;
						break;
					}
				}
			}

			if (exit_loop) break;
		}

		if (exit_loop) break;
	}
#endif
	//mount sd card
	LUX_ipc_sd_remount();

	//初始化 lux_record结构体
	memset(g_lux_record, 0, sizeof(lux_record_t));
	sprintf(g_lux_record->basePath_DCIM, "%s/%s", SD_MOUNT_PATH, "DCIM");
	sprintf(g_lux_record->origin_file_path, "%s/%s/%s", SD_MOUNT_PATH, "DCIM", ORIGIN_RECORD_FILE_NAME);
	g_lux_record->stop_loop_delete_thread = 0;
	g_lux_record->stop_record_thread = 0;
	g_lux_record->recordFileMaxByteNum = 30 * 1024 * 1024;
	g_lux_record->originFileMaxByteNum = g_lux_record->recordFileMaxByteNum + 1.5 * 1024 * 1024;
	g_lux_record->need_re_stat_space = 0;
	g_lux_record->data_len = 0;
	g_lux_record->fpInfo = -1;
	g_lux_record->fpRecord = -1;

	g_lux_record->mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(g_lux_record->mutex, NULL);

	g_lux_record->playback_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(g_lux_record->playback_mutex,NULL);

	g_lux_record->freespace_mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(g_lux_record->freespace_mutex, NULL);

	g_lux_record->cur_record_info = (record_info_t*)malloc(sizeof(record_info_t));

	Semaphore_Create(0, &g_lux_record->sem_create_origin_file);

	g_lux_record->frame_buf = (unsigned char*)malloc(RECORD_BUF_MAX_SIZE);
	memset(g_lux_record->frame_buf, 0, RECORD_BUF_MAX_SIZE);

	g_lux_record->playback_framebuf = (unsigned char*)malloc(PLAYBACK_BUF_SIZE);
	memset(g_lux_record->playback_framebuf, 0, PLAYBACK_BUF_SIZE);

	//获取SD卡的状态
	if (LUX_ipc_sd_get_status() != SD_STATUS_NORMAL)
	{
		g_lux_record->is_running = 0;
		MYERRORING("sd card status is not normal! \n");
		return -1;
	}
	else
	{
		g_lux_record->is_running = 1;
	}
	
	//创建DCIM目录
//if (strlen(g_lux_record->basePath) > 0 && g_lux_record->basePath[strlen(g_lux_record->basePath) - 1] == '/')
//{
//	g_lux_record->basePath[strlen(g_lux_record->basePath) - 1] = '\0';
//}
//strcat(g_lux_record->basePath, "/DCIM");

	if (access(g_lux_record->basePath_DCIM, 0) != 0)
	{
		int ret2 = mkdir(g_lux_record->basePath_DCIM, 0777);
		if (ret2 == 0)
		{
			printf("create directory success:[%s] \n", g_lux_record->basePath_DCIM);
			g_lux_record->is_running = 1;
		}
		else
		{
			printf("create directory failed:[%s], ret:[%d] \n", g_lux_record->basePath_DCIM, ret2);
			g_lux_record->is_running = 0;
		}
	}
	else
	{
		printf("directory is exist:[%s] \n", g_lux_record->basePath_DCIM);
	}

	//获取SD卡的剩余空间
	if (access(g_lux_record->basePath_DCIM, 0) == 0)
	{
		uint64_t total_size, free_size;
		ret = stat_space(g_lux_record->basePath_DCIM, &free_size, &total_size);
		if (0 == ret)
		{
			g_lux_record->stat_space_time_s = time_ms()/1000;
			//设置剩余空间
			set_sdcard_free_space(free_size);
			MYTRACE("sd card free space(MB):%llu, total space(MB):%llu \n", free_size / (1024 * 1024L), total_size / (1024 * 1024L));
		}
		else
		{
			MYERROR("error: read sd card failed, statvfs ret:%d \n ", ret);
		}
	}
	else
	{
		set_sdcard_free_space(0);
	}

	return 0;
}

static void* create_origin_file_threadfunc(void* p)
{
	lux_record_t* pLuxRecord = (lux_record_t*)p;

	char tempFile[72] = { 0 };

	sprintf(tempFile, "%s_%s", pLuxRecord->origin_file_path, "temp");

	do
	{
		Semaphore_Pend(&pLuxRecord->sem_create_origin_file, SEM_FOREVER);

		int fd1 = open(tempFile, O_RDWR | O_CREAT);
		if (fd1 > 0)
		{
			ftruncateEx(fd1, pLuxRecord->originFileMaxByteNum);
			fsync(fd1);
			close(fd1);
			rename(tempFile, pLuxRecord->origin_file_path);
			MYTRACE("create origin record file[%s] success! \n", pLuxRecord->origin_file_path);
		}
		else
		{
			MYERROR("initialize origin record file fialed,error:[%d][%s] \n", errno, strerror(errno));
		}

	} while (1);
}

static void* record_threadfunc(void* p)
{
	sleep(4);

	int ret;
	char dir[128];
	char recordInfoFileName[64];
	record_info_t recordInfoBlank;

	lux_record_t* pLuxRecord = (lux_record_t*)p;

	//初始化录像文件
	int fd1 = open(pLuxRecord->origin_file_path, O_RDWR | O_CREAT);
	if (fd1 > 0)
	{
		ftruncateEx(fd1, pLuxRecord->originFileMaxByteNum);
		fsync(fd1);
		close(fd1);
		MYTRACE("create origin record file[%s] success! \n", pLuxRecord->origin_file_path);
	}
	else
	{
		MYERROR("initialize origin record file fialed,error:[%d][%s] \n", errno, strerror(errno));
	}

	struct tm cur_tm;
	struct stat myFileStat;

	do {
		if (0 == pLuxRecord->is_running)
		{
			if (pLuxRecord->fpInfo)
			{
				fsync(pLuxRecord->fpInfo);
				close(pLuxRecord->fpInfo);
				pLuxRecord->fpInfo = -1;
			}

			if (pLuxRecord->fpRecord)
			{
				fsync(pLuxRecord->fpRecord);
				close(pLuxRecord->fpRecord);
				pLuxRecord->fpRecord = -1;
			}

			do
			{
				usleep(1000 * 1000);

				printf("check sdcard loop... ... \n");
				if (SD_STATUS_NORMAL == LUX_ipc_sd_get_status())
				{
                    g_lux_record->is_running = 1;

					//需要执行初始化SD，创建原始文件的步骤
					
					//获取SD卡的剩余空间
					uint64_t total_size, free_size;
					ret = stat_space(pLuxRecord->basePath_DCIM, &free_size, &total_size);
					if (0 == ret)
					{
						pLuxRecord->stat_space_time_s = time_ms()/1000;
						//设置剩余空间
						set_sdcard_free_space(free_size);
						MYTRACE("sd card free space(MB):%llu, total space(MB):%llu \n", free_size / (1024 * 1024L), total_size / (1024 * 1024L));
					}
					else
					{
						MYERROR("error: read sd card failed, statvfs ret:%d \n ", ret);
					}

					//创建DCIM目录
					//if (strlen(pLuxRecord->basePath) > 0 && pLuxRecord->basePath[strlen(pLuxRecord->basePath) - 1] == '/')
					//{
					//	pLuxRecord->basePath[strlen(pLuxRecord->basePath) - 1] = '\0';
					//}
					//strcat(pLuxRecord->basePath, "/DCIM");

					if (access(pLuxRecord->basePath_DCIM, 0) != 0)
					{
						int ret2 = mkdir(pLuxRecord->basePath_DCIM, 0777);
						if (ret2 == 0)
						{
							MYTRACE("create directory success:[%s] \n", pLuxRecord->basePath_DCIM);
							//pLuxRecord->is_running = 1;
						}
						else
						{
							MYERROR("create directory failed:[%s], ret:[%d] \n", pLuxRecord->basePath_DCIM, ret2);
							//pLuxRecord->is_running = 0;
						}
					}
					else
					{
						MYWARN("directory is exist:[%s] \n", pLuxRecord->basePath_DCIM);
					}

					if (access(pLuxRecord->basePath_DCIM, 0) == 0)
					{
						usleep(1000*1000);

						//检查文件大小
						if (stat(pLuxRecord->origin_file_path, &myFileStat) != 0 || myFileStat.st_size < pLuxRecord->originFileMaxByteNum)
						{
							//初始化空白录像文件
							int fd1 = open(pLuxRecord->origin_file_path, O_RDWR | O_CREAT);
							if (fd1 > 0)
							{
								ftruncateEx(fd1, pLuxRecord->originFileMaxByteNum);
								fsync(fd1);
								close(fd1);
								MYTRACE("create origin record file[%s] success! \n", pLuxRecord->origin_file_path);
							}
							else
							{
								MYERROR("initialize origin record file fialed,error:[%d][%s] \n", errno, strerror(errno));
							}
						}
						
						pLuxRecord->is_running = 1;
					}
					else
					{
						pLuxRecord->is_running = 0;
					}
				}
                else
                {
                    g_lux_record->is_running = 0;
                   
                }

			} while (0 == pLuxRecord->is_running);
		}

		int buf_data_len = 0;
		uint64_t packTimeStamp_ms = 0L;
		pthread_mutex_lock(pLuxRecord->mutex);
		buf_data_len = pLuxRecord->data_len;
		frame_head_t *pack_tmp1 = (frame_head_t*)pLuxRecord->frame_buf;
		time_t mytime = pack_tmp1->timestamp_ms / 1000;
		cur_tm = *gmtime(&mytime);
		packTimeStamp_ms = pack_tmp1->timestamp_ms;
		pthread_mutex_unlock(pLuxRecord->mutex);

		//检查缓冲区的数据长度
		if (buf_data_len <= sizeof(frame_head_t))
		{
			usleep(1 * 1000);
			continue;
		}

		//创建年份目录
		sprintf(dir, "%s/%d", pLuxRecord->basePath_DCIM, cur_tm.tm_year + 1900);
		if (access(dir, 0) != 0)
		{
			ret = mkdirEx(dir);
			if (ret != 0)
			{
				printf("mkdir[%s] error, ret:%d\n", dir, ret);
				pLuxRecord->is_running = 0;
				continue;
			}
		}

		//创建月份目录
		sprintf(dir, "%s/%d/%02d", pLuxRecord->basePath_DCIM, cur_tm.tm_year + 1900, cur_tm.tm_mon + 1);
		if (access(dir, 0) != 0)
		{
			ret = mkdirEx(dir);
			if (ret != 0)
			{
				printf("mkdir[%s] error, ret:%d\n", dir, ret);
				pLuxRecord->is_running = 0;
				continue;
			}
		}

		sprintf(dir, "%s/%d/%02d/%02d", pLuxRecord->basePath_DCIM, cur_tm.tm_year + 1900, cur_tm.tm_mon + 1, cur_tm.tm_mday);
				
		sprintf(recordInfoFileName, "%s/%04d/%02d/%02d/record.info", pLuxRecord->basePath_DCIM, cur_tm.tm_year + 1900, cur_tm.tm_mon + 1, cur_tm.tm_mday);

		//创建日目录
		if (access(dir, 0) != 0)
		{
			ret = mkdirEx(dir);
			if (ret != 0)
			{
				printf("mkdir[%s] error, ret:[%d][%s]\n", dir, errno, strerror(errno));
				pLuxRecord->is_running = 0;
				continue;
			}

			//关闭已经有的指针
			if (pLuxRecord->fpInfo != -1)
			{
				MYTRACE("close record info file id[%d] \n", pLuxRecord->fpInfo);
				fsync(pLuxRecord->fpInfo);
				close(pLuxRecord->fpInfo);
				pLuxRecord->fpInfo = -1;
			}		
			
			if (access(recordInfoFileName, 0) == 0)
			{
				remove(recordInfoFileName);
			}						
		}

		//初始化recordinfo文件
		if (pLuxRecord->fpInfo == -1)
		{
			if (access(recordInfoFileName, 0) == 0)
			{				
				pLuxRecord->fpInfo = open(recordInfoFileName, O_RDWR);
				if (pLuxRecord->fpInfo == -1)
				{
					MYERROR("open file[%s] fd:[%d] error:[%d][%s]\n", recordInfoFileName, pLuxRecord->fpInfo, errno, strerror(errno));
					pLuxRecord->is_running = 0;
					continue;
				}
			}
			else
			{
				MYTRACE("create file[%s] \n", recordInfoFileName);
				pLuxRecord->fpInfo = open(recordInfoFileName, O_RDWR|O_CREAT);

				if (pLuxRecord->fpInfo == -1)
				{
					MYERROR("create file[%s] error:[%d][%s]\n", recordInfoFileName, errno, strerror(errno));
				}

				if (pLuxRecord->fpInfo > 0)
				{
					//创建record.info文件 初始化1000个节点
#if 0
					int recordNodeNum = 0;
					int recordNodeIdx = 0;

					memset(&recordInfoBlank, 0, sizeof(record_info_t));
					//写入节点个数
					writeEx(pLuxRecord->fpInfo, &recordNodeNum, sizeof(int));
					//写入空节点
					for (recordNodeIdx = 0; recordNodeIdx < 1000; recordNodeIdx++)
					{
						writeEx(pLuxRecord->fpInfo, &recordInfoBlank,  sizeof(record_info_t));
					}
#else
					if (ftruncateEx(pLuxRecord->fpInfo, 1000 * sizeof(record_info_t) + sizeof(int)) != 0)
					{
						MYERROR("ftruncateEx return error \n");
						pLuxRecord->is_running = 0;
						continue;
					}
#endif
					uint64_t curFreeSpace = get_sdcard_free_space();
					curFreeSpace -= (1000 * sizeof(record_info_t) + sizeof(int));
					set_sdcard_free_space(curFreeSpace);
				}
			}
		}		

		if (pLuxRecord->fpRecord == -1)
		{
			sprintf(dir, "%s/%d/%02d/%02d/%04d%02d%02d%02d%02d%02d.mp4", pLuxRecord->basePath_DCIM, cur_tm.tm_year + 1900, cur_tm.tm_mon + 1, cur_tm.tm_mday,
				cur_tm.tm_year + 1900, cur_tm.tm_mon + 1, cur_tm.tm_mday, cur_tm.tm_hour, cur_tm.tm_min, cur_tm.tm_sec);

			if (access(pLuxRecord->origin_file_path, 0) == 0)
			{
				MYTRACE("origin record file is already exists, rename[%s] \n", dir);

				//检查文件大小
				if (stat(pLuxRecord->origin_file_path, &myFileStat) != 0 || myFileStat.st_size < pLuxRecord->originFileMaxByteNum)
				{
					remove(pLuxRecord->origin_file_path);

					//初始化录像文件
					MYWARN("origin record file is invalid, create new record file:[%s] \n", dir);
					Semaphore_Post(&pLuxRecord->sem_create_origin_file);
					pLuxRecord->fpRecord = open(dir, O_WRONLY | O_CREAT);
				}
				else
				{
					rename(pLuxRecord->origin_file_path, dir);
					usleep(1 * 1000);
					pLuxRecord->fpRecord = open(dir, O_WRONLY);

					Semaphore_Post(&pLuxRecord->sem_create_origin_file);
				}
			}
			else
			{
				MYWARN("origin record file is not exist, create new record file:[%s] \n", dir);
				Semaphore_Post(&pLuxRecord->sem_create_origin_file);
				pLuxRecord->fpRecord = open(dir, O_WRONLY | O_CREAT);
			}

			if (pLuxRecord->fpRecord == -1)
			{
				MYERROR("open file error:[%d][%s]\n", errno, strerror(errno));
				pLuxRecord->cur_file_path[0] = '\0';
				pLuxRecord->cur_record_info->file_name[0] = '\0';
			}
			else {
				//检查文件是否撑大
				if (lseek(pLuxRecord->fpRecord, 0, SEEK_END) <= 0)
				{
					MYTRACE("truncate file to size:[%llu] \n", pLuxRecord->originFileMaxByteNum);
					lseek(pLuxRecord->fpRecord, 0, SEEK_SET);
					if (ftruncateEx(pLuxRecord->fpRecord, pLuxRecord->originFileMaxByteNum) != 0)
					{
						MYERROR("ftruncateEx return error \n");
					}
				}			

				//文件指针复位
				lseek(pLuxRecord->fpRecord, 0, SEEK_SET);

				sprintf(pLuxRecord->cur_file_path, "%s/%d/%02d/%02d", pLuxRecord->basePath_DCIM, cur_tm.tm_year + 1900, cur_tm.tm_mon + 1, cur_tm.tm_mday);

				sprintf(pLuxRecord->cur_record_info->file_name, "%04d%02d%02d%02d%02d%02d", cur_tm.tm_year + 1900, cur_tm.tm_mon + 1, cur_tm.tm_mday, cur_tm.tm_hour, cur_tm.tm_min, cur_tm.tm_sec);
				pLuxRecord->cur_record_info->record_size = 0;
				pLuxRecord->cur_record_info->start_time_ms = packTimeStamp_ms;
				pLuxRecord->cur_record_info->end_time_ms = 0;

				//分配一个节点，并取得节点位置
				lseek(pLuxRecord->fpInfo, 4, SEEK_SET);
				do
				{
					int readlen = read(pLuxRecord->fpInfo, &recordInfoBlank, sizeof(record_info_t));
					if (readlen != sizeof(record_info_t)) //文件已经结束，增加一个新节点
					{
						MYTRACE("alloc new record node within record.info file \n");
						memcpy(&recordInfoBlank, pLuxRecord->cur_record_info, sizeof(record_info_t));
						lseek(pLuxRecord->fpInfo, 0, SEEK_END);
						writeEx(pLuxRecord->fpInfo, &recordInfoBlank, sizeof(record_info_t));
						pLuxRecord->cur_record_info_file_pos = lseek(pLuxRecord->fpInfo,0,SEEK_CUR)-sizeof(record_info_t);

						break;
					}
					else 
					{
						if (recordInfoBlank.record_size == 0) //读取到一个节点，并且节点是未被占用的
						{
							MYTRACE("write record node within record.info file \n");
							pLuxRecord->cur_record_info_file_pos = lseek(pLuxRecord->fpInfo,0,SEEK_CUR) - sizeof(record_info_t);
							memcpy(&recordInfoBlank, pLuxRecord->cur_record_info, sizeof(record_info_t));
							lseek(pLuxRecord->fpInfo, pLuxRecord->cur_record_info_file_pos, SEEK_SET);
							writeEx(pLuxRecord->fpInfo, &recordInfoBlank, sizeof(record_info_t));

							break;
						}
						else
						{
							continue;
						}
					}

				} while (1);
			}
		}

		pthread_mutex_lock(pLuxRecord->mutex);
		frame_head_t* pack = (frame_head_t*)pLuxRecord->frame_buf;
		int will_write = sizeof(frame_head_t) + pack->frame_head_len + pack->frame_data_len;
		if (pLuxRecord->fpRecord != -1 && get_sdcard_free_space() > will_write + 50 * 1024 * 1024)
		{
			unsigned char* data = (char*)pack + sizeof(frame_head_t) + pack->frame_head_len;
#if 0
			//加密视频帧数据的数据
			if (FRAME_TYPE_VIDEO == pack->frame_type && 0x00 == data[0] && 0x00 == data[1])
			{
				data[0] = frame_head_magic[0];
				data[1] = frame_head_magic[1];
			}
#endif
			//printf("write frame type:[%d] \n", pack->frame_type);
			int writelen = writeEx(pLuxRecord->fpRecord, (char*)pack, will_write);
			video_frame_head_t* videoHead = (video_frame_head_t*)((char*)pack + sizeof(frame_head_t));
#if 0	
			if (videoHead->frame_type == 1)
			{
				printf("write len:%d frame len:%d data:%02X %02X %02X %02X %02X \n", writelen, videoHead->frame_len, data[0],data[1],data[2],data[3],data[4]);
			}
#endif
			pLuxRecord->writeByteNum += writelen;

			//向前移动缓冲区
			memcpy(pLuxRecord->frame_buf, pLuxRecord->frame_buf+will_write, pLuxRecord->data_len-will_write);
			pLuxRecord->data_len -= will_write;

			//改变剩余空间大小值
			if (pLuxRecord->need_re_stat_space || time_ms()/1000 - pLuxRecord->stat_space_time_s > 30)
			{
				//获取SD卡的剩余空间
				uint64_t total_size, free_size;
				int ret = stat_space(pLuxRecord->basePath_DCIM, &free_size, &total_size);
				if (0 == ret)
				{
					pLuxRecord->stat_space_time_s = time_ms()/1000;
					//设置剩余空间大小值
					set_sdcard_free_space(free_size);
					MYTRACE("restat sd card free space(MB):%llu, total space(MB):%llu \n", free_size / (1024 * 1024L), total_size / (1024 * 1024L));
				}
				else
				{
					MYERROR("error: read sd card failed, statvfs ret:%d \n ", ret);
				}
				pLuxRecord->need_re_stat_space = 0;
			}
			else
			{
				set_sdcard_free_space(get_sdcard_free_space() - writelen);
			}			
		}
		else if (pLuxRecord->fpRecord != -1 && get_sdcard_free_space() <= will_write + 50 * 1024 * 1024)
		{
			pLuxRecord->need_re_stat_space = 1;
			MYWARN("sd card is full,free space:[%llu], will_write:%d!\n", get_sdcard_free_space(), will_write);
		}
		else if (pLuxRecord->fpRecord == -1)
		{
			MYERROR("pLuxRecord->fpRecord is NULL\n");
		}

		pthread_mutex_unlock(pLuxRecord->mutex);
		
		//更新节点信息
		pLuxRecord->cur_record_info->end_time_ms = packTimeStamp_ms;
		pLuxRecord->cur_record_info->record_size = pLuxRecord->writeByteNum;
		if (pLuxRecord->fpInfo)
		{
			lseek(pLuxRecord->fpInfo, pLuxRecord->cur_record_info_file_pos, SEEK_SET);
			writeEx(pLuxRecord->fpInfo, pLuxRecord->cur_record_info, sizeof(record_info_t));
		}
		else
		{
			MYERROR("record info file pointer is NULL \n");
		}

		//检查文件是否超过大小限制
		if (pLuxRecord->writeByteNum >= pLuxRecord->recordFileMaxByteNum)
		{
			MYTRACE("file[%s/%s.mp4] size[%u] is greater than 30M, close and create new file\n", 
				pLuxRecord->cur_file_path, pLuxRecord->cur_record_info->file_name, pLuxRecord->writeByteNum);

			pLuxRecord->writeByteNum = 0;
			fsync(pLuxRecord->fpRecord);
			close(pLuxRecord->fpRecord);
			pLuxRecord->fpRecord = -1;
		}

		if (pLuxRecord->data_len > sizeof(frame_head_t))
		{
			usleep(1000);
			continue;
		}
		else
		{
			usleep(2*1000);
			continue;
		}

	} while (!pLuxRecord->stop_record_thread);

	return 0;
}

static int search_dir(const char* dirName, int* dateList, int minValue, int maxValue)
{
	int dir_count = 0;

	DIR* dirfiles;
	dirfiles = opendir(dirName);
	struct dirent* dir;
	while ((dir = readdir(dirfiles)) != NULL)
	{
		if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
		{
			continue;
		}
		
		if ((strspn(dir->d_name, "0123456789") == strlen(dir->d_name)) && (dir->d_type & DT_DIR))
		{
			int value = atoi(dir->d_name);
			if (value >= minValue && value <= maxValue)
			{
				dateList[dir_count++] = value;
			}
			else
			{
				printf("record file directory[%s] subdir[%s] is not between [%d] and [%d] \n", dirName, dir->d_name, minValue, maxValue);
			}
		}
		
	}
	closedir(dirfiles);

	bubble_sort(dateList, dir_count);

	return dir_count;
}

static int findMinValue(int* dateList, int num)
{
	if (num == 1)
	{
		return dateList[0];
	}

	int ret = dateList[0];
	int i;
	for (i = 1; i < num; i++)
	{
		if (dateList[i] < ret)
		{
			ret = dateList[i];
		}
	}

	return ret;
}

int remove_dir(const char* dirName)
{
	char cur_dir[] = ".";
	char up_dir[] = "..";
	char dir_name[128];
	DIR* dirp;
	struct dirent* dp;
	struct stat dir_stat;

	// 参数传递进来的目录不存在，直接返回
	if (0 != access(dirName, F_OK)) {
		return 0;
	}

	// 获取目录属性失败，返回错误
	if (0 > stat(dirName, &dir_stat)) {
		perror("get directory stat error");
		return -1;
	}

	if (S_ISREG(dir_stat.st_mode)) {	// 普通文件直接删除
		remove(dirName);
	}
	else if (S_ISDIR(dir_stat.st_mode)) {	// 目录文件，递归删除目录中内容
		dirp = opendir(dirName);
		while ((dp = readdir(dirp)) != NULL) {
			// 忽略 . 和 ..
			if ((0 == strcmp(cur_dir, dp->d_name)) || (0 == strcmp(up_dir, dp->d_name))) {
				continue;
			}

			sprintf(dir_name, "%s/%s", dirName, dp->d_name);
			remove_dir(dir_name);   // 递归调用
		}
		closedir(dirp);

		rmdir(dirName);		// 删除空目录
	}
	else {
		perror("unknow file type!");
	}

	return 0;
}

static void* loop_delete_threadfunc(void* p)
{
	lux_record_t* pLuxRecord = (lux_record_t*)p;

	uint64_t cur_free_space = 0;

	int yearList[300];
	int monthList[12];
	int dayList[31];

	int dateNumYear = 0, dateNumMonth = 0, dateNumDay = 0;
	char dirTmp[128];
	int i, j, k;
	int need_re_stat_space;

	int minYear = 0, minMonth = 0, minDay = 0;

	do {
		usleep(2 * 1000 * 1000);

		if (!pLuxRecord->is_running)
		{
			continue;
		}

		cur_free_space = get_sdcard_free_space();
		need_re_stat_space = pLuxRecord->need_re_stat_space; 

		printf("loop_delete_threadfunc ... \n");

		if (access(pLuxRecord->basePath_DCIM,0) == 0 && !need_re_stat_space && cur_free_space < 100 * 1024 * 1024)
		{
			printf("begin loop delete record file ... \n");

			/**
			一次删除一天的录像 删除整个文件夹
			**/
				
			//获取年份列表
			sprintf(dirTmp, "%s", pLuxRecord->basePath_DCIM);
			dateNumYear = search_dir(dirTmp, &yearList[0], 1900, 2100);
			if (dateNumYear == 0)
			{
				printf("sd card has not record file, because can not find year directory! \n");
				continue;
			}

			printf("search dir[%d]:[%s], ", dateNumYear, dirTmp);
			for (i = 0; i < dateNumYear; i++) printf("%04d ", yearList[i]);
			printf("\n");

			//找最早的年 月 日
			for (i = 0; i < dateNumYear; i++)
			{
				printf("begin search months from year directory \n");
				sprintf(dirTmp, "%s/%04d", pLuxRecord->basePath_DCIM, yearList[i]);
				dateNumMonth = search_dir(dirTmp, &monthList[0], 1, 12);
				printf("search dir[%d]:[%s], ", dateNumMonth, dirTmp);
				int id;
				for (id = 0; id < dateNumMonth; id++) printf("%02d ", monthList[id]);
				printf("\n");

				if (dateNumMonth == 0)
				{
					printf("not found month in the [%s] \n", dirTmp);
					continue;
				}

				minDay = 0;
				printf("begin search days from month directory \n");
				for (j = 0; j < dateNumMonth; j++)
				{
					sprintf(dirTmp, "%s/%04d/%02d", pLuxRecord->basePath_DCIM, yearList[i], monthList[j]);
					dateNumDay = search_dir(dirTmp, &dayList[0], 1, 31);
					printf("search dir[%d]:[%s], ", dateNumDay, dirTmp);
					for (k = 0; k < dateNumDay; k++) printf("%02d ", dayList[k]);
					printf("\n");
					if (dateNumDay == 0)
					{
						continue;
					}
					minDay = dayList[0];
					break;
				}

				if (minDay > 0)
				{
					minYear = yearList[i];
					minMonth = monthList[j];
					break;
				}
			}

			printf("minYear:%04d minMonth:%02d minDay:%02d \n", minYear, minMonth, minDay);

			//删除整天的录像
			if (minDay > 0)
			{
				sprintf(dirTmp, "%s/%04d/%02d/%02d", pLuxRecord->basePath_DCIM, minYear, minMonth, minDay);
				printf("delete record path:%s \n", dirTmp);
				remove_dir(dirTmp);
			}

			//获取SD卡的剩余空间
			uint64_t total_size, free_size;
			int ret = stat_space(pLuxRecord->basePath_DCIM, &free_size, &total_size);
			if (0 == ret)
			{
				//设置剩余空间
				set_sdcard_free_space(free_size);
				pLuxRecord->need_re_stat_space = 1;
			}

			printf("loop delete record file finished! \n");
		}

	} while (!pLuxRecord->stop_loop_delete_thread);

	return 0;
}

#if 0
static void* check_sdcard_threadfunc(void* p)
{
	lux_record_t* pLuxRecord = (lux_record_t*)p;

	do
	{
		if (0 == pLuxRecord->is_running)
		{
			if (SD_STATUS_NORMAL == LUX_ipc_sd_get_status())
			{
				//???

				pLuxRecord->is_running = 1;
			}
		}

		usleep(1000*1000);
	} while (1);
}
#endif

int LUX_Record_Start()
{
	int err;

	//if (!g_lux_record->is_running)
	//{
	//	return 0;
	//}

	//启动原始文件创建线程
	pthread_t create_origin_file_thread_id;
	err = pthread_create(&create_origin_file_thread_id, NULL, create_origin_file_threadfunc, g_lux_record);
	pthread_detach(create_origin_file_thread_id);

	//启动录像存储线程
	pthread_t record_thread_id;
	err = pthread_create(&record_thread_id, NULL, record_threadfunc, g_lux_record);
	pthread_detach(record_thread_id);

#if 1
	//启动循环删除线程
	pthread_t loop_delete_thread_id;
	err = pthread_create(&loop_delete_thread_id, NULL, loop_delete_threadfunc, g_lux_record);
	pthread_detach(loop_delete_thread_id);

	//启动SD卡检查线程
	//pthread_t check_sdcard_thread_id;
	//err = pthread_create(&check_sdcard_thread_id, NULL, check_sdcard_threadfunc, g_lux_record);
	//pthread_detach(check_sdcard_thread_id);
#endif

	return 0;
}

int LUX_Record_Stop()
{
	g_lux_record->stop_record_thread = 1;
	g_lux_record->stop_loop_delete_thread = 1;
	
	sleep(2);
	
	return 0;
}

int LUX_Record_Final()
{
	pthread_mutex_destroy(g_lux_record->mutex);
	free(g_lux_record->mutex);

	return 0;
}

frame_head_t* LUX_Record_alloc_packet(int type, int pack_len)
{
	pthread_mutex_lock(g_lux_record->mutex);
	
	if (g_lux_record->data_len + pack_len <= RECORD_BUF_MAX_SIZE)
	{
		return (frame_head_t*)(g_lux_record->frame_buf+g_lux_record->data_len);
	}
	else
	{
		return NULL;
	}
}

int LUX_Record_free_packet(frame_head_t* pack)
{
	pthread_mutex_unlock(g_lux_record->mutex);

	return 0;
}

int LUX_Record_push_packet(frame_head_t* pack)
{
	if (!g_lux_record->is_running)
	{
		return -1;
	}

	pthread_mutex_lock(g_lux_record->mutex);
	int pack_len = sizeof(frame_head_t) + pack->frame_head_len + pack->frame_data_len;
	if (g_lux_record->data_len + pack_len <= RECORD_BUF_MAX_SIZE)
	{
		memcpy(g_lux_record->frame_buf + g_lux_record->data_len, (char*)pack, pack_len);
		g_lux_record->data_len += pack_len;
	}
	pthread_mutex_unlock(g_lux_record->mutex);

	return 0;
}

int LUX_Record_Find_By_Time(IN const uint64_t startTime_ms, OUT char* file_name)
{
	int ret = -1;
	char filePath[64];
	struct tm curTime;
	time_t time1;
	record_info_t recordInfo;
	int i = -3;

	//查找前后两天的录像
	do
	{
		i++;
		time1 = startTime_ms/1000 + i*24*3600;
		curTime = *gmtime(&time1);
		sprintf(filePath, "%s/%04d/%02d/%02d/record.info",
			g_lux_record->basePath_DCIM,
			curTime.tm_year + 1900,
			curTime.tm_mon + 1,
			curTime.tm_mday);

		if (access(filePath, 0) != 0)
		{
			MYWARN("not found file:[%s]\n", filePath);
			continue;
		}

		FILE* fp1 = fopen(filePath, "rb");
		if (fp1)
		{
			fseek(fp1, sizeof(int), SEEK_SET);

			do {
				int readlen1 = -1;
				readlen1 = fread(&recordInfo, 1, sizeof(record_info_t), fp1);
				if (readlen1 != sizeof(record_info_t))
				{
					MYERROR("not found record file within path[%s]\n", filePath);
					break;
				}

				//printf("record info time:[%llu][%s], play time:[%llu][%s] \n", recordInfo.start_time_ms, format_time(recordInfo.start_time_ms/1000), startTime_ms, format_time(startTime_ms/1000));

				if (recordInfo.start_time_ms >= startTime_ms)
				{					
					ret = 0;
					recordInfo.file_name[14] = '\0';
					sprintf(file_name, "%s/%04d/%02d/%02d/%s.mp4", g_lux_record->basePath_DCIM, curTime.tm_year + 1900, curTime.tm_mon + 1, curTime.tm_mday, recordInfo.file_name);
					MYTRACEING("already found record file:[%s] \n", file_name);
					break;
				}

			} while (1);

			fclose(fp1);
		}
		else
		{
			MYERROR("open file[%s] error:[%d][%s]\n", filePath, errno, strerror(errno));
		}

	} while (i <= 2 && ret != 0);

	return ret;
}

