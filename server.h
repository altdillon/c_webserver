#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include "severconfig.h"
#include "endpoint.h"
#include "datalogging.h"

// structs from hacking-network.h ...
/* Structure for Ethernet headers */
#define ETHER_ADDR_LEN 6
#define ETHER_HDR_LEN 14

struct ether_hdr {
   unsigned char ether_dest_addr[ETHER_ADDR_LEN]; // Destination MAC address
   unsigned char ether_src_addr[ETHER_ADDR_LEN];  // Source MAC address
   unsigned short ether_type; // Type of Ethernet packet
};

/* Structure for Internet Protocol (IP) headers */
struct ip_hdr {
   unsigned char ip_version_and_header_length; // version and header length combined
   unsigned char ip_tos;          // type of service
   unsigned short ip_len;         // total length
   unsigned short ip_id;          // identification number
   unsigned short ip_frag_offset; // fragment offset and flags
   unsigned char ip_ttl;          // time to live
   unsigned char ip_type;         // protocol type
   unsigned short ip_checksum;    // checksum
   unsigned int ip_src_addr;      // source IP address
   unsigned int ip_dest_addr;     // destination IP address
};

/* Structure for Transmission Control Protocol (TCP) headers */
struct tcp_hdr {
   unsigned short tcp_src_port;   // source TCP port
   unsigned short tcp_dest_port;  // destination TCP port
   unsigned int tcp_seq;          // TCP sequence number
   unsigned int tcp_ack;          // TCP acknowledgement number
   unsigned char reserved:4;      // 4-bits from the 6-bits of reserved space
   unsigned char tcp_offset:4;    // TCP data offset for little endian host
   unsigned char tcp_flags;       // TCP flags (and 2-bits from reserved space)
#define TCP_FIN   0x01
#define TCP_SYN   0x02
#define TCP_RST   0x04
#define TCP_PUSH  0x08
#define TCP_ACK   0x10
#define TCP_URG   0x20
   unsigned short tcp_window;     // TCP window size
   unsigned short tcp_checksum;   // TCP checksum
   unsigned short tcp_urgent;     // TCP urgent pointer
};

struct server_app {
  int serverFD; //   FD of the server
};


// function headers from hacking network 
int send_string(int sockfd, unsigned char *buffer);
int recv_line(int sockfd, unsigned char *dest_buffer);

// functions for running the server
void runServerLoop();             // just run the server loop
void handle_sigint(int sig);      // ^C that bitch
void stopServer();
void handle_connection(int sockfd, struct sockaddr_in *client_addr_ptr);
int cutStr(char *str);            // remove spaces from text

// global config stuff
bool running;
volatile sig_atomic_t serverRunning; // flag to tell the server if it's running or not

// small functions for sending approate HTTP responce
void send404(int sockfd); // 404 page not found
void send200(int sockfd); // 200 page found
// right now not implmented, but might be added in the future
void send408(int sockfd); // 408 client timeout, the client waited to long after connecting to send an actual reqest
void send500(int sockfd); // 500 server internal error

#endif
