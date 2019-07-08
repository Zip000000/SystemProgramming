/*************************************************************************
	> File Name: 1.select_connect.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年07月07日 星期日 09时37分23秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/time.h>
#include <errno.h>



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
    
    unsigned long ul = 1;

    //fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFL, 0) | O_NONBLOCK);
    int ioctlcheck = ioctl(sockfd, FIONBIO, &ul);


    fd_set wfds;
    struct timeval tv;
    int retval;
    
    FD_ZERO(&wfds);
    FD_SET(sockfd, &wfds);

    tv.tv_sec = 2;
    tv.tv_usec = 0;

    int error;
    int len = sizeof(error);

    printf("ready to connect!\n");
	if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
        printf("errno = %d\n", errno);
		perror("connect() error");
    } else {
        printf("connect return 0\n");
        close(sockfd);
        exit(0);
    }
    printf("connect once\n");

    retval = select(sockfd + 1, NULL, &wfds, NULL, &tv);
    printf("retval = %d  select once\n", retval);

    if(retval == -1) perror("select");
    if(retval > 0) printf("[SELECT] WRITING is available!!!!!!\n");
    if(retval == 0) printf("select over time! nobody is ok!\n");

    if(retval > 0) {
        if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len) < 0) {
            perror("getsockopt");
            close(sockfd);
            return -1;
        }
        if(error == 0) {
            return sockfd;
        } else {
            close(sockfd);
            return -1;
        }
    }

	return -1;
}

int main() {
	int  socket_fd;
	//struct passwd *pwd;
	//pwd = getpwuid(getuid());
    char ip_addr[20] = "127.0.0.1";
	int port = 8888;
    //printf("username = %s\n", username);

	char username[50];
	strcpy(username, "Hellp i am zip");

    for(int i = 0; i <= 255; i++) {
        //sprintf(ip_addr, "192.168.2.%d", i);
	    socket_fd = socket_connect(port, ip_addr);
        printf("---------------------socketfd = %d\n", socket_fd);
        if(socket_fd < 0) continue;
        printf("*******************************%s\n", ip_addr);
        close(socket_fd);
    }

	if (send(socket_fd, username, strlen(username), 0) > 0) {
		printf("Check Success\n");
	} 
	close(socket_fd);
	return 0;

}

