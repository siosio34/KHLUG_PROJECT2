#include "Configure.h"

class Windivert_Packet_System
{
private:
	HANDLE handle;
	float input_time; // UDP ��Ŷ�� ��Ƶ� �ð�
	vector<SENDSAVEPACKET> Saved_UDP_Packet; // ��Ŷ
	mutex mtx_lock; // mutex ����ȭ ��ü
public:
	void Windivert_Open_UDP();

	void ReceiveUdp_Packet(); // UDP �� �����ؼ�����
	void SendUdp_Packet(); // UDP �� �����ڽ��ϴ�.
	void _RunUDPDelay();

	void _RunPacketContentChange();
	void ReceiveTCP_Packet();



};