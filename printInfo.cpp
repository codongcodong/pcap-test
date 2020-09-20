#include <stdio.h>
#include <stdint.h>
#include <libnet.h>
#include "printInfo.h"

#define ETHER_HDR_LEN 14

void printMacAddr(u_int8_t* addr);
void printIPAddr(struct in_addr addr);

void printPktInfo(const u_char* packet, int pktlen){
    struct libnet_ethernet_hdr* ethernet_hdr = (libnet_ethernet_hdr*)packet;
    struct libnet_ipv4_hdr* ipv4_hdr = (libnet_ipv4_hdr*)((char*)(ethernet_hdr)+ETHER_HDR_LEN);

    int ipLen = (*((char*)ipv4_hdr)&0x0F)<<2;		//caculate the length of ip header
    struct libnet_tcp_hdr* tcp_hdr = (libnet_tcp_hdr*)((char*)ipv4_hdr+ipLen);

    int tcpLen = ((*((char*)tcp_hdr+12))&0xF0)>>2;  //caculate the length of tcp header
    const u_char* payload = (u_char*)tcp_hdr + tcpLen;

    int payloadLen = pktlen - ETHER_HDR_LEN - ipLen - tcpLen;

    printf("Packet Length: %d\n",pktlen);
    printEtherInfo(ethernet_hdr);
	puts("");

    printf("IP Header Length: %d\n",ipLen);
    printIPv4Info(ipv4_hdr);
	puts("");

    printf("TCP Header Length: %d\n",tcpLen);
    printTCPInfo(tcp_hdr);
	puts("");

    printf("Payload Length: %d\n",payloadLen);
    printPayloadData(payload, payloadLen);
	puts("\n");
}

void printEtherInfo(struct libnet_ethernet_hdr* ethernet_hdr){
	printf("Src MAC addr: ");
	printMacAddr(ethernet_hdr->ether_shost);
	printf("Dst MAC addr: ");
	printMacAddr(ethernet_hdr->ether_dhost);
}

void printIPv4Info(struct libnet_ipv4_hdr* ipv4_hdr){
	printf("Src IP addr: ");
	printIPAddr(ipv4_hdr->ip_src);
	printf("Dst IP addr: ");
	printIPAddr(ipv4_hdr->ip_dst);
}

void printTCPInfo(struct libnet_tcp_hdr* tcp_hdr){
	printf("Src TCP Port: %hu\n", ntohs(tcp_hdr->th_sport));
	printf("Dst TCP Port: %hu\n", ntohs(tcp_hdr->th_dport));
}

void printPayloadData(const u_char* payload, int len){
	int limit = (16<len)?16:len;

	puts("Payload Data");
	for(int i=0;i<limit;i++)			//print hexadecimal values (at most 16bytes)
		printf("%#X ",payload[i]);
	puts("");
}

void printMacAddr(u_int8_t* addr){
	printf("%.2X",addr[0]);
	for(int i=1;i<6;i++)
		printf(":%.2X",addr[i]);
	puts("");
}

void printIPAddr(struct in_addr addr){
	puts(inet_ntoa(addr));
}

