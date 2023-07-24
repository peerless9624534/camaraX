#include <lux_base.h>
#include <jwhead.h>
#include "lux_users.h"



LUX_USER_INFO_ST g_UsersInfo[MAX_USER_NUM] = { 0 };

int LUX_UserMngr_Init()
{
	int ret = 0;
     
	if ( access(USER_INFO_FILE, 0) != 0)
	{
		strcpy(g_UsersInfo[0].user_id, "admin");
		strcpy(g_UsersInfo[0].user_name, "administrator");
		strcpy(g_UsersInfo[0].password, "admin12345");
		g_UsersInfo[0].role = ROLE_ADMIN;

		strcpy(g_UsersInfo[1].user_id, "Operator");
		strcpy(g_UsersInfo[1].user_name, "Operator");
		strcpy(g_UsersInfo[1].password, "admin12345");
		g_UsersInfo[1].role = ROLE_OPERATER;
         
		FILE* fp = fopen(USER_INFO_FILE, "wb");
		if (fp)
		{
			fwrite(g_UsersInfo, sizeof(g_UsersInfo[0])*2, 1, fp);
			fclose(fp);
			MYTRACE("init user info success!\n");
		}
		else
		{
			MYERROR("init user info failed err:[%d],str:[%s] \n", errno, strerror(errno));
		}
	}
	else
	{
		FILE* fp = fopen(USER_INFO_FILE, "rb");

		if (fp)
		{
			int readlen = fread(g_UsersInfo, sizeof(g_UsersInfo), 1, fp);
			if (readlen != 1)
			{
				MYERROR("load user info failed, err:[%d] str:[%s] \n", errno, strerror(errno));

				strcpy(g_UsersInfo[0].user_id, "admin");
				strcpy(g_UsersInfo[0].user_name, "administrator");
				strcpy(g_UsersInfo[0].password, "admin12345");
				g_UsersInfo[0].role = ROLE_ADMIN; 

				strcpy(g_UsersInfo[1].user_id, "Operator");
				strcpy(g_UsersInfo[1].user_name, "Operator");
				strcpy(g_UsersInfo[1].password, "admin12345");
				g_UsersInfo[1].role = ROLE_OPERATER;
			}
			else
			{
				MYTRACE("load user info success! \n");
			}
			fclose(fp);

		}
		else
		{
			MYERROR("open file[] is failed, err:[%d], str[%s] \n", USER_INFO_FILE, errno, strerror(errno));
		}

	}

	return ret;
}

int LUX_UserMngr_AddUser(LUX_USER_INFO_ST* user)
{ 
	int ret = 0;
    int r, num = 0;
    int i = 0;
	if (access(USER_INFO_FILE, 0) == 0)
	{
        /*比较用户名和密码，如果与本地的重复返回-1*/
        FILE* fp_1 = fopen(USER_INFO_FILE, "rb");
        if (fp_1)
        {
            /*读取用户数据到缓存*/
            r = fread(&g_UsersInfo[num], 1, sizeof(struct LUX_USER_INFO_ST), fp_1);
            while (r) {
                num++;
                r = fread(&g_UsersInfo[num], 1, sizeof(struct LUX_USER_INFO_ST), fp_1);
            }
            fclose(fp_1);
        }
        int count = 0;
        while (count <= num) {
            if (strcmp(g_UsersInfo[count].user_name, user->user_name) == 0 )
            {
                MYTRACE("Add user duplication，Please try again!\n");
                return -1;
            }
            else {
                count++;
            }
        }
         
        MYTRACE("user is not repetitive，Add later!\n");

		FILE* fp = fopen(USER_INFO_FILE, "ab+");
		if (fp)
		{
			ret = fwrite(user, sizeof(*user), 1, fp);
            if (ret != 1)
            {
                fclose(fp);
                return -1;
                MYERROR("write file is fault!\n");
            }
			fclose(fp);
			MYTRACE("add user success!\n");
		}
		else
		{
            ret = -1;
			MYERROR("open file[] is faileds err:[%d],str:[%s] \n", errno, strerror(errno));
		}
	}
    else
    {
        MYERROR("File-user.dat does not exist, \n");
        ret = -1;
    }

	return ret;
}

int LUX_UserMngr_DelUser(const char* user_name)
{
	int ret = 0;
	if (access(USER_INFO_FILE, 0) == 0)
	{
		FILE* fp = fopen(USER_INFO_FILE, "rb");
		if (fp)
		{
			int r, num = 0;
			//读取用户数据到缓存
			r = fread(&g_UsersInfo[num], 1, sizeof(struct LUX_USER_INFO_ST), fp);
			while (r) {
				num++;
				r = fread(&g_UsersInfo[num], 1, sizeof(struct LUX_USER_INFO_ST), fp);
			}
			fclose(fp);
			int count = 0 ,i = 0;
			//删除用户        
            while (count <= num) {
                if (strcmp(g_UsersInfo[count].user_name, user_name) == 0) {
                    i = count;
                    for (; i < num; i++) {
                        strcpy(g_UsersInfo[i].user_name, g_UsersInfo[i + 1].user_name);
                        strcpy(g_UsersInfo[i].user_id, g_UsersInfo[i + 1].user_id);
                        strcpy(g_UsersInfo[i].password, g_UsersInfo[i + 1].password);
                        g_UsersInfo[i].role = g_UsersInfo[i + 1].role;
                    }
                    break;
                }
                else {
                    count++;
                }
            }
#if 0
			while (count <= num) {
				if (strcmp(g_UsersInfo[count].user_id, userID) == 0) {
					for (; count < num; count++) {
						strcpy(g_UsersInfo[count].user_name, g_UsersInfo[count + 1].user_name);
						strcpy(g_UsersInfo[count].user_id, g_UsersInfo[count + 1].user_id);
						strcpy(g_UsersInfo[count].password, g_UsersInfo[count + 1].password);
					}
					break;
				}
				else {
					count++;
				}
			}
#endif
			if (count > num) 
            {
                printf("User not found!\n");
                return -1;
			}

			//写入用户数据
			fp = fopen(USER_INFO_FILE, "wb");
            fwrite(g_UsersInfo, sizeof(struct LUX_USER_INFO_ST)*(num-1), 1, fp);
			fclose(fp);
			MYTRACE("Delete user success!\n");
		}
		else
		{
			MYERROR("open file[] is failed err:[%d],str:[%s] \n", errno, strerror(errno));
		}
	}
	return ret;
}


