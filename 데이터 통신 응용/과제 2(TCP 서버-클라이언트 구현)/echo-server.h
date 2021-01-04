#include<stdio.h>
#include<winsock2.h>

#define PORT 8081
#define BUFFER_SIZE 1024

void Error(char* info) {
	printf("Error : %s\n", info);
	exit(1);
}
