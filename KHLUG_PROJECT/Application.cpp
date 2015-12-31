
#include "Application.h"

void Application::PrintRun()
{
	int Selecte_Num;
	printf(" Network Packet Control Project \n\n");
	printf(" ------- WinPcap Mode ----------\n");
	printf(" 1. Packet Capture \n"); // ��Ŷ ĸ�� �⺻���
	printf(" 2. Arp Spoofing \n"); // Arp Spoofing 
	printf(" 3. Send TCP RST \n\n"); 

	printf(" ------- WinDivert Mode ----------\n");
	printf(" 4. UDP Delay Mode \n ");
	printf(" 5. Packet content Change \n\n"); // ���ڼ� �����Ÿ� �ϴ� ��

	printf(" ------- Use WinAPI Mode ----------\n");
	printf(" 6. TCP Client \n");


	printf(" ------- ���� ���� ----------\n");
	printf(" 6. ��ȭ�� ��� �� ������Ʈ ��� (���� ����) \n");
	printf(" 7. Cookie Sniff (���� ����)\n\n");

	

	printf(" Num : ");
	scanf_s("%d", &Selecte_Num);
	
	ProcessInput(Selecte_Num);
}

void Application::ProcessInput(int num)
{
	switch (num)
	{
	case 1:
		RunPacketCapture();
		break;
	case 2:
		RunArpSpoofing();
		break;
	case 3:
		RunSendRst();
	case 4:
		RunUDPDelay();
		break;
	case 5:
		RunPacketContentChange();
		break;
	case 6:

		break;
	default:
		break;
	}
}

void Application::RunPacketCapture()
{
	pcap_obj._RunPacketCapture();
}

void Application::RunArpSpoofing()
{
	pcap_obj._RunArpSpoofing();
}

void Application::RunSendRst()
{
	pcap_obj._RunSendRst();
}

void Application::RunUDPDelay()
{
	divert_ob._RunUDPDelay();
}

void Application::RunPacketContentChange()
{
	divert_ob._RunPacketContentChange();
}

