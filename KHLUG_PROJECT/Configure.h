#pragma once

#define HAVE_REMOTE

#define ETHER_ADDR_LEN 6
#define ETHERTYPE_IP 0x0800
#define ETHERTYPE_ARP 0x0806
#define ETHERTYPE_REVARP 0X8035
#define ETHERTYPE_IPv6 0X86dd

#define ARP_IP_LEN 4
#define ARP_REQUEST 1
#define ARP_REPLY 2

#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20

#define TCP_PRO 6
#define UDP_PRO 17  

#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

#define GET_GATEMAC_MODE 0
#define GET_VICTIMEMAC_MODE 1

#define MAXBUF  0xFFFF

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "windivert.lib")

#include <stdio.h>
#include <stdlib.h>

#include <WinSock2.h>
#include <iphlpapi.h>
#include <Windows.h>
#include <conio.h>


#include <iostream>
#include <string>
#include <vector>

#include <thread> // c+ 11 �����带 ���� ���� �߰��� ���.
#include <mutex> // Mutex ��ü ����
#include <chrono> // c+ 11 ���� �������ִ� �ð� �ڵ�


// ���� �ҽ��� pcap ���̺귯���� windivert ���̺귯�� ��Ŭ��� 
// winpcap�� windivert�� ���̴� ���� ��Ŷ�̳� ���� ��Ŷ�� ���� ç�� �ִ��� �ƴ� ���⸸ �ϴ��� ����
// windiver �Լ��� ����ҷ��� ������ ���� �Ѿߵǰ� �̻��ϰ� ������� �õ��ϸ� ��Ŷ ��ü�� �ȿ��� ( �Ƹ� ����̹� ���� �����ε� )
#include <windivert.h>
#include <pcap.h>
using namespace std;

typedef struct etc_header {
	u_int8_t  ether_dhost[ETHER_ADDR_LEN];     //destination Mac
	u_int8_t  ether_shost[ETHER_ADDR_LEN];     //source  Mac
	u_int16_t ether_type;
}ETC_HEADER;

typedef struct arp_header
{
	u_short hard_type; //
	u_short Pro_type; // 
	u_char hard_length;
	u_char pro_length;
	u_short op_code;

	u_char source_Macaddr[ETHER_ADDR_LEN];  //sender hard address
	u_char source_ipaddr[ARP_IP_LEN]; // sender sourse ip

	u_char Des_Macaddr[ETHER_ADDR_LEN]; // target Hard address
	u_char Des_ipaddr[ARP_IP_LEN]; // target source IP
}ARP_HEADER;

typedef struct infection {
	ETC_HEADER etc;
	ARP_HEADER arp;
} infection;

/* IPv4 header */
typedef struct ip_header {
	u_char  ver_ihl;        // Version (4 bits) + Internet header length (4 bits)
	u_char  tos;            // Type of service
	u_short tlen;           // Total length
	u_short identification; // Identification
	u_short flags_fo;       // Flags (3 bits) + Fragment offset (13 bits)
	u_char  ttl;            // Time to live
	u_char  proto;          // Protocol
	u_short crc;            // Header checksum
	u_char  saddr[4];      // Source address
	u_char  daddr[4];      // Destination address
	u_int   op_pad;         // Option + Padding
}IPHEADER;


// TCP Header
typedef struct tcp_header {
	unsigned short sourceport;			// source port
	unsigned short destport;				// destination port
	unsigned long seqno;				// sequenz number
	unsigned long ackno;				// acknowledge number
	unsigned char th_x2 : 4;
	unsigned char hlen:4;					// Header length
	unsigned char flag;					// flags
	unsigned short window;				// window
	unsigned short chksum;				// checksum
	unsigned short urgptr;				// urgend pointer
	unsigned int op_pad;

}TCPHEADER, *PTCPHEADER;


/* UDP header*/
typedef struct udp_header {
	u_short sport;          // Source port
	u_short dport;          // Destination port
	u_short len;            // Datagram length
	u_short crc;            // Checksum

}UDPHEADER;

typedef struct Basic_ip_mac_addr
{
	vector<u_char> attacker_ip; // ����Ŀ ip
	vector<u_char> attacker_mac; // ����Ŀ��
	vector<u_char> gate_ip; // ����Ʈ���� ip
	vector<u_char> gate_mac; // ����Ʈ���� mac
	vector<u_char> victim_ip; // ����� ip
	vector<u_char> victim_mac; // victim mac

} Basic_ip_mac_addr;
//TCP PAYLOAD ���� ip header�� �ѱ��� - ip���ũ�� - tcp ���ũ��
//UDP DATAGRAM ���� ȣ��Ʈ ���� ������ �م� udp_header.len����  udp ���ũ�⸦ ���־���Ѵ�.


typedef struct
{
	WINDIVERT_ADDRESS send_addr; // ���� ���� �� �÷��� ����
	chrono::system_clock::time_point receive_packet_time; // �� ��Ŷ�� ������ �ð�
	int len;
	u_char buff[MAXBUF];

} SENDSAVEPACKET;

typedef struct
{
	WINDIVERT_ADDRESS send_addr; // ���� ���� �� �÷��� ����				
	u_char pay_load_data[65535];

} SENDTCPPACKET, *PSENDTCPPACKET;