int LUX_UserMngr_ModifyUser(struct LUX_USER_INFO_ST* user)
{
	int ret = 0;
    int i = 0;
	if (access(USER_INFO_FILE, 0) == 0)
	{
		FILE* fp = fopen(USER_INFO_FILE, "rb");
		if (fp)
		{
			int r, num = 0;
			//读取用户数据到缓存
			r = fread(&g_UsersInfo[num], 1, sizeof(struct LUX_USER_INFO_ST), fp);
			while (r) {
				num++;
				r = fread(&g_UsersInfo[num], 1, sizeof(struct LUX_USER_INFO_ST), fp);
			}
			fclose(fp);
			int count = 0;
			//修改字段
			while (count <= num) {
				if (strcmp(g_UsersInfo[count].user_name, user->user_name) == 0) {
						strcpy(g_UsersInfo[count].user_name, user->user_name);
						strcpy(g_UsersInfo[count].user_id, user->user_id);
						strcpy(g_UsersInfo[count].password, user->password);
                        g_UsersInfo[count].role = user->role;
					break;
				}
				else {
					count++;
				}
			}
			if (count > num) {
                return -1;
				printf("User not found!\n");
			}
			//写入用户数据
			fp = fopen(USER_INFO_FILE, "wb");
            fwrite(g_UsersInfo, sizeof(struct LUX_USER_INFO_ST) * num, 1, fp);
			fclose(fp);
			MYTRACE("Modify  User  success!\n");
		}
		else
		{
			MYERROR("open file[] is failed err:[%d],str:[%s] \n", errno, strerror(errno));
		}
	}

	return ret;
}

int LUX_UserMngr_ListUsers(struct LUX_USER_INFO_ST* user, int* num)
{
	int ret = 0;
	(*num) = 0;
	int i = 0;
    int UsersInfo_num = 0;
    int r = 0;
    FILE* fp = fopen(USER_INFO_FILE, "rb");
    if (fp)
    {
        /*读取用户数据到缓存*/
        r = fread(&g_UsersInfo[UsersInfo_num], 1, sizeof(struct LUX_USER_INFO_ST), fp);
        while (r) {
            UsersInfo_num++;
            r = fread(&g_UsersInfo[UsersInfo_num], 1, sizeof(struct LUX_USER_INFO_ST), fp);
        }
        fclose(fp);
    }

	for (i = 0; i < MAX_USER_NUM; i++)
	{
		if (strlen(g_UsersInfo[i].user_id) > 0)
		{
			memcpy(&user[i], &g_UsersInfo[i], sizeof(struct LUX_USER_INFO_ST));
			(*num) += 1;
		}
		else
		{
			break;
		}
	}

	return ret;
}

int LUX_UserMngr_ModifyPasswd(const char* userID, const char* passwd)
{
	int ret = 0;

	if (access(USER_INFO_FILE, 0) != 0)
	{
		FILE* fp = fopen(USER_INFO_FILE, "rb");
		if (fp)
		{
			int r, num = 0;
			//读取用户数据到缓存
			r = fread(&g_UsersInfo[num], 1, sizeof(struct LUX_USER_INFO_ST), fp);
			while (r) {
				num++;
				r = fread(&g_UsersInfo[num], 1, sizeof(struct LUX_USER_INFO_ST), fp);
			}
			fclose(fp);
			int count = 0;
			//修改字段
			while (count <= num) {
				if (strcmp(g_UsersInfo[count].user_id, userID) == 0) {
					for (; count < num; count++) {
						strcpy(g_UsersInfo[count].user_id, userID);
						strcpy(g_UsersInfo[count].password, passwd);
					}
					break;
				}
				else {
					count++;
				}
			}
			if (count > num) {
				printf("User not found!\n");
			}
			//写入用户数据
			fp = fopen(USER_INFO_FILE, "wb");
			count = 0;
			while (count <= num) {
				fwrite(&g_UsersInfo[count], 1, sizeof(struct LUX_USER_INFO_ST), fp);
				count++;
			}
			fclose(fp);
			MYTRACE("Modify  Passwd success!\n");
		}
		else
		{
			MYERROR("open file[] is failed err:[%d],str:[%s] \n", errno, strerror(errno));
		}
	}


	return ret;
}

int LUX_UserMngr_GetPasswd(const char* userID, char* passwd)
{
	int ret = -1;
	int i = 0;

	for (i = 0; i < MAX_USER_NUM; i++)
	{
		MYTRACE("userid:[%s] userid[%s] \n", g_UsersInfo[i].user_id, userID);
		if (strcmp(g_UsersInfo[i].user_id, userID) == 0)
		{
			strcpy(passwd, g_UsersInfo[i].password);
			ret = 0;
			break;
		}
	}

	return ret;
}

int LUX_UserMngr_Final()
{
	int ret = 0;

	//???

	return ret;
}