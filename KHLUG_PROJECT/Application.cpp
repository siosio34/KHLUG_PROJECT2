
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
	printf("5. Packet content Change \n\n");

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

	case 4:
		RunUDPDelay();
		break;
	case 5:
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

void Application::RunUDPDelay()
{
	divert_ob.Run_UDP_Delay();
}
