/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: sample cli file.
 * Author: Hisilicon
 * Create: 2020-09-09
 */
/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/sockios.h>
#include <linux/if.h>
#include <linux/wireless.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "hi_base.h"
#include "lux_server.h"
#include "hichannel_host_comm.h"
#include "lux_cmd.h"

/*****************************************************************************
  2 宏定义、全局变量
*****************************************************************************/
static hi_s32 sample_exit_cmd_process(hi_void *wdata, hi_char *param, hi_u32 len, hi_void *pmsg);
static hi_s32 sample_help_cmd_process(hi_void *wdata, hi_char *param, hi_u32 len, hi_void *pmsg);

static const sample_cmd_entry_stru  g_sample_cmd[] = {
    {"help", sample_help_cmd_process},
    {"exit", sample_exit_cmd_process},
};

#define MAX_CMD_LEN 20
#define MAX_IPV4_LEN 16
#define WIFI_MAC_LEN 6
#define SLEEP_TIME 10
//#define SAMPLE_CMD_NUM (sizeof(g_lux_cmd) / sizeof(g_lux_cmd[0]))
/*****************************************************************************
  3 枚举、结构体定义
*****************************************************************************/


typedef enum {
    HOST_CMD_GET_MAC,
    HOST_CMD_GET_IP,
    HOST_CMD_GET_DNS,
    HOST_CMD_SET_FILTER,
    HOST_CMD_TBTT,
}host_cmd_e;


struct snode {
    sample_message_s message;
    struct snode *next;
};

struct squeue {
    struct snode *front;
    struct snode *rear;
};

typedef struct {
    pthread_mutex_t mut;
    pthread_cond_t cond;
    struct squeue cmd_queue;
    pthread_t sock_thread;
    hi_s32 sockfd;
} sample_link_s;

static hi_bool g_terminate = HI_FALSE;
static sample_link_s *g_sample_link = HI_NULL;
static hi_char host_cmd[][MAX_CMD_LEN] = {"cmd_get_mac", "cmd_get_ip", "cmd_get_dns", "cmd_set_filter"};
/*****************************************************************************
  4 函数实现
*****************************************************************************/
static hi_void sample_usage(hi_void)
{
    printf("\nUsage:\n");
    printf("\tsample_cli  exit          quit sample_ap\n");
    printf("\tsample_cli  help          show this message\n");
}

hi_s32 sample_str_cmd_process(hi_void *wdata, hi_char *param, hi_u32 len, hi_void *pmsg)
{
    sample_unused(wdata);
    sample_message_s *msg = (sample_message_s *)pmsg;
    msg->what = LUX_CMD_IOCTL;
    msg->len = len;
    (hi_void)memcpy(msg->obj, param, len);
    msg->obj[LUX_CMD_MAX_LEN - 1] = '\0';
    return HI_SUCCESS;
}

hi_s32 sample_help_cmd_process(hi_void *wdata, hi_char *param, hi_u32 len, hi_void *pmsg)
{
    sample_unused(wdata);
    sample_unused(param);
    sample_unused(len);
    sample_message_s *msg = (sample_message_s *)pmsg;
    msg->what = LUX_CMD_HELP;
    return HI_SUCCESS;
}

static hi_s32 sample_exit_cmd_process(hi_void *wdata, hi_char *param, hi_u32 len, hi_void *pmsg)
{
    sample_unused(wdata);
    sample_unused(param);
    sample_unused(len);
    sample_message_s *msg = (sample_message_s *)pmsg;
    msg->what = LUX_CMD_EXIT;
    return HI_SUCCESS;
}

static hi_void sample_cleanup(hi_void)
{
    sample_log_print("sample_cleanup enter\n");
    if (g_sample_link->sock_thread) {
        pthread_cancel(g_sample_link->sock_thread);
        pthread_join(g_sample_link->sock_thread, HI_NULL);
    }

    pthread_mutex_destroy(&g_sample_link->mut);
    pthread_cond_destroy(&g_sample_link->cond);

    if (g_sample_link->sockfd != -1) {
        close(g_sample_link->sockfd);
    }

    if (g_sample_link != HI_NULL) {
        free(g_sample_link);
        g_sample_link = HI_NULL;
    }
}

static hi_void sample_terminate(hi_s32 sig)
{
    sample_unused(sig);
    sample_cleanup();
    g_terminate = HI_TRUE;
    _exit(0);
}

