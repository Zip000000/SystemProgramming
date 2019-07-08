/*************************************************************************
	> File Name: 1.check.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月04日 星期二 19时53分48秒
 ************************************************************************/


#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
int socket_connect(int port, char *host) {
	int sockfd;
	struct sockaddr_in dest_addr;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket() error");
		return -1;
	}

	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port);
	dest_addr.sin_addr.s_addr = inet_addr(host);

    printf("ready to connect\n");
	if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
		perror("connect() error");
		return -1;
	}
	return sockfd;

}

int main() {
	int  socket_fd;
	struct passwd *pwd;
	pwd = getpwuid(getuid());
	char ip_addr[20] = "127.0.0.1";
	int port = 8888;
	char username[100];
    memset(username, 0, sizeof(username));
	strcpy(username, pwd->pw_name);
	socket_fd = socket_connect(port, ip_addr);
	if (socket_fd < 0)
	{
		exit(1);
	}
    //scanf("%s", username);
	if (send(socket_fd, username, strlen(username), 0) > 0) {
		printf("Check Success\n");
	} 
    sleep(1);
    char retstr[100];
    memset(retstr, 0, sizeof(retstr));
    if (recv(socket_fd, retstr, sizeof(retstr), 0) > 0) {
        printf("recv succcess   strlen = %ld\n", strlen(retstr));
    }
    printf("recv from server : %s\n", retstr);
	close(socket_fd);
	return 0;

}

