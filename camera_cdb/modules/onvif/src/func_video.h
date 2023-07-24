#ifndef __FUNC_VIDEO_H__

#ifdef __cplusplus
extern "C" {
#endif

int func_video_set_fps(uint32_t fps_num, uint32_t fps_den);
int func_video_get_fps(uint32_t *fps_num, uint32_t *fps_den);
int func_video_set_bitrate(int encChnNum, int bps_num);
int func_video_get_bitrate(int encChnNum, int *bps_num);
int func_video_set_resolution(int fsChnNum, int picWidth, int picHeigth);
int func_video_get_resoulution(int fsChnNum, int *picWidth, int *picHeigth);


#ifdef __cplusplus
}
#endif

#endif /* __FUNC_VIDEO_H__ */
