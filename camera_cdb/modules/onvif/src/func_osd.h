#ifndef __FUNC_OSD_H__

#ifdef __cplusplus
extern "C" {
#endif


extern int func_OSD_delete_OsdRgn(int streamnum, char *token);
extern int func_osd_create_text(int streamnum, int pos_x, int pos_y, char *text, char *token);
extern int func_osd_set_text(int streamnum,char* osdtoken, int pos_x, int pos_y, char *text);
extern int func_osd_get_osdrgn();
extern int func_osd_get_osdrgns_num(int streamnum);
extern int func_osd_get_osdrgns();

#ifdef __cplusplus
}
#endif

#endif /* __FUNC_VIDEO_H__ */