static hi_s32 sample_sock_create(hi_void)
{
    if (g_sample_link == HI_NULL) {
        return HI_FAILURE;
    }

    g_sample_link->sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (g_sample_link->sockfd == -1) {
        sample_log_print("error:%s", strerror(errno));
        return HI_FAILURE;
    }

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SOCK_PORT);

    if (bind(g_sample_link->sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        sample_log_print("error:%s", strerror(errno));
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static hi_s32 sample_enqueue(struct squeue *pqueue, const sample_message_s *element)
{
    struct snode *pnew = HI_NULL;

    if (pqueue == HI_NULL || element == HI_NULL) {
        return -1;
    }
    /* Create a new node */
    pnew = malloc(sizeof(struct snode));
    if (pnew == HI_NULL) {
        return -1;
    }

    pnew->message = *element;
    pnew->next = HI_NULL;

    if (pqueue->rear == HI_NULL) {
        /* queue is empty, set front and rear points to new node */
        pqueue->front = pqueue->rear = pnew;
    } else {
        /* queue is not empty, set rear points to the new node */
        pqueue->rear = pqueue->rear->next = pnew;
    }

    return HI_SUCCESS;
}

static hi_s32 sample_dequeue(struct squeue *pqueue, sample_message_s *element)
{
    struct snode *p = HI_NULL;

    if (pqueue == HI_NULL || element == HI_NULL) {
        return HI_FAILURE;
    }

    if (pqueue->front == HI_NULL) {
        return HI_FAILURE;
    }

    *element = pqueue->front->message;
    p = pqueue->front;
    pqueue->front = p->next;
    /* if the queue is empty, set rear = NULL */
    if (pqueue->front == HI_NULL) {
        pqueue->rear = HI_NULL;
    }

    free(p);
    return HI_SUCCESS;
}

static hi_void *sample_sock_thread(hi_void *args)
{
    hi_char link_buf[SOCK_BUF_MAX];
    ssize_t recvbytes;
    sample_message_s message;
    struct sockaddr_in clientaddr;
    socklen_t addrlen = sizeof(clientaddr);
    sample_unused(args);

    while (1) {
        /* 安全编程规则6.6例外(1) 对固定长度的数组进行初始化，或对固定长度的结构体进行内存初始化 */
        (hi_void)memset(link_buf, 0, sizeof(link_buf));
        /* 安全编程规则6.6例外(1) 对固定长度的数组进行初始化，或对固定长度的结构体进行内存初始化 */
        (hi_void)memset(&clientaddr, 0, addrlen);
        (hi_void)memset(&message, 0, sizeof(sample_message_s));

        recvbytes = recvfrom(g_sample_link->sockfd, link_buf, sizeof(link_buf), 0,
                             (struct sockaddr *)&clientaddr, &addrlen);
        if (recvbytes < 0) {
            if (errno == EINTR) {
                usleep(SLEEP_TIME);
                continue;
            } else {
                sample_log_print("recvfrom error! fd:%d\n", g_sample_link->sockfd);
                return HI_NULL;
            }
        }
        if (sample_sock_cmd_entry(g_sample_link, link_buf, recvbytes, (hi_void *)&message) != HI_SUCCESS) {
            sample_log_print("command not found entry\n");
            sample_str_cmd_process(g_sample_link, link_buf, recvbytes, (hi_void *)&message);
        }

        if (sendto(g_sample_link->sockfd, "OK", strlen("OK"), MSG_DONTWAIT, (const struct sockaddr *)&clientaddr,
                   addrlen) == -1) {
            sample_log_print("sendto error!fd:%d\n", g_sample_link->sockfd);
        }

        pthread_mutex_lock(&g_sample_link->mut);
        if (sample_enqueue(&g_sample_link->cmd_queue, &message) == HI_SUCCESS) {
            pthread_cond_signal(&g_sample_link->cond);
        }
        pthread_mutex_unlock(&g_sample_link->mut);
    }
}


void main_process(void)
{
    /* main loop */
    while (!g_terminate) {
        sample_message_s message;
        pthread_mutex_lock(&g_sample_link->mut);
        while (sample_dequeue(&g_sample_link->cmd_queue, &message) != HI_SUCCESS) {
            pthread_cond_wait(&g_sample_link->cond, &g_sample_link->mut);
        }
        pthread_mutex_unlock(&g_sample_link->mut);
        sample_log_print("RECIEVE MSG:%s LEN:%d\n", (char*)message.obj, message.len);
        if(message.what == LUX_CMD_EXIT)
        {
            g_terminate = HI_TRUE;
            continue;
        }
        lux_cmd_exec(&message);
        #if 0
        switch (message.what) {
            case LUX_CMD_HELP:
                sample_usage();
                break;
            case LUX_CMD_EXIT:
                sample_log_print("exit!!!\n");
                g_terminate = HI_TRUE;
                break;
            case LUX_CMD_IOCTL:
                printf("unkonown cmd: %s\n", (char*)message.obj);
                break;
            default:
                break;
        }
    #endif
    }
    sample_log_print("exited!!!\n");
}

int lux_server_start()
{
    hi_s32 ret;

    signal(SIGINT, sample_terminate);
    signal(SIGTERM, sample_terminate);

    g_sample_link = (sample_link_s *)malloc(sizeof(sample_link_s));
    if (g_sample_link == HI_NULL) {
        return -1;
    }

    (void)memset(g_sample_link, 0, sizeof(sample_link_s));
    pthread_mutex_init(&g_sample_link->mut, HI_NULL);
    pthread_cond_init(&g_sample_link->cond, HI_NULL);

    if (sample_register_cmd((sample_cmd_entry_stru *)&g_lux_cmd, g_lux_cmd_cnt) != HI_SUCCESS) {
        sample_log_print("register wlan cmd is fail\n");
        goto link_out;
    }

    if (sample_sock_create() != HI_SUCCESS) {
        sample_log_print("create sock is fail\n");
        goto link_out;
    }

    ret = pthread_create(&g_sample_link->sock_thread, HI_NULL, sample_sock_thread, HI_NULL);
    if (ret != HI_SUCCESS) {
        sample_log_print("create sock thread is fail\n");
        goto link_out;
    }

    main_process();

link_out:
    sample_cleanup();
    return 0;
}

