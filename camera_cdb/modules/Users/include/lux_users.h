
#ifndef _LUX_USERS_H__
#define _LUX_USERS_H__

#define USER_INFO_FILE "/system/init/user.dat"
#define MAX_USER_NUM 16

enum LUX_USER_ROLE
{
    ROLE_ADMIN = 0,
    ROLE_OPERATER = 1,
    ROLE_USER = 2,
    ROLE_ANONYMOUS = 3,
    ROLE_EXTENDED = 4,
};

typedef struct LUX_USER_INFO_ST
{
	char user_id[16];
	char user_name[32];
	char password[64];
	int  role;
	int  resv;
}LUX_USER_INFO_ST;

//extern struct LUX_USER_INFO_ST g_UsersInfo[16];

int LUX_UserMngr_Init();

int LUX_UserMngr_AddUser(LUX_USER_INFO_ST *user);

int LUX_UserMngr_DelUser(const char * user_name);

int LUX_UserMngr_ModifyUser(struct LUX_USER_INFO_ST* user);

int LUX_UserMngr_ListUsers(struct LUX_USER_INFO_ST* user, int *num);

int LUX_UserMngr_ModifyPasswd(const char *userID, const char *passwd);

int LUX_UserMngr_GetPasswd(const char* userID, char* passwd);

int LUX_UserMngr_Final();

#endif
