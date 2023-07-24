#include <hichannel_host_comm.h>
#include <hi_types.h>
#include <hi_base.h>
#include <lux_cmd.h>
#include <lux_hal_led.h>
#include <lux_hal_misc.h>
#include <lux_ivs.h>
#include <string.h>
#include <lux_person_det.h>

const sample_cmd_entry_stru  g_lux_cmd[] = {
    {"help", lux_cmd_process, LUX_CMD_HELP},
    {"exit", lux_cmd_process, LUX_CMD_EXIT},
    {"irled", lux_cmd_process, LUX_CMD_IRLED},
    {"ircut", lux_cmd_process, LUX_CMD_IRCUT},
    {"led", lux_cmd_process, LUX_CMD_LED},
    {"persondet", lux_cmd_process, LUX_CMD_PERSONDET},
};

struct lux_param
{
    hi_char           *cmd_name;    /* 命令字符串 */
    int option_id;
};


int g_lux_cmd_cnt = sizeof(g_lux_cmd) / sizeof(g_lux_cmd[0]);

int get_param_option(const struct lux_param * array, int arrsize, const char* option)
{
    
    while (--arrsize >= 0)
    {
        //printf("cmd_name:%s\n",array[arrsize].cmd_name);
        if(strncmp(array[arrsize].cmd_name, option, strlen(array[arrsize].cmd_name)) == 0)
        {
            //printf("match option:%s\n", option);
            return array[arrsize].option_id;
        }
    }
    return -1;
}

hi_s32 lux_cmd_process(hi_void *wdata, hi_char *param, hi_u32 len, hi_void *pmsg)
{
    sample_log_print("param %s, len%d", param, len);
    return 0;
}

hi_s32 lux_cmd_persondet(hi_char *param, hi_u32 len)
{
    hi_u32 offset = 0;
    int ret = -1;
    int onoff = LUX_FALSE;
    int sensitive = LUX_IVS_SENSITIVE_LOW_EN;
    hi_char param1[8] = {0};
    hi_char param2[8] = {0};
    hi_char param_ok = 0;
    
    const struct lux_param param1_list[] =
    {
        {"off", 0},
        {"on", 1},
    };
    const struct lux_param param2_list[] =
    {
        {"low", LUX_IVS_SENSITIVE_LOW_EN},
        {"mid", LUX_IVS_SENSITIVE_MID_EN},
        {"hig", LUX_IVS_SENSITIVE_HIG_EN},
    };
    do
    {
        if (param == HI_NULL) {
            break;
        }
        if (sample_get_cmd_one_arg(&param[offset], param1, 8, &offset) != HI_SUCCESS) {
            break;
        }
        printf("param1:%s\n", param1);
        onoff = get_param_option(param1_list, sizeof(param1_list)/sizeof(param1_list[0]), param1);
        if(onoff < 0) 
            break;
        if (sample_get_cmd_one_arg(&param[offset + 1], param2, 8, &offset) != HI_SUCCESS) {
            break;
        }
        printf("param2:%s\n", param2);
        sensitive = get_param_option(param2_list, sizeof(param2_list)/sizeof(param2_list[0]), param2);
        if(sensitive < 0) 
            break;
        param_ok = 1;
    }while(0);
    if(param_ok)
    {
        if (onoff)
        {
            ret = LUX_IVS_PD_StartAlgo();
            if (0 != ret)
            {
                printf("LUX_IVS_StartAlgo failed!, error num [0x%x] ", ret);
            }
            ret = LUX_IVS_PD_SetSense(sensitive);
            if (0 != ret)
            {
                printf("LUX_IVS_SetSensitive failed!, error num [0x%x] ", ret);
            }
        }
        else
        {
            ret = LUX_IVS_PD_StartAlgo();
            if (0 != ret)
            {
                printf("LUX_IVS_StopAlgo failed!, error num [0x%x] ", ret);
            }
        }
    }
    else
    {
        printf("persondet,<on/off>,<low/mid/hig>");
    }
    return 0;
}

