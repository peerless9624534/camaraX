#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/reboot.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/route.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdbool.h>
#include <signal.h>
#include "soapStub.h"    // server stubs, serializers, etc.
#include "stdsoap2.h"
#include "soapH.h"
#include "authenticate.h"
#include "lux_global.h"
#include <AppVersion.h>
#include "func_log.h"
#include "func_network.h"
#include "func_com.h"
#include "func_conf.h"
#include "lux_base.h"

#ifndef DISCOVERY_ONLY
#include "func_video.h"
#include "func_osd.h"
#include "lux_osd.h"
#include "lux_time.h"
#include <imp_isp.h>
#endif
#include "lux_config.h"
#include "lux_users.h"
#define LOG_TAG "ONVIF"
// 主题：
// tns1:RuleEngine/TamperDetector/Tamper # 遮挡报警
// tns1:RuleEngine/CellMotionDetector/Motion # 移动侦测
// tns1:RuleEngine/FieldDetector/ObjectsInside # 区域检测
// tns1:RuleEngine/LineDetector/Crossed # 绊线检测
// 能力集
// WSSubscriptionPolicySupport # 表明设备是否支持WS订阅策略
// WSPullPointSupport # 表明设备是否支持WS拉点推送
// WSPausableSubscriptionManagerInterfaceSupport # 表明设备支持WS可暂停订阅管理接口
// MaxNotificationProducers #  WS-BaseNotification定义的支持通知生产者的最大数量。
// MaxPullPoints # 受支持的最大通知拉点数
// PersistentNotificationStorage #是否支持持久通知存储

#define TAMPER_TOPIC            "tns1:RuleEngine/TamperDetector/Tamper"
#define TAMPER_NAME             "IsTamper"
#define TAMPER_VALUE            "true"

static void *soap_zalloc(struct soap *soap, size_t n)
{
    void *p = soap_malloc(soap, n);
    if( p == NULL ) {
	LOG_ERR(LOG_TAG, " soap_zalloc error \n");
	return NULL;
    }
    memset(p, 0x00, n);
    return p;
}

int find_event(struct _tev__PullMessagesResponse *rep, char *topic, char *name, char *value)
{
    int i, j;

    if(NULL == rep) {
        return 0;
    }

    for (i = 0; i < rep->__sizeNotificationMessage; i++) {
        struct wsnt__NotificationMessageHolderType *p = rep->wsnt__NotificationMessage + i;

        if (NULL == p->Topic) {
            continue;
        }
        if (NULL == p->Topic->__mixed ) {
            continue;
        }
        if (0 != strcmp(topic, p->Topic->__mixed)) {
            continue;
        }

        if (NULL == p->Message.tt__Message) {
            continue;
        }
        if (NULL == p->Message.tt__Message->Data) {
            continue;
        }
        if (NULL == p->Message.tt__Message->Data->SimpleItem) {
            continue;
        }
        for (j = 0; j < p->Message.tt__Message->Data->__sizeSimpleItem; j++) {
            struct _tt__ItemList_SimpleItem *a = p->Message.tt__Message->Data->SimpleItem + j;
            if (NULL == a->Name || NULL == a->Value) {
                continue;
            }
            if (0 != strcmp(name, a->Name)) {
                continue;
            }
            if (0 != strcmp(value, a->Value)) {
                continue;
            }

            return 1;
        }
    }


    return 0;
}

int IsTamper(struct _tev__PullMessagesResponse *rep)
{
    return find_event(rep, TAMPER_TOPIC, TAMPER_NAME, TAMPER_VALUE);
}

