#ifndef __LIB_JPEG_CODEC_H__
#define __LIB_JPEG_CODEC_H__
//#define C_VB_ALIGN 256

typedef struct {
	uint8_t *streamAddr;
	int streamLen;
}streamInfo;

int alcodec_jpege_init(void **h, int picWidth, int picHight, int initQp);
int alcodec_jpege_encode(void *h, IMPFrameInfo frame, streamInfo *stream);
int alcodec_jpege_deinit(void *h);

#endif
