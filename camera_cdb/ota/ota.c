#include<stdio.h>
#include<unistd.h>

/* 执行系统指令 */
static int LUX_BASE_System_CMD(char* pCmd)
{
	int ret = 0;
	char cCmd[256] = { 0 };
	char buffer[512] = { 0 };
	//strncpy(cCmd,256,pCmd);
	snprintf(cCmd, 256, "%s", pCmd);
	printf("cmd: %s\n", pCmd);
	FILE* pp = popen(cCmd, "r");
	if (NULL == pp)
	{
		ret = -1;
		printf("LUX_BASE_System_CMD Err(%d)\n", ret);
		return ret;
	}

	while (fgets(buffer, sizeof(buffer), pp) != NULL)
	{
		printf("%s\n", buffer);
	}

	pclose(pp);

	return ret;
}

int main(int argc, char* argv[])
{
	int find = 0;
	while (1)
	{
		if (access("/tmp/mark_ota.txt", F_OK) == 0)
		{
			find = 1;
			break;
		}
		sleep(1);
	}
	if(find)
	{
		LUX_BASE_System_CMD("killall media_sample");
		sleep(5);
		LUX_BASE_System_CMD("unzip -o /tmp/ota.zip -d /tmp;chmod 775 -R /tmp/ota");
		LUX_BASE_System_CMD("sh /tmp/ota/ota.sh");
		printf("ota upgrade successfully! Now reboot.\n");
		sleep(1);
		LUX_BASE_System_CMD("reboot");
	}


	return 0;
}
