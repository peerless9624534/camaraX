#ifndef __VERIFY_H__
#define __VERIFY_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif /* __cplusplus */

/**
 * please don't modify this
 **/
__attribute__((__used__)) __attribute__((__section__(".protected_data")))
static unsigned char vendor_info[32] = {
	'I', 'n', 'g', 'e', 'n', 'i', 'c', '\0',
	'a', 'l', 'g', 'o', '\0', '\0', '\0', '\0',
	'v', 'e', 'r', 'i', 'f', 'y', '\0', '\0',
	'v', 'e', 'r', '0', '0', '1', '\0', '\0',
};

int verify_dummy(int number);
int license_verify(unsigned int magic, unsigned int algo_version, unsigned int protected_start, unsigned int protected_end);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
