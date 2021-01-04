#include"echo-server.h"

boolean ConnectServer(int sock, struct sockaddr *addr);

int main() {
	int sock;
	struct sockaddr_in addr;
	char buffer[BUFFER_SIZE];
	int recv_len;

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	if((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		Error((char*)"Socket Init");

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);

	do {
		char ip[20];
		printf("Please enter server ip address : ");
		scanf("%s", ip);
		addr.sin_addr.s_addr = inet_addr(ip);
	} while(!ConnectServer(sock, (struct sockaddr*)&addr));

	printf("Please text to send : ");
	fflush(stdin); //남아 있는 입력 버퍼 지우기
	scanf("%[^\n]", buffer);

	
	if(send(sock, buffer, strlen(buffer), 0) < 0)
		Error("Sending");
	
	if((recv_len = recv(sock, buffer, BUFFER_SIZE - 1, 0)) < 0)
		Error("Receive Data");

	buffer[recv_len] = '\0';

	printf("received : %s\n", buffer);

	return 0;
}

boolean ConnectServer(int sock, struct sockaddr *addr) {
	int a;
	if((a = connect(sock, addr, sizeof(*addr))) < 0) {
		printf("Please Recheck Ip address\n");
		return FALSE;
	}
	else return TRUE;
}
