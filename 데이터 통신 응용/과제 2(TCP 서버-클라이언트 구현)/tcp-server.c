#include"echo-server.h"
#define WATING_QUEUE_SIZE 5

int main() {
	int sock, client_sock;
	struct sockaddr_in addr, client_addr;
	char buffer[BUFFER_SIZE];
	int addr_len, recv_len;
	
	//윈도우 환경변수 초기화
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//AF_INET : IPv4 주소 형식 사용
	//SOCK_STREAM : 양방향의 TCP/IP 기반 통신 사용
	//IPPROTO_TCP : TCP 사용
	if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		Error((char*)"Socket Init");

	//htonl : 네트워크 바이트 순서로 변경
	//INADDR_ANY : 아이피 자동 설정
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);
	
	if(bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		Error((char*)"Socket Binding");

	if(listen(sock, WATING_QUEUE_SIZE) < 0)
		Error((char*)"Listening");

	//client_addr로 받을 데이터의 사이즈로 추측
	//엉뚱한 값을 넣으면 프로그램이 계속 헛돈다.
	addr_len = sizeof(client_addr);

	printf("Waiting...\n");
	while(1) {
		client_sock = accept(sock, (struct sockaddr*)&client_addr, &addr_len);
		if(client_sock < 0) continue;

		if((recv_len = recv(client_sock, buffer, BUFFER_SIZE - 1, 0)) < 0)
			Error("Receive Data");
		buffer[recv_len] = '\0';

		printf("%s : %s\n", inet_ntoa(client_addr.sin_addr), buffer);
		send(client_sock, buffer, strlen(buffer), 0);

		closesocket(client_sock);
	}

	closesocket(sock);
	return 0;
}

