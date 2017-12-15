#ifndef NETSOCK_H
#define NETSOCK_H

#if defined (_WIN32)

#	pragma comment(lib, "ws2_32.lib")
#	include <WinSock2.h>
#	include <WS2tcpip.h>
#	define closesock closesocket

typedef SOCKET sock_t;
typedef SOCKADDR_IN sockaddr_in_t;

#elif defined (__unix) // _WIN32

#	include <sys/socket.h>
#	include <sys/types.h>
#	include <arpa/inet.h>
#	include <netinet/in.h>
#	include <unistd.h>
#	define closesock close

typedef int sock_t;
typedef struct sockaddr_in sockaddr_in_t;

#endif // __unix

void	end_socket(sock_t sock)
{
	closesock(sock);
#if defined(_WIN32)
	WSACleanup();
#endif // _WIN32
}

#endif // NETSOCK_H
