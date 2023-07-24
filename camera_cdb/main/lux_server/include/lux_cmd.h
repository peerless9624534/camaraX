#include <hichannel_host_comm.h>
#include <hi_types.h>

typedef enum {
    /* commands */
    LUX_CMD_IOCTL,
    LUX_CMD_HELP,
    LUX_CMD_EXIT,
    LUX_CMD_IRLED,
    LUX_CMD_IRCUT,
    LUX_CMD_LED,
    LUX_CMD_PERSONDET,
    LUX_CMD_BUTT,
} sample_cmd_e;

/* command/event information */
typedef struct {
    sample_cmd_e what;
    hi_u32 len;
    hi_u8 obj[LUX_CMD_MAX_LEN];
} sample_message_s;

extern const sample_cmd_entry_stru g_lux_cmd[];
extern int g_lux_cmd_cnt;
hi_s32 lux_cmd_process(hi_void *wdata, hi_char *param, hi_u32 len, hi_void *pmsg);
hi_s32 lux_cmd_exec(sample_message_s* message);