#include "Windivert_Packet_System.h"

void Windivert_Packet_System::Windivert_Open_UDP()
{
	INT16 priority = 0;

	//and udp.DstPort == 12345 for Debug Port

	string filter_name;
	cin >> filter_name;
	handle = WinDivertOpen(filter_name.c_str(), WINDIVERT_LAYER_NETWORK, priority, 0); // ��Ŷ ����. ��� ��Ŷ�� �ٹ޾ƿ;���.
	if (handle == INVALID_HANDLE_VALUE) // HANDLE ���� �����϶�
	{
		if (GetLastError() == ERROR_INVALID_PARAMETER)
		{
			fprintf(stderr, "error: filter syntax error\n");
			exit(EXIT_FAILURE);
		}
		fprintf(stderr, "error: failed to open the WinDivert device (%d)\n",
			GetLastError());
		exit(EXIT_FAILURE);
	}

	//if (!WinDivertSetParam(handle, WINDIVERT_PARAM_QUEUE_LEN, 8192))
	//{
	//	fprintf(stderr, "error: failed to set packet queue length (%d)\n",
	//		GetLastError());
	//	exit(EXIT_FAILURE);
	//}

}

void Windivert_Packet_System::ReceiveUdp_Packet()
{

	unsigned char packet[MAXBUF];
	WINDIVERT_ADDRESS recv_addr;
	UINT packet_len;

	while (TRUE)
	{
		mtx_lock.lock();
		// Read a matching packet.
		if (!WinDivertRecv(handle, packet, sizeof(packet), &recv_addr, // ��Ŷ���� �޾ƿ�, ���� �� �޾ƿ�
			&packet_len))
		{
			fprintf(stderr, "warning: failed to read packet\n");
			continue;
		}
		mtx_lock.unlock();
		// Print info about the matching packet.
		WinDivertHelperParsePacket(packet, packet_len, NULL, // �����ϱ� ���� ��Ŷ���� �Ľ��Ѵ�.
			NULL, NULL, NULL, NULL,
			NULL, NULL, NULL);

		//mtx_lock.lock();
		SENDSAVEPACKET UDP_temp; //

		memcpy(&(UDP_temp.buff), &packet, packet_len); // UDP ��Ŷ ����
		memcpy(&(UDP_temp.send_addr), &recv_addr, sizeof(WINDIVERT_ADDRESS)); //�ּ� ����
		UDP_temp.len = packet_len;
		UDP_temp.receive_packet_time = chrono::system_clock::now(); // �ð��ޱ�

		mtx_lock.lock();
		Saved_UDP_Packet.push_back(UDP_temp); // ��Ŷ ����
		mtx_lock.unlock();

	}

}

void Windivert_Packet_System::SendUdp_Packet()
{
	while (TRUE)
	{
		if (Saved_UDP_Packet.size() == 0)
			continue;

		else
		{
			//chrono::system_clock::time_point Endtime = chrono::system_clock::now();
			//time_t t = chrono::system_clock::to_time_t(Endtime);
			//time_t l = chrono::system_clock::to_time_t(Saved_UDP_Packet[0].receive_packet_time);

			std::chrono::duration<double> sec = std::chrono::system_clock::now() - Saved_UDP_Packet[0].receive_packet_time;

			if (sec.count() < input_time)
				continue;

			else
			{
				SENDSAVEPACKET now = Saved_UDP_Packet[0];
				//u_char *tt = (u_char*)now.buff;

				//for (int i = 0; i < now.len; i++)
				//	printf("%X ", tt[i]);

				mtx_lock.lock();
				if (!WinDivertSend(handle, (PVOID)&now.buff, (now.len), &(now.send_addr),
					NULL)) // send_addr �� ���⸸ �����ؼ� �������ָ� �ȴ�. ���� �ʿ��Ѱ� dnr �迭�� ���� �����ؾߵȴ�.
				{
					//fprintf(stderr, "warning: failed to send UDP message "
						//"(%d)\n", GetLastError());
				}
				mtx_lock.unlock();

				mtx_lock.lock();
				Saved_UDP_Packet.erase(Saved_UDP_Packet.begin());
				mtx_lock.unlock();

			}

		}
	}

}

