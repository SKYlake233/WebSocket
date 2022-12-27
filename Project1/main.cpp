#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<winsock.h>
#include<stdlib.h>
#include<time.h>
#include<string>

#pragma comment(lib,"WS2_32")
//TCP��������
void lsend(SOCKET sock, char* sd, int len) {
	int count = 0;
	char* lsd = sd;
	while (count < len) {
		count = send(sock, lsd, len, 0);
		lsd += count;
		len -= count;
	}
}

int main() {
	int a = 0;
	printf("������B�豸��id��\n");
	scanf("%d", &a);
	std::string a_id = std::to_string(a);
	const char* id = std::data(a_id);

	//ѡȡ�����ļ� ----> �������ֵ�̶�
	srand((unsigned)time(NULL));
	WSADATA ver;
	//����SOCKET�� ------> �׽��ֹ淶
	WSAStartup(MAKEWORD(1, 1), &ver);
	//���������׽���   IPV4   socket_stream   tcp
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock <= 0) printf("sock error!");
	//���÷��������׽���
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(SOCKADDR_IN));
	//��ʼ����ַ  127.0.0.1:8080
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//����������ַ   ��������������
	int con = connect(sock, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
	//���ݣ�
	char sd[1000];
	char dt[200];
	char rd[1000];
	//�豸id��
	while (true) {
		memset(dt, 0, 200);
		memset(sd, 0, 1000);
		memset(rd, 0, 1000);
		//�õ��¶�  [0,50]
		double temperature = (rand() % 500) / 10.0;
		//�õ�ʪ��ֵ [0,1]
		double humidity = (rand() * 1.0 / RAND_MAX);
		//�õ� pm25  [0,100]
		int pm25 = (rand() * 100 / RAND_MAX);
		//�õ�so2	[0,200]
		int so2 = (rand() * 20 / RAND_MAX);
		//�õ�     [0,100]
		int no2 = (rand() * 100 / RAND_MAX);
		//�õ�co    [0 , 2]
		double co = (rand() * 1.0 / RAND_MAX) * 2.0;
		/*
		POST /PathLocation/web/setTem.do HTTP/1.1
		Host:127.0.0.1:8080
		Content-Type: application/json
		Connection: keep-alive
		Content-Length: 55

		{"sid":1001,"temperature":17.5,"humidity":0.75,"illumination":123}
		*/
		//��ʼ��json����
		//sprintf(dt, "{\"collectDevice\":%s,\"temperature\":%.1f,\"humidity\":%.2f,\"pm25\":%d,\"so2\":%d,\"no2\":%d,\"co\":%.2f}", sid, temperature, humidity, pm25 , so2 , no2 , co);
		sprintf(dt, "{\"collectDevice\":%s,\"pm25\":%d,\"so2\":%d,\"no2\":%d,\"co\":%.2f}", id, pm25, so2, no2, co);
		int dtlen = strlen(dt);
		//��ʼ��header����
		strcpy(sd, "POST /data/upload HTTP/1.1\r\nHost:182.61.17.49:8080\r\nContent-Type: application/json\r\nConnection: keep-alive\r\nContent-Length: ");
		int hlen = strlen(sd);
		char* sdlen = sd + hlen;
		sprintf(sdlen, "%d\r\n\r\n", dtlen);
		hlen = strlen(sd);
		sdlen = sd + hlen;
		strcpy(sdlen, dt);
		printf(sd);
		printf("\r\n");
		lsend(sock, sd, strlen(sd));
		recv(sock, rd, 1000, 0);
		printf(rd);
		printf("\r\n");
		//����3����֮���ٷ���
		Sleep(180000);
	}
	return 0;
}