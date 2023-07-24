#include <stdio.h>
#include "func_log.h"

#include "config.h"

//#include "xcam_osd.h"
//#include "xcam_general.h"
//#ifndef ONVIF_WITH_CARRIER_SERVER
//#include "xcam_ctrl.h"
//#endif


int func_osd_delete_osdrgn(int streamnum, char *token)
{
	int ret = XCAM_SUCCESS;
#ifdef ONVIF_WITH_CARRIER_SERVER
	return ret;
#else
	ret = xcam_osd_delete_osdrgn(streamnum,token);
	return ret;
#endif
}

int func_osd_create_text(int streamnum, int pos_x, int pos_y, char *text, char *token)
{
	int ret = XCAM_SUCCESS;
#ifdef ONVIF_WITH_CARRIER_SERVER
	return ret;
#else
	ret = xcam_osd_create_text(streamnum, pos_x, pos_y, text, token);
	return ret;
#endif
}

int func_osd_set_text(int streamnum, char* osdtoken, int pos_x, int pos_y, char *text)
{
	int ret = XCAM_SUCCESS;
#ifdef ONVIF_WITH_CARRIER_SERVER
	return ret;
#else
	ret = xcam_osd_set_text(streamnum, osdtoken, pos_x, pos_y, text);
	return ret;
#endif
}

int func_osd_get_osdrgn()
{
	int ret = XCAM_SUCCESS;
#ifdef ONVIF_WITH_CARRIER_SERVER
	return ret;
#else
	return ret;
#endif
}

int func_osd_get_osdrgns_num(int streamnum)
{
	int ret = XCAM_SUCCESS;
#ifdef ONVIF_WITH_CARRIER_SERVER
	return ret;
#else
	ret = xcam_osd_get_osdrgns_num(streamnum);
	return ret;
#endif
}

int func_osd_get_osdrgns()
{
	int ret = XCAM_SUCCESS;
#ifdef ONVIF_WITH_CARRIER_SERVER
	return ret;
#else

	return ret;
#endif
}