hi_s32 lux_cmd_led(hi_char *param, hi_u32 len)
{
    int led, status;
    hi_u32 offset = 0;
    hi_char param1[8] = {0};
    hi_char param2[8] = {0};
    hi_char param_ok = 0;
    
    const struct lux_param param1_list[] =
    {
        {"green", LUX_HAL_LED_GREEN},
        {"red", LUX_HAL_LED_RED},
    };
    const struct lux_param param2_list[] =
    {
        {"off", LUX_HAL_LED_OFF},
        {"on", LUX_HAL_LED_ON},
        {"blink", LUX_HAL_LED_BLINK},
    };
    do
    {
        if (param == HI_NULL) {
            break;
        }
        if (sample_get_cmd_one_arg(&param[offset], param1, 8, &offset) != HI_SUCCESS) {
            break;
        }
        //printf("param1:%s\n", param1);
        led = get_param_option(param1_list, sizeof(param1_list)/sizeof(param1_list[0]), param1);
        if(led < 0) 
            break;
        if (sample_get_cmd_one_arg(&param[offset + 1], param2, 8, &offset) != HI_SUCCESS) {
            break;
        }
        //printf("param2:%s\n", param2);
        status = get_param_option(param2_list, sizeof(param2_list)/sizeof(param2_list[0]), param2);
        if(status < 0) 
            break;
        param_ok = 1;
    }while(0);
    if(param_ok)
    {
        LUX_HAL_LedSetStatus(led, status);
    }
    else
    {
        printf("led,<green/red>,<on/off/blink>");
    }
    return 0;
}

hi_s32 lux_cmd_ircut(hi_char *param, hi_u32 len)
{
    hi_u32 offset = 0;
    int enable;
    hi_char param1[8] = {0};
    hi_char param_ok = 0;
    
    const struct lux_param param_list[] =
    {
        {"day", 0},
        {"night", 1},
    };
    do
    {
        if (param == HI_NULL) {
            break;
        }
        if (sample_get_cmd_one_arg(&param[offset], param1, sizeof(param1), &offset) != HI_SUCCESS) {
            break;
        }
        //printf("param1:%s\n", param1);
        enable = get_param_option(param_list, sizeof(param_list)/sizeof(param_list[0]), param1);
        if(enable < 0) 
            break;
        param_ok = 1;
    }while(0);
    if(param_ok)
    {
        LUX_ISP_SetIRCUT(enable);
    }
    else
    {
        printf("ircut,<day/night>");
    }
    return 0;
}

hi_s32 lux_cmd_irled(hi_char *param, hi_u32 len)
{
    hi_u32 offset = 0;
    int enable;
    hi_char param1[8] = {0};
    hi_char param_ok = 0;
    
    const struct lux_param param_list[] =
    {
        {"off", 0},
        {"on", 1},
    };
    do
    {
        if (param == HI_NULL) {
            break;
        }
        if (sample_get_cmd_one_arg(&param[offset], param1, sizeof(param1), &offset) != HI_SUCCESS) {
            break;
        }
        //printf("param1:%s\n", param1);
        enable = get_param_option(param_list, sizeof(param_list)/sizeof(param_list[0]), param1);
        if(enable < 0) 
            break;
        param_ok = 1;
    }while(0);
    if(param_ok)
    {
        LUX_Hal_Misc_IRLightOnOff(enable);
    }
    else
    {
        printf("irled,<on/off>");
    }
    return 0;
}

hi_s32 lux_cmd_exec(sample_message_s* message)
{
    //sample_log_print("cmd_id%d param %s, len%d", message->what, message->obj, message->len);
    switch (message->what)
    {
    case LUX_CMD_IOCTL:
        {
            printf("cmd not found:%s", (hi_char*)message->obj);
        }
        break;
    case LUX_CMD_HELP:
        printf("cmd list\n");
        for (size_t i = 0; i < g_lux_cmd_cnt; i++)
        {
            printf("%s\n",g_lux_cmd[i].cmd_name);
        }
        break;
    case LUX_CMD_IRLED:
        {
            lux_cmd_irled((hi_char*)message->obj, message->len);
        }
        break;
    case LUX_CMD_IRCUT:
        {
            lux_cmd_ircut((hi_char*)message->obj, message->len);
        }
        break;
    case LUX_CMD_LED:
        {
            lux_cmd_led((hi_char*)message->obj, message->len);
        }
        break;
    case LUX_CMD_PERSONDET:
        {
            lux_cmd_persondet((hi_char*)message->obj, message->len);
        }
        break;
    default:
        break;
    }
    return 0;
}