void debug(int __size,char **__any) {
    LOG_DBG(LOG_TAG,"__size: %d\n",__size);
    
    for (int i = 0; i < __size; i++) {
        LOG_DBG(LOG_TAG,"__any[%d]: %s\n", i, __any[i]);
    }
}
/******************************************************************************\
 *                                                                            *
 * Server-Side Operations                                                     *
 *                                                                            *
\******************************************************************************/

    /** Web service operation '__tev__PullMessages' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__PullMessages(struct soap* soap, struct _tev__PullMessages *tev__PullMessages, struct _tev__PullMessagesResponse *tev__PullMessagesResponse)
    {
        LOG_DBG(LOG_TAG, "\n");

        int ret, i;
        char ip[20];
        ret = LUX_Network_getip2((char*)"eth0", ip); //eth0
        if (ret < 0) {
            LOG_ERR(LOG_TAG, "ERROR: can't get host ip address!\n");
            return SOAP_ERR;
        }

        time_t CurrentTime;
        CurrentTime = time_ms() / 1000ULL;
        tev__PullMessagesResponse->CurrentTime = CurrentTime;
        tev__PullMessagesResponse->TerminationTime = CurrentTime + 60ULL;

        struct wsnt__NotificationMessageHolderType* wsnt__NotificationMessage = (struct wsnt__NotificationMessageHolderType*)soap_zalloc(soap, sizeof(struct wsnt__NotificationMessageHolderType));
        wsnt__NotificationMessage->Topic = (struct wsnt__TopicExpressionType*)soap_zalloc(soap, sizeof(struct wsnt__TopicExpressionType));
        wsnt__NotificationMessage->Topic->__mixed = (char*)soap_zalloc(soap, 100);
        sprintf(wsnt__NotificationMessage->Topic->__mixed, "%s", TAMPER_TOPIC);
        wsnt__NotificationMessage->Message.tt__Message = (struct _tt__Message*)soap_zalloc(soap, sizeof(struct _tt__Message));
        wsnt__NotificationMessage->Message.tt__Message->Data = (struct tt__ItemList*)soap_zalloc(soap, sizeof(struct tt__ItemList));
        wsnt__NotificationMessage->Message.tt__Message->Data->SimpleItem = (struct _tt__ItemList_SimpleItem*)soap_zalloc(soap, sizeof(struct _tt__ItemList_SimpleItem));
        wsnt__NotificationMessage->Message.tt__Message->Data->__sizeSimpleItem = (int)soap_zalloc(soap, 100);
        wsnt__NotificationMessage->Message.tt__Message->Data->SimpleItem->Name = (char*)soap_zalloc(soap, 100);
        wsnt__NotificationMessage->Message.tt__Message->Data->SimpleItem->Value = (char*)soap_zalloc(soap, 100);
        strcpy(wsnt__NotificationMessage->Message.tt__Message->Data->SimpleItem->Name,  TAMPER_NAME);
        strcpy(wsnt__NotificationMessage->Message.tt__Message->Data->SimpleItem->Value, TAMPER_VALUE);
        wsnt__NotificationMessage->SubscriptionReference = (struct wsa5__EndpointReferenceType*)soap_zalloc(soap, sizeof(struct wsa5__EndpointReferenceType));
        //wsnt__NotificationMessage->SubscriptionReference->__sizeNotificationMessage =10;
        wsnt__NotificationMessage->SubscriptionReference->Address = (char*)soap_zalloc(soap, 100);
        sprintf(wsnt__NotificationMessage->SubscriptionReference->Address, "%s", ip);
        // wsnt__NotificationMessage->SubscriptionReference->__size;
        wsnt__NotificationMessage->SubscriptionReference->__any = (char*)soap_zalloc(soap, 100);
        wsnt__NotificationMessage->SubscriptionReference->__anyAttribute = (char*)soap_zalloc(soap, 100);

        wsnt__NotificationMessage->SubscriptionReference->ReferenceParameters = (struct wsa5__ReferenceParametersType*)soap_zalloc(soap, sizeof(struct wsa5__ReferenceParametersType));

        // *wsnt__NotificationMessage->SubscriptionReference->ReferenceParameters->__ChannelInstance;
        // wsnt__NotificationMessage->SubscriptionReference->ReferenceParameters->__size;
        wsnt__NotificationMessage->SubscriptionReference->ReferenceParameters->__any = (char*)soap_zalloc(soap, 100);
        wsnt__NotificationMessage->SubscriptionReference->ReferenceParameters->__anyAttribute = (char*)soap_zalloc(soap, 100);

        wsnt__NotificationMessage->SubscriptionReference->Metadata = (struct wsa5__MetadataType*)soap_zalloc(soap, sizeof(struct wsa5__MetadataType));

        // wsnt__NotificationMessage->SubscriptionReference->Metadata->__size;
        wsnt__NotificationMessage->SubscriptionReference->Metadata->__any = (char*)soap_zalloc(soap, 100);
        wsnt__NotificationMessage->SubscriptionReference->Metadata->__anyAttribute = (char*)soap_zalloc(soap, 100);
        // wsnt__NotificationMessage->Topic = (struct wsnt__TopicExpressionType *)soap_zalloc(soap, sizeof(struct wsnt__TopicExpressionType));
        //        wsnt__NotificationMessage->Topic->_any= (char *)soap_zalloc(soap, 100);
        //        wsnt__NotificationMessage->Topic->Dialect= (char *)soap_zalloc(soap, 100);
        //        wsnt__NotificationMessage->Topic->__anyAttribute= (char *)soap_zalloc(soap, 100);
        //        wsnt__NotificationMessage->Topic->__mixed= (char *)soap_zalloc(soap, 100);


        wsnt__NotificationMessage->ProducerReference = (struct wsa5__EndpointReferenceType*)soap_zalloc(soap, sizeof(struct wsa5__EndpointReferenceType));

        wsnt__NotificationMessage->ProducerReference->Address = (char*)soap_zalloc(soap, 100);
        sprintf(wsnt__NotificationMessage->ProducerReference->Address, "%s", ip);
        // wsnt__NotificationMessage->ProducerReference->__size;
        wsnt__NotificationMessage->ProducerReference->__any = (char*)soap_zalloc(soap, 100);
        wsnt__NotificationMessage->ProducerReference->__anyAttribute = (char*)soap_zalloc(soap, 100);

        wsnt__NotificationMessage->ProducerReference->ReferenceParameters = (struct wsa5__ReferenceParametersType*)soap_zalloc(soap, sizeof(struct wsa5__ReferenceParametersType));

        // *wsnt__NotificationMessage->ProducerReference->ReferenceParameters->__ChannelInstance;
        // wsnt__NotificationMessage->ProducerReference->ReferenceParameters->__size;
        wsnt__NotificationMessage->ProducerReference->ReferenceParameters->__any = (char*)soap_zalloc(soap, 100);
        wsnt__NotificationMessage->ProducerReference->ReferenceParameters->__anyAttribute = (char*)soap_zalloc(soap, 100);

        wsnt__NotificationMessage->ProducerReference->Metadata = (struct wsa5__MetadataType*)soap_zalloc(soap, sizeof(struct wsa5__MetadataType));

        // wsnt__NotificationMessage->ProducerReference->Metadata->__size;
        wsnt__NotificationMessage->ProducerReference->Metadata->__any = (char*)soap_zalloc(soap, 100);
        wsnt__NotificationMessage->ProducerReference->Metadata->__anyAttribute = (char*)soap_zalloc(soap, 100);

        // wsnt__NotificationMessage->Message = (struct _wsnt__NotificationMessageHolderType_Message )soap_zalloc(soap, sizeof(struct _wsnt__NotificationMessageHolderType_Message));
        // wsnt__NotificationMessage->Message.__any = (char*)soap_zalloc(soap, 100);

        // wsnt__NotificationMessage->Message.tt__Message = (struct _tt__Message*)soap_zalloc(soap, sizeof(struct _tt__Message));

        wsnt__NotificationMessage->Message.tt__Message->UtcTime = time_ms() / 1000ULL;
        wsnt__NotificationMessage->Message.tt__Message->PropertyOperation = tt__PropertyOperation__Initialized;
        wsnt__NotificationMessage->Message.tt__Message->__anyAttribute = (char*)soap_zalloc(soap, 100);

        wsnt__NotificationMessage->Message.tt__Message->Source = (struct tt__ItemList*)soap_zalloc(soap, sizeof(struct tt__ItemList));
        wsnt__NotificationMessage->Message.tt__Message->Key = (struct tt__ItemList*)soap_zalloc(soap, sizeof(struct tt__ItemList));
        // wsnt__NotificationMessage->Message.tt__Message->Data = (struct tt__ItemList*)soap_zalloc(soap, sizeof(struct tt__ItemList));
        // wsnt__NotificationMessage->Message.tt__Message->Source->__sizeSimpleItem;
        // wsnt__NotificationMessage->Message.tt__Message->Source->__sizeElementItem;
        wsnt__NotificationMessage->Message.tt__Message->Source->__anyAttribute = (char*)soap_zalloc(soap, 100);

        wsnt__NotificationMessage->Message.tt__Message->Source->SimpleItem = (struct tt__ItemList*)soap_zalloc(soap, sizeof(struct tt__ItemList));
        wsnt__NotificationMessage->Message.tt__Message->Source->SimpleItem->Name = (char*)soap_zalloc(soap, 100);
        wsnt__NotificationMessage->Message.tt__Message->Source->SimpleItem->Value = (char*)soap_zalloc(soap, 100);


        wsnt__NotificationMessage->Message.tt__Message->Source->ElementItem = (struct _tt__ItemList_ElementItem*)soap_zalloc(soap, sizeof(struct _tt__ItemList_ElementItem));
        wsnt__NotificationMessage->Message.tt__Message->Source->ElementItem->__any = (char*)soap_zalloc(soap, 100);
        wsnt__NotificationMessage->Message.tt__Message->Source->ElementItem->Name = (char*)soap_zalloc(soap, 100);
        wsnt__NotificationMessage->Message.tt__Message->Source->Extension = (struct tt__ItemListExtension*)soap_zalloc(soap, sizeof(struct tt__ItemListExtension));
        //  wsnt__NotificationMessage->Message.tt__Message->Source->Extension->__size;
        wsnt__NotificationMessage->Message.tt__Message->Source->Extension->__any = (char*)soap_zalloc(soap, 100);
        strcpy(wsnt__NotificationMessage->Message.tt__Message->Source->SimpleItem->Name,  TAMPER_NAME);
        strcpy(wsnt__NotificationMessage->Message.tt__Message->Source->SimpleItem->Value,  TAMPER_VALUE);

        // wsnt__NotificationMessage->Message.tt__Message->Key->__sizeSimpleItem;
        // wsnt__NotificationMessage->Message.tt__Message->Key->__sizeElementItem;
        wsnt__NotificationMessage->Message.tt__Message->Key->__anyAttribute = (char*)soap_zalloc(soap, 100);

        wsnt__NotificationMessage->Message.tt__Message->Key->SimpleItem = (struct tt__ItemList*)soap_zalloc(soap, sizeof(struct tt__ItemList));
        wsnt__NotificationMessage->Message.tt__Message->Key->SimpleItem->Name = (char*)soap_zalloc(soap, 100);
        wsnt__NotificationMessage->Message.tt__Message->Key->SimpleItem->Value = (char*)soap_zalloc(soap, 100);

        wsnt__NotificationMessage->Message.tt__Message->Key->ElementItem = (struct _tt__ItemList_ElementItem*)soap_zalloc(soap, sizeof(struct _tt__ItemList_ElementItem));
        wsnt__NotificationMessage->Message.tt__Message->Key->ElementItem->__any = (char*)soap_zalloc(soap, 100);
        wsnt__NotificationMessage->Message.tt__Message->Key->ElementItem->Name = (char*)soap_zalloc(soap, 100);
        wsnt__NotificationMessage->Message.tt__Message->Key->Extension = (struct tt__ItemListExtension*)soap_zalloc(soap, sizeof(struct tt__ItemListExtension));
        //  wsnt__NotificationMessage->Message.tt__Message->Key->Extension->__size;
        wsnt__NotificationMessage->Message.tt__Message->Key->Extension->__any = (char*)soap_zalloc(soap, 100);



        // wsnt__NotificationMessage->Message.tt__Message->Data->__sizeSimpleItem;
        // wsnt__NotificationMessage->Message.tt__Message->Data->__sizeElementItem;
        // wsnt__NotificationMessage->Message.tt__Message->Data->__anyAttribute = (char*)soap_zalloc(soap, 100);

        // wsnt__NotificationMessage->Message.tt__Message->Data->SimpleItem = (struct tt__ItemList*)soap_zalloc(soap, sizeof(struct tt__ItemList));
        // wsnt__NotificationMessage->Message.tt__Message->Data->SimpleItem->Name = (char*)soap_zalloc(soap, 100);
        // wsnt__NotificationMessage->Message.tt__Message->Data->SimpleItem->Value = (char*)soap_zalloc(soap, 100);

        // wsnt__NotificationMessage->Message.tt__Message->Data->ElementItem = (struct _tt__ItemList_ElementItem*)soap_zalloc(soap, sizeof(struct _tt__ItemList_ElementItem));
        // wsnt__NotificationMessage->Message.tt__Message->Data->ElementItem->__any = (char*)soap_zalloc(soap, 100);
        // wsnt__NotificationMessage->Message.tt__Message->Data->ElementItem->Name = (char*)soap_zalloc(soap, 100);
        // wsnt__NotificationMessage->Message.tt__Message->Data->Extension = (struct tt__ItemListExtension*)soap_zalloc(soap, sizeof(struct tt__ItemListExtension));
        // //  wsnt__NotificationMessage->Message.tt__Message->Data->Extension->__size;
        // wsnt__NotificationMessage->Message.tt__Message->Data->Extension->__any = (char*)soap_zalloc(soap, 100);


        // wsnt__NotificationMessage->Message.tt__Message->Extension = (struct tt__MessageExtension*)soap_zalloc(soap, sizeof(struct tt__MessageExtension));
        // //  wsnt__NotificationMessage->Message.tt__Message-->Extension->__size;
        // wsnt__NotificationMessage->Message.tt__Message->Extension->__any = (char*)soap_zalloc(soap, 100);
        tev__PullMessagesResponse->wsnt__NotificationMessage = wsnt__NotificationMessage;


        return SOAP_OK;
    }
    /** Web service operation '__tev__Seek' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__Seek(struct soap* soap, struct _tev__Seek *tev__Seek, struct _tev__SeekResponse *tev__SeekResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tev__SetSynchronizationPoint' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__SetSynchronizationPoint(struct soap* soap, struct _tev__SetSynchronizationPoint *tev__SetSynchronizationPoint, struct _tev__SetSynchronizationPointResponse *tev__SetSynchronizationPointResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tev__Unsubscribe' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__Unsubscribe(struct soap* soap, struct _wsnt__Unsubscribe *wsnt__Unsubscribe, struct _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse){
    LOG_DBG(LOG_TAG, "\n");

    debug(wsnt__Unsubscribe->__size,wsnt__Unsubscribe->__any);

    return SOAP_OK;
}
    /** Web service operation '__tev__GetServiceCapabilities' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__GetServiceCapabilities(struct soap* soap, struct _tev__GetServiceCapabilities *tev__GetServiceCapabilities, struct _tev__GetServiceCapabilitiesResponse *tev__GetServiceCapabilitiesResponse){
    LOG_DBG(LOG_TAG, "\n");
    int i;
    struct tev__Capabilities *Capabilities = (struct tev__Capabilities *)soap_zalloc(soap, sizeof(struct tev__Capabilities));    
    Capabilities->__size = 30;
    Capabilities->__any = (char *)soap_zalloc(soap, 100);
    Capabilities->WSSubscriptionPolicySupport = (enum xsd__boolean *)soap_zalloc(soap, sizeof(enum xsd__boolean));
    Capabilities->WSSubscriptionPolicySupport = xsd__boolean__true_;
    Capabilities->WSPullPointSupport = (enum xsd__boolean *)soap_zalloc(soap, sizeof(enum xsd__boolean));
    Capabilities->WSPullPointSupport = xsd__boolean__true_;
    Capabilities->WSPausableSubscriptionManagerInterfaceSupport = (enum xsd__boolean *)soap_zalloc(soap, sizeof(enum xsd__boolean));
    Capabilities->WSPausableSubscriptionManagerInterfaceSupport =  xsd__boolean__true_;
    Capabilities->MaxNotificationProducers = (enum xsd__boolean *)soap_zalloc(soap, sizeof(enum xsd__boolean));
    Capabilities->MaxNotificationProducers =  xsd__boolean__true_;
    Capabilities->MaxPullPoints = 20;//允许的最大 Pull Point（拉取点）数量
    Capabilities->PersistentNotificationStorage = (enum xsd__boolean *)soap_zalloc(soap, sizeof(enum xsd__boolean));
    Capabilities->PersistentNotificationStorage = xsd__boolean__true_;
    Capabilities->EventBrokerProtocols =(char *)soap_zalloc(soap, 100);
    Capabilities->EventBrokerProtocols = "MQTT, HTTP";//事件代理协议；
    Capabilities->MaxEventBrokers = 10;
    Capabilities->MetadataOverMQTT = xsd__boolean__true_;//是否支持mqtt发布元数据
    Capabilities->__anyAttribute = (char *)soap_zalloc(soap, 100);
    tev__GetServiceCapabilitiesResponse->Capabilities = Capabilities;
    return SOAP_OK;
}
    /** Web service operation '__tev__CreatePullPointSubscription' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__CreatePullPointSubscription(struct soap* soap, struct _tev__CreatePullPointSubscription *tev__CreatePullPointSubscription, struct _tev__CreatePullPointSubscriptionResponse *tev__CreatePullPointSubscriptionResponse){
        LOG_DBG(LOG_TAG, "\n");
        int ret, i;
        char ip[20];
        ret = LUX_Network_getip2((char*)"eth0", ip); //eth0
        if (ret < 0) {
            LOG_ERR(LOG_TAG, "ERROR: can't get host ip address!\n");
            return SOAP_ERR;
        }

        debug(tev__CreatePullPointSubscription->__size, tev__CreatePullPointSubscription->__any);
        LOG_DBG(LOG_TAG, "tev__CreatePullPointSubscription->InitialTerminationTime = %s\n", tev__CreatePullPointSubscription->InitialTerminationTime);
        // debug(tev__CreatePullPointSubscription->Filter->__size, tev__CreatePullPointSubscription->Filter->__any);
        LOG_DBG(LOG_TAG, "TopicExpression->__any= %s\n", tev__CreatePullPointSubscription->Filter->TopicExpression->__any);
        LOG_DBG(LOG_TAG, "TopicExpression->Dialect= %s\n", tev__CreatePullPointSubscription->Filter->TopicExpression->Dialect);
        LOG_DBG(LOG_TAG, "TopicExpression->__anyAttribute= %s\n", tev__CreatePullPointSubscription->Filter->TopicExpression->__anyAttribute);
        LOG_DBG(LOG_TAG, "TopicExpression->__mixed= %s\n", tev__CreatePullPointSubscription->Filter->TopicExpression->__mixed);
        //debug(tev__CreatePullPointSubscription->SubscriptionPolicy->__size, tev__CreatePullPointSubscription->SubscriptionPolicy->__any);


        tev__CreatePullPointSubscriptionResponse->wsnt__CurrentTime = time_ms() / 1000ULL;
        tev__CreatePullPointSubscriptionResponse->wsnt__TerminationTime = tev__CreatePullPointSubscriptionResponse->wsnt__CurrentTime + 60ULL;
        tev__CreatePullPointSubscriptionResponse->SubscriptionReference.Address = (char*)soap_zalloc(soap, 100);
        sprintf(tev__CreatePullPointSubscriptionResponse->SubscriptionReference.Address, "%s", ip);
       tev__CreatePullPointSubscriptionResponse->SubscriptionReference.__size = 20;
       tev__CreatePullPointSubscriptionResponse->SubscriptionReference.__any = (char*)soap_zalloc(soap, 100);
       tev__CreatePullPointSubscriptionResponse->SubscriptionReference.__anyAttribute = (char*)soap_zalloc(soap, 100);
       tev__CreatePullPointSubscriptionResponse->SubscriptionReference.ReferenceParameters = (struct wsa5__ReferenceParametersType*)soap_zalloc(soap, sizeof(struct wsa5__ReferenceParametersType));
    //    tev__CreatePullPointSubscriptionResponse->SubscriptionReference.ReferenceParameters->chan__ChannelInstance = 20;
       tev__CreatePullPointSubscriptionResponse->SubscriptionReference.ReferenceParameters->__size = 20;
       tev__CreatePullPointSubscriptionResponse->SubscriptionReference.ReferenceParameters->__any = (char*)soap_zalloc(soap, 100);
       tev__CreatePullPointSubscriptionResponse->SubscriptionReference.ReferenceParameters->__anyAttribute = (char*)soap_zalloc(soap, 100);
       tev__CreatePullPointSubscriptionResponse->SubscriptionReference.Metadata = (struct wsa5__MetadataType*)soap_zalloc(soap, sizeof(struct wsa5__MetadataType));
       tev__CreatePullPointSubscriptionResponse->SubscriptionReference.Metadata->__size = 20;
       tev__CreatePullPointSubscriptionResponse->SubscriptionReference.Metadata->__any = (char*)soap_zalloc(soap, 100);
       tev__CreatePullPointSubscriptionResponse->SubscriptionReference.Metadata->__anyAttribute = (char*)soap_zalloc(soap, 100);

        return SOAP_OK;

}
    /** Web service operation '__tev__GetEventProperties' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__GetEventProperties(struct soap* soap, struct _tev__GetEventProperties *tev__GetEventProperties, struct _tev__GetEventPropertiesResponse *tev__GetEventPropertiesResponse){
    LOG_DBG(LOG_TAG, "\n");
    
    tev__GetEventPropertiesResponse->__sizeTopicNamespaceLocation = 30; //主题名称空间尺寸
    tev__GetEventPropertiesResponse->wsnt__FixedTopicSet = (enum xsd__boolean )soap_zalloc(soap, sizeof(enum xsd__boolean));
    tev__GetEventPropertiesResponse->wsnt__FixedTopicSet = xsd__boolean__true_;//固定主题集
    struct wstop__TopicSetType *wstop__TopicSet = (struct wstop__TopicSetType *)soap_zalloc(soap, sizeof(struct wstop__TopicSetType));  
    wstop__TopicSet->documentation = (struct wstop__Documentation *)soap_zalloc(soap, sizeof(struct wstop__Documentation)); //凭证
    wstop__TopicSet->documentation->__size = 10;
    wstop__TopicSet->documentation->__any = (char *)soap_zalloc(soap, 100);
    wstop__TopicSet->documentation->__mixed = (char *)soap_zalloc(soap, 100);
    wstop__TopicSet->__anyAttribute = (char *)soap_zalloc(soap, 100);
    wstop__TopicSet->__any = (char *)soap_zalloc(soap, 100);
    wstop__TopicSet->__mixed = (char *)soap_zalloc(soap, 100);
    //tev__GetEventPropertiesResponse->__sizeTopicExpressionDialect = 100;//主题方言
    tev__GetEventPropertiesResponse->wsnt__TopicExpressionDialect = (char *)soap_zalloc(soap, 100);
    strcpy(tev__GetEventPropertiesResponse->wsnt__TopicExpressionDialect, "http://www.onvif.org/ver10/tev/topicExpression/ConcreteSet");
    //tev__GetEventPropertiesResponse->__sizeMessageContentFilterDialect =100;
    tev__GetEventPropertiesResponse->MessageContentFilterDialect =(char *)soap_zalloc(soap, 100);       //消息内容过滤
    strcpy(tev__GetEventPropertiesResponse->MessageContentFilterDialect, "http://www.onvif.org/ver10/tev/messageContentFilter/ItemFilter");
    //tev__GetEventPropertiesResponse->__sizeProducerPropertiesFilterDialect=100;      //生产者属性过滤器
    tev__GetEventPropertiesResponse->ProducerPropertiesFilterDialect =(char *)soap_zalloc(soap, 100); 
    //tev__GetEventPropertiesResponse->__sizeMessageContentSchemaLocation =100;            //设置消息内容模式位置
    tev__GetEventPropertiesResponse->MessageContentSchemaLocation = (char *)soap_zalloc(soap, 100);
    strcpy(tev__GetEventPropertiesResponse->MessageContentFilterDialect, "http://www.onvif.org/onvif/ver10/schema/onvif.xsd");
    //tev__GetEventPropertiesResponse->__size =100;
    tev__GetEventPropertiesResponse->__any=(char *)soap_zalloc(soap, 80);
    tev__GetEventPropertiesResponse->wstop__TopicSet = wstop__TopicSet;
    return SOAP_OK;
}
    /** Web service operation '__tev__AddEventBroker' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__AddEventBroker(struct soap* soap, struct _tev__AddEventBroker *tev__AddEventBroker, struct _tev__AddEventBrokerResponse *tev__AddEventBrokerResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tev__DeleteEventBroker' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__DeleteEventBroker(struct soap* soap, struct _tev__DeleteEventBroker *tev__DeleteEventBroker, struct _tev__DeleteEventBrokerResponse *tev__DeleteEventBrokerResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tev__GetEventBrokers' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__GetEventBrokers(struct soap* soap, struct _tev__GetEventBrokers *tev__GetEventBrokers, struct _tev__GetEventBrokersResponse *tev__GetEventBrokersResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tev__Renew' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__Renew(struct soap* soap, struct _wsnt__Renew *wsnt__Renew, struct _wsnt__RenewResponse *wsnt__RenewResponse){
    LOG_DBG(LOG_TAG, "\n");

    debug(wsnt__Renew->__size,wsnt__Renew->__any);

    time_t CTime;
    CTime = time_ms() / 1000ULL;
    wsnt__RenewResponse->TerminationTime = CTime + 60;
    wsnt__RenewResponse->__size = 10;
    wsnt__RenewResponse->__any = (char *)soap_zalloc(soap, 100);

    return SOAP_OK;
}
    /** Web service operation '__tev__Unsubscribe_' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__Unsubscribe_(struct soap* soap, struct _wsnt__Unsubscribe *wsnt__Unsubscribe, struct _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tev__Subscribe' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__Subscribe(struct soap* soap, struct _wsnt__Subscribe *wsnt__Subscribe, struct _wsnt__SubscribeResponse *wsnt__SubscribeResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tev__GetCurrentMessage' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__GetCurrentMessage(struct soap* soap, struct _wsnt__GetCurrentMessage *wsnt__GetCurrentMessage, struct _wsnt__GetCurrentMessageResponse *wsnt__GetCurrentMessageResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service one-way operation '__tev__Notify' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__Notify(struct soap* soap, struct _wsnt__Notify *wsnt__Notify){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tev__GetMessages' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__GetMessages(struct soap* soap, struct _wsnt__GetMessages *wsnt__GetMessages, struct _wsnt__GetMessagesResponse *wsnt__GetMessagesResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tev__DestroyPullPoint' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__DestroyPullPoint(struct soap* soap, struct _wsnt__DestroyPullPoint *wsnt__DestroyPullPoint, struct _wsnt__DestroyPullPointResponse *wsnt__DestroyPullPointResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service one-way operation '__tev__Notify_' implementation, should return value of soap_send_empty_response() to send HTTP Accept acknowledgment, or return an error code, or return SOAP_OK to immediately return without sending an HTTP response message */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__Notify_(struct soap* soap, struct _wsnt__Notify *wsnt__Notify){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tev__CreatePullPoint' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__CreatePullPoint(struct soap* soap, struct _wsnt__CreatePullPoint *wsnt__CreatePullPoint, struct _wsnt__CreatePullPointResponse *wsnt__CreatePullPointResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tev__Renew_' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__Renew_(struct soap* soap, struct _wsnt__Renew *wsnt__Renew, struct _wsnt__RenewResponse *wsnt__RenewResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tev__Unsubscribe__' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__Unsubscribe__(struct soap* soap, struct _wsnt__Unsubscribe *wsnt__Unsubscribe, struct _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tev__PauseSubscription' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__PauseSubscription(struct soap* soap, struct _wsnt__PauseSubscription *wsnt__PauseSubscription, struct _wsnt__PauseSubscriptionResponse *wsnt__PauseSubscriptionResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}
    /** Web service operation '__tev__ResumeSubscription' implementation, should return SOAP_OK or error code */
    SOAP_FMAC5 int SOAP_FMAC6 __tev__ResumeSubscription(struct soap* soap, struct _wsnt__ResumeSubscription *wsnt__ResumeSubscription, struct _wsnt__ResumeSubscriptionResponse *wsnt__ResumeSubscriptionResponse){
    LOG_DBG(LOG_TAG, "\n");
    return SOAP_OK;
}


