#include "netsock.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <ft/libft.h>

void	exit_error(const char *err);
int		read_file(const char *file_name, char *buffer, size_t read_size);

int		exec_command(const char *cmd);

#define FILE_BYTE_MAX 65536

char	*g_home_file = "home.html";
char	*g_nfnd_file = "nfound.html";

sock_t	serv_sock, client_sock;

int main(int argc, char **argv)
{
#if defined (_WIN32)
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 0), &wsa) != 0)
		exit_error("Failed to startup WSA");
#endif

	sockaddr_in_t	addr_info;
	int				sizeof_addr = sizeof(sockaddr_in_t);
	char			http_request[1024];
	char			file_content[FILE_BYTE_MAX];
	char			http_response[FILE_BYTE_MAX] = "HTTP/1.1 200 OK\r\n\n";
	char			nfound_buff[FILE_BYTE_MAX];
	char			home_buff[2048];
	int				fd_request;

	if (read_file(g_nfnd_file, nfound_buff, FILE_BYTE_MAX) == -1 || read_file(g_home_file, home_buff, FILE_BYTE_MAX) == -1)
		exit_error("Could not open 'nfound' or 'home' html file");

	serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
		exit_error(strerror(errno));
	addr_info.sin_family = AF_INET;
	addr_info.sin_port = htons(ft_atoi(argv[1]));
	addr_info.sin_addr.s_addr = INADDR_ANY;
	if (bind(serv_sock, (struct sockaddr *)&addr_info, sizeof_addr) == -1)
		exit_error(strerror(errno));
	
	listen(serv_sock, 10);
	printf("Listening to port %d\n\n", ft_atoi(argv[1]));
	while ((client_sock = accept(serv_sock, (struct sockaddr *)&addr_info, &sizeof_addr)) != -1)
	{
		char	*ip_addr = inet_ntoa(addr_info.sin_addr);
		int		recv_len = recv(client_sock, http_request, 1024, 0);
		if (recv_len == -1)
		{
			printf("%s\n", strerror(errno));
			closesock(client_sock);
			continue;
		}

		int		send_len;
		char	*file_rqst_name = ft_strbetweenstr(ft_strrepstr(http_request, "S/", "../"), " /", " ");
		if (*file_rqst_name == '\0')
			file_rqst_name = g_home_file;
		printf("/--------------------------------------------------------------\\\n");
		printf("CONNECTION : %s | REQUEST : %s\n", ip_addr, file_rqst_name);
		printf("<---------->\n%s\n", http_request);
		if (*file_rqst_name == '?')
			exec_command(file_rqst_name + 1);
		else if (read_file(file_rqst_name, file_content, FILE_BYTE_MAX) == -1)
			ft_strcat(http_response, nfound_buff);
		else
			ft_strcat(http_response, file_content);

		send_len = send(client_sock, http_response, ft_strlen(http_response), 0);
		if (send_len == -1)
			printf("%s\n", strerror(errno));
		
		closesock(client_sock);
		ft_strncpy(http_response, "HTTP/1.1 200 OK\r\n\n", FILE_BYTE_MAX);
	}

	end_socket(serv_sock);
	return (0);
}

void	exit_error(const char *err)
{
	printf("ERROR : %s\n", err);
	end_socket(serv_sock);
	exit(1);
}

int		read_file(const char *file_name, char *buffer, size_t read_size)
{
	int		fd_file = open(file_name, O_RDONLY);
	return (read(fd_file, buffer, read_size));
}

int		exec_command(const char *cmd)
{
	if (ft_strequ(cmd, "quit_server"))
	{
		printf("Server shutdown\n");
		end_socket(serv_sock);
		exit(0);
	}
	return (0);
}