void Windivert_Packet_System::_RunUDPDelay()
{
	// ���� ���� udp and outbound

	cout << " �ð� �Է� : ";
	cin >> input_time;

	Windivert_Open_UDP();

	std::thread* ReceiveThread;
	std::thread* SendThread;

	ReceiveThread = new std::thread(&Windivert_Packet_System::ReceiveUdp_Packet, this);
	SendThread = new std::thread(&Windivert_Packet_System::SendUdp_Packet, this);

	ReceiveThread->join();
	SendThread->join();
}


void Windivert_Packet_System::_RunPacketContentChange()
{
	// ���� ���� tcp and tcp.DstPort == 1239
	// UDP,HTTP ��Ŷ ���� Change ����
	// ���ڼ� �ٸ��� ��� �ؾߵ�
	// ���ú� ���� ���� ä������ ���߿� �Լ� ���� ���� ���� 
	// ���ڿ� �˻� �˰��� ���� �ʿ�

	//http://shaeod.tistory.com/220


	Windivert_Open_UDP();
	ReceiveTCP_Packet();


}

void Windivert_Packet_System::ReceiveTCP_Packet()
{
	//unsigned char pay_data[MAXBUF];
	u_int ip_len;
	u_int th_len;
	unsigned char *data_ptr;

	unsigned char packet[MAXBUF];

	UINT packet_len;
	UINT payload_len; // ���̷ε� ����  

	WINDIVERT_ADDRESS recv_addr;

	PWINDIVERT_IPHDR ip_header; // ip ���
	PWINDIVERT_TCPHDR tcp_header;

	while (TRUE)
	{
		// Read a matching packet.
		if (!WinDivertRecv(handle, packet, sizeof(packet), &recv_addr, // ��Ŷ���� �޾ƿ�, ���� �� �޾ƿ�
			&packet_len))
		{
			fprintf(stderr, "warning: failed to read packet\n");
			continue;
		}

		WinDivertHelperParsePacket(packet, packet_len, &ip_header, // �����ϱ� ���� ��Ŷ���� �Ľ��Ѵ�.
			NULL, NULL, NULL, &tcp_header,
			NULL, (PVOID*)NULL, &payload_len);

		

		if (ip_header == NULL) // ������ ��Ŷ�� ��� �ٽ� ���Ϲ�
		{
			continue;
		}

		if (tcp_header != NULL)
		{

			ip_len = (ip_header->HdrLength) * 4;
			th_len = tcp_header->HdrLength << 2;
			data_ptr = (unsigned char*)packet + ip_len + th_len;

			for (int i = 0; i < payload_len; i++)
			{
				//   printf("%X ", data_ptr[i]);
				data_ptr[i] = 'B';
			}

			WinDivertHelperCalcChecksums((PVOID)packet, packet_len, 0);

			SENDTCPPACKET Packet_temp;

			memcpy(&(Packet_temp.pay_load_data), &packet, packet_len); // UDP ��Ŷ ����
			memcpy(&(Packet_temp.send_addr), &recv_addr, sizeof(WINDIVERT_ADDRESS)); //�ּ� ����

			if (!WinDivertSend(handle, (PVOID)&(Packet_temp.pay_load_data), packet_len, &Packet_temp.send_addr,
				NULL)) // send_addr �� ���⸸ �����ؼ� �������ָ� �ȴ�. ���� �ʿ��Ѱ� dnr �迭�� ���� �����ؾߵȴ�.
			{
				fprintf(stderr, "warning: failed to send UDP message "
					"(%d)\n", GetLastError());
			}

		}
	}
}
