#pragma once
#include "Configure.h"



class Winpcap_Packet_System
{
private:
	 pcap_t *adhandle; // ���� ����� �ڵ�
	 Basic_ip_mac_addr Basic_addr;
	
	
public:
	
	void Print_Hex(void *Data, u_int len);
	static void pcap_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);
	
	int open_device(pcap_t *_adhandle, int _flag);
	int Input_Victim_ip(); // ����� IP �� �˰� �־�� �Ѵ�.
	PIP_ADAPTER_ADDRESSES Find_Addapter(string _device_name); // ����� �̸� ã��
	vector<u_char> Send_ARP_For_Macaddr(pcap_t* handle, int flag); // Arp ��Ŷ�� ������ ���ּҸ� ����.

	void Send_Arp_Infection_Packet();
	void Send_Arp_Relay_Packet();

	void _RunPacketCapture();
	void _RunArpSpoofing();
	void _RunSendRst();
};

