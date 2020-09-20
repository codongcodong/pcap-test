#pragma once

void printPktInfo(const u_char* packet, int pktLen);
void printEtherInfo(struct libnet_ethernet_hdr* ethernet_hdr);
void printIPv4Info(struct libnet_ipv4_hdr* ipv4_hdr);
void printTCPInfo(struct libnet_tcp_hdr* tcp_hdr);
void printPayloadData(const u_char* payload, int len);
