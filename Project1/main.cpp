#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<winsock.h>
#include<stdlib.h>
#include<time.h>
#include<string>

#pragma comment(lib,"WS2_32")
//TCP发送数据
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
	printf("请输入B设备的id：\n");
	scanf("%d", &a);
	std::string a_id = std::to_string(a);
	const char* id = std::data(a_id);

	//选取种子文件 ----> 随机的数值固定
	srand((unsigned)time(NULL));
	WSADATA ver;
	//加载SOCKET库 ------> 套接字规范
	WSAStartup(MAKEWORD(1, 1), &ver);
	//建立流试套接字   IPV4   socket_stream   tcp
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock <= 0) printf("sock error!");
	//设置服务器的套接字
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(SOCKADDR_IN));
	//初始化地址  127.0.0.1:8080
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//连接主机地址   返回连接描述符
	int con = connect(sock, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
	//数据？
	char sd[1000];
	char dt[200];
	char rd[1000];
	//设备id？
	while (true) {
		memset(dt, 0, 200);
		memset(sd, 0, 1000);
		memset(rd, 0, 1000);
		//得到温度  [0,50]
		double temperature = (rand() % 500) / 10.0;
		//得到湿度值 [0,1]
		double humidity = (rand() * 1.0 / RAND_MAX);
		//得到 pm25  [0,100]
		int pm25 = (rand() * 100 / RAND_MAX);
		//得到so2	[0,200]
		int so2 = (rand() * 20 / RAND_MAX);
		//得到     [0,100]
		int no2 = (rand() * 100 / RAND_MAX);
		//得到co    [0 , 2]
		double co = (rand() * 1.0 / RAND_MAX) * 2.0;
		/*
		POST /PathLocation/web/setTem.do HTTP/1.1
		Host:127.0.0.1:8080
		Content-Type: application/json
		Connection: keep-alive
		Content-Length: 55

		{"sid":1001,"temperature":17.5,"humidity":0.75,"illumination":123}
		*/
		//初始化json数据
		//sprintf(dt, "{\"collectDevice\":%s,\"temperature\":%.1f,\"humidity\":%.2f,\"pm25\":%d,\"so2\":%d,\"no2\":%d,\"co\":%.2f}", sid, temperature, humidity, pm25 , so2 , no2 , co);
		sprintf(dt, "{\"collectDevice\":%s,\"pm25\":%d,\"so2\":%d,\"no2\":%d,\"co\":%.2f}", id, pm25, so2, no2, co);
		int dtlen = strlen(dt);
		//初始化header数据
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
		//休眠3分钟之后再发送
		Sleep(180000);
	}
	return 0;
}