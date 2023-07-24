#ifndef __NET_UTILS_H_INCLUDED__
#define __NET_UTILS_H_INCLUDED__

#include <netinet/in.h>
#include <sys/socket.h>

struct udp4_client_t {
    int socket;
    int peerlen;
    struct sockaddr_in peer;
};
struct idip_hwaddr;

#ifdef __cplusplus
extern "C" {
#endif

int isvalidsock(int sockfd);

int set_nonblocking(int fd);

int set_address(const char* hostName, const char* serviceName,
    struct sockaddr_in* sa, const char* protocol);

///
int tcp_server_socket(struct sockaddr* sockaddr);

///
int tcp4_server(const char* hostName, const char* serviceName);
int tcp4_server_n(const char* hostName, int port);

///
int udp4_server(const char* hostName, const char* serviceName);
int udp4_server_n(const char* hostName, int port);

struct udp4_client_t* udp4_client(const char* hostName, const char* serviceName);
struct udp4_client_t* udp4_client_n(const char* hostName, int port);
int udp4_client_fd(const struct udp4_client_t* self);
int udp4_client_sendto(struct udp4_client_t* self, const char* msg, size_t len);

///
int unix_domain_server(const char* path);

///
char* peer_name(int fd, const struct sockaddr* peer, socklen_t peerlen,
                char* name, size_t namelen);

const char* idip_mac_to_str(char* macstr, size_t macstr_max, const struct idip_hwaddr* hwaddr);
int idip_parse_mac_address(const char* macstr, struct idip_hwaddr* hwaddr);

struct idip_hwaddr;
struct idip_ipv4_addr;
struct idip_ipv6_addr;
struct idip_iface;

int idip_get_mac_address(const char* ifname, struct idip_hwaddr* hwaddr);

int idip_get_ifaddresses(const char* ifname,
                         struct idip_ipv4_addr* ipv4_addr,
                         struct idip_ipv6_addr* ipv6_addr);

#ifdef __cplusplus
}
#endif

#endif /* __NET_UTILS_H_INCLUDED__ */
