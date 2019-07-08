/*************************************************************************
	> File Name: 2.check_server.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月04日 星期二 19时59分00秒
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

#define MAX_CLIENT 40
#define PORT 8888

typedef struct server_data {
    int client_cnt;    //一共有多少用户
    int client_fds[MAX_CLIENT];   //保存每个客户的套接字
    fd_set all_fds;   //存入所有用户的套接字
    int maxfd;     //最大的文件描述符
} server_data;

server_data my_s_d;
server_data *s_d = &my_s_d;

int main() {
    int serv_sock;
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sock_addr.sin_port = htons(PORT);

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock;
    
    int client_fd;
    fd_set *rfds = &(s_d->all_fds);
    struct timeval tv;
    
    int cnt = 0;
    bind(serv_sock, (struct sockaddr* )&sock_addr, sizeof(sock_addr));
    listen(serv_sock, 100);

    while(1) {
        cnt++;
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        FD_ZERO(rfds);
        for(int i = 0; i < s_d->client_cnt; i++) {
            client_fd = s_d->client_fds[i];
            if(client_fd != -1) {
                FD_SET(client_fd, rfds);
            }
            s_d->maxfd = (client_fd > s_d->maxfd ? client_fd : s_d->maxfd);
        }
        
        int ret = select(s_d->maxfd+1, rfds, NULL, NULL, &tv);
        printf("select ret = %d\n", ret);
        if(ret == -1) {
            perror("select");
        } else if(ret == 0) {
            printf("Time out\n");
            continue;
        } else {
            if(FD_ISSET(serv_sock, rfds)) {
                printf("可读！\n");
            if( (clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size) ) < 0 ) {
                perror("accept");
            }
            s_d->client_fds[++s_d->client_cnt] = clnt_sock;

            }







        }
        

        unsigned long ul = 1;
        int ioctlcheck = ioctl(clnt_sock, FIONBIO, &ul);
        printf("accept over!\n");
        
            //if(FD_ISSET(clnt_sock, &rfds)) printf("读 ready\n");
            //if(FD_ISSET(clnt_sock, &wfds)) printf("写 ready\n");

        if(ret < 0) {
            perror("select error");
        } else if(ret == 0) {
            printf("time out!\n");
            continue;
        } else {
            printf("[No. %d] select success! 准备反弹！\n", cnt);
            char str[100];
            //char sendstr[100] = {0};
            int flag = 0;
            if(FD_ISSET(clnt_sock, &rfds)) {
                printf("clnt可读， 准备读取...\n");
                if( recv(clnt_sock, str, sizeof(str), 0) > 0 ) {
                    printf("读取成功 读取到的字符串是： %s , 长度： %ld\n", str, strlen(str));
                }
                int ret = select(clnt_sock + 1, &rfds, &wfds, NULL, &tv);
                if(FD_ISSET(clnt_sock, &wfds)) {
                    printf("clnt可写，准备发送...\n");
                    //sprintf(sendstr, "[%d] %s", cnt, str);
                    //sprintf(str, "[%d]", cnt);
                    if( send(clnt_sock, str, strlen(str), 0) > 0 ) printf("send success\n");
                    printf("反弹成功！\n");
                    flag = 1;
                }
            }
            if(flag == 0) printf("反弹失败!\n");
        }
        close(clnt_sock);

    }

    close(serv_sock);
	return 0;

}

