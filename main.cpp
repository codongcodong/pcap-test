#include <stdio.h>
#include <pcap.h>
#include <libnet.h>
#include "printInfo.h"

#define ETHER_HDR_LEN 14

void usage() {
    printf("syntax: pcap-test <interface>\n");
    printf("sample: pcap-test wlan0\n");
}

bool isTCPpkt(const u_char* packet){
    struct libnet_ipv4_hdr* ipv4_hdr = (libnet_ipv4_hdr*)(packet+ETHER_HDR_LEN);
    if(ipv4_hdr->ip_p==IPPROTO_TCP)
        return true;
    else
        return false;
}

int main(int argc, char* argv[]) {

    char* dev;
    char errbuf[PCAP_ERRBUF_SIZE];

    struct pcap_pkthdr* header;
    const u_char* packet;
    int res;

    if (argc != 2) {
        usage();
        return -1;
    }

    dev = argv[1];
    pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr) {
        fprintf(stderr, "pcap_open_live(%s) return nullptr - %s\n", dev, errbuf);
        return -1;
    }

    while (true) {
        res = pcap_next_ex(handle, &header, &packet);
        if (res == 0) continue;
        if (res == -1 || res == -2) {
            printf("pcap_next_ex return %d(%s)\n", res, pcap_geterr(handle));
            break;
        }

        if(isTCPpkt(packet)){
            printPktInfo(packet, header->caplen);
        }
    }

    pcap_close(handle);
}

