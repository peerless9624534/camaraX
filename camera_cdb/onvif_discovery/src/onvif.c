#include <sys/prctl.h>
#include <pthread.h>
#include "soapH.h"    // server stubs, serializers, etc.
#include "wsdd.nsmap" // XML namespace mapping table (only needed once at the global level)
#include "soapStub.h"
#include "func_network.h"
#include "config.h"

#include "func_log.h"
#include "onvif.h"
#include <jwhead.h>

void* onvif_start(void* p);

static void *discovery_thread(void *data)
{
    int count = 0;
    int ret = 0;
    SOAP_SOCKET socket_s, socket_c;
    struct soap ServerSoap;
    struct ip_mreq mcast;

	LOG_DBG(LOG_TAG, "start\n");
    soap_init1(&ServerSoap, SOAP_IO_UDP | SOAP_XML_IGNORENS);
    soap_set_namespaces(&ServerSoap,  namespaces);


    ServerSoap.send_timeout = 100;
    ServerSoap.recv_timeout = 100;
    LOG_DBG(LOG_TAG, "ServerSoap.version = %d\n", ServerSoap.version);
    //soap_register_plugin(&ServerSoap, soap_wsa);
    socket_s = soap_bind(&ServerSoap, NULL, DISCOVERY_LISTEN_PORT, 100);
    if(!soap_valid_socket(socket_s))
    {
	soap_print_fault(&ServerSoap, stderr);
        LOG_ERR(LOG_TAG, "ERROR: soap_bind error! %s\n", strerror(errno));
	exit(1);
    }
    mcast.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
    mcast.imr_interface.s_addr = htonl(INADDR_ANY);

    ret = setsockopt(ServerSoap.master, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mcast, sizeof(mcast));
    if(ret < 0){
	LOG_ERR(LOG_TAG, "setsockopt error! error code = %d,err string = %s\n",errno,strerror(errno));
	return 0;
    }

    for(;;) {
        socket_c = soap_accept(&ServerSoap);
        if (!soap_valid_socket(socket_c)){
            soap_print_fault(&ServerSoap, stderr);
            LOG_ERR(LOG_TAG, "ERROR: valid_socket: socket = %d, %s\n", socket_c, strerror(errno));
        }

		if(soap_serve(&ServerSoap)) {
			soap_print_fault(&ServerSoap, stderr);
		}

		soap_destroy(&ServerSoap);
		soap_end(&ServerSoap);

		LOG_DBG(LOG_TAG, "RECEIVE count %d, connection from IP = %u.%u.%u.%u socket = %d\n",
			count, ((ServerSoap.ip)>>24)&0xFF, ((ServerSoap.ip)>>16)&0xFF,
			((ServerSoap.ip)>>8)&0xFF, (ServerSoap.ip)&0xFF, (ServerSoap. socket));

		count++;
	}

    soap_done(&ServerSoap);

    return NULL;
}

#ifndef DISCOVERY_ONLY
static void *server_thread(void *data)
{
    SOAP_SOCKET socket_s, socket_c;
    struct soap soap;

    soap_init(&soap);
    soap_set_namespaces(&soap, namespaces);
    //soap_set_mode(&soap, SOAP_C_UTFSTRING);
    soap.bind_flags   = SO_REUSEADDR;
    soap.send_timeout = 100; /* 10 sec */
    soap.recv_timeout = 100; /* 10 sec */
    socket_s = soap_bind(&soap, NULL, SERVER_LISTEN_PORT, 100);

    if (!soap_valid_socket(socket_s))   
    {
	    soap_print_fault(&soap, stderr);
	    MYERRORING("ERROR: soap_bind error! %s\n", strerror(errno));
	    return NULL;
    }

    MYTRACEING("Socket connection successful, server socket = %d\n", socket_s);

    for (;;) {
	    socket_c = soap_accept(&soap);
	    if (!soap_valid_socket(socket_c)) 
        {
	        soap_print_fault(&soap, stderr);
	        MYERRORING("ERROR: valid_socket: socket = %d, %s\n", socket_c, strerror(errno));
	        break;
	    }
	    soap_serve(&soap);
	    soap_destroy(&soap);
	    soap_end(&soap);
    }
    soap_done(&soap);

    return NULL;
}

void *onvif_start(void* arg)
{
    printf("********************** onvif_start \n");
    int ret;
    pthread_t discovery_thid, server_thid;

#ifdef DISCOVERY_ONLY
    //prctl(PR_SET_NAME, "onvif_main", 0, 0, 0);
    ret = pthread_create(&discovery_thid, NULL, discovery_thread, NULL);
    if (ret != 0) {
	    MYERROR("pthread_create discovery_thread failed\n");
	    return NULL;
    }
#endif

#ifndef DISCOVERY_ONLY
    ret = pthread_create(&server_thid, NULL, server_thread, NULL);
    if (ret != 0) {
	    MYERROR("pthread_create server_thread failed\n");
	    return NULL;
    }
#endif

#ifdef DISCOVERY_ONLY
    pthread_join(discovery_thid, NULL);
    //pthread_join(server_thid, NULL);
#else
    //pthread_detach(discovery_thid);
    pthread_detach(server_thid);
#endif

	return NULL;
}
#endif


#ifdef DISCOVERY_ONLY
int main(int argc,char ** argv)
{
    LUX_Global_Init();
    //onvif_start(NULL);

    int ret;
    pthread_t discovery_thid, server_thid;

    //prctl(PR_SET_NAME, "onvif_main", 0, 0, 0);
    ret = pthread_create(&discovery_thid, NULL, discovery_thread, NULL);
    if (ret != 0) {
        MYERROR("pthread_create discovery_thread failed\n");
        return NULL;
    }

    pthread_join(discovery_thid, NULL);

    return 0;
}
#endif //DISCOVERY_ONLY
