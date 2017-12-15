#include "netsock.h"
#include <stdio.h>
#include <stdlib.h>
#include <ft/libft.h>

void exit_error(const char *msg);

int main(int argc, char **argv)
{
#if defined (_WIN32)
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 0), &wsa) != 0)
		exit_error("Failed to startup WSA");
#endif

	sock_t net_socket;
	sockaddr_in_t serv_addr;
	char serv_msg[2048];

	net_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (net_socket < 0)
		exit_error("Failed to create the socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(ft_atoi(argv[2]));
	if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr.s_addr) != 1)
		exit_error("Not a valid address");

	if (connect(net_socket, (struct sockaddr *)(&serv_addr), sizeof(serv_addr)) == -1)
		exit_error("Failed to connect to server");

	int byte_sent = send(net_socket, argv[3], ft_strlen(argv[3]), 0);
	printf("%d bytes of %s has been sent\n", byte_sent, argv[3]);

	closesock(net_socket);
#if defined (_WIN32)
	WSACleanup();
#endif

	return (0);
}

void exit_error(const char *msg)
{
	printf("ERROR : %s\n", msg);
#if defined (_WIN32)
	WSACleanup();
#endif
	exit(1);
}
