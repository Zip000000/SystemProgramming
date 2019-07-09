/*************************************************************************
	> File Name: 6.poll_test.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年07月08日 星期一 21时35分31秒
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
#include <poll.h>

#define MAX_CLIENT 1000
#define PORT 8888

typedef struct client_data {
    int listen_socket;
    int clientnum;    //当前一共有多少用户
    int client_socket[MAX_CLIENT];   //保存每个客户的套接字
    fd_set all_fds;   //存入所有用户的client套接字, 以及listen套接字，select来查找。
    int maxfdnum;     //目前最大的文件描述符
} client_data;

client_data my_c_d;
client_data *c_data = &my_c_d;


int acceptNaddinfo() {
    
    //unsigned long ul = 1;
    //int ioctlcheck = ioctl(c_data->listen_socket, FIONBIO, &ul);
    
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock;
    clnt_sock = accept(c_data->listen_socket, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if(clnt_sock < 0) {
        perror("accept");
        return -1;
    }
    c_data->client_socket[(c_data->clientnum)++] = clnt_sock;
    return 0;
}

void rebound() {
    fd_set *rfds = &(c_data->all_fds);
    int recvnum = 0;
    int flag = 0;
    char str[100];
    for(int i = 0; i < c_data->clientnum; i++) {
        int clnt_sock = c_data->client_socket[i];
        if(clnt_sock == -1) continue;
        if(FD_ISSET(clnt_sock, rfds)) {
            printf("clnt可读， 准备读取...\n");
            recvnum = recv(clnt_sock, str, sizeof(str), 0);
            if( recvnum > 0 ) {
                printf("读取成功 读取到的字符串是： %s , 长度： %ld\n", str, strlen(str));
            }
            if(recvnum == 0) {
                printf("有用户登出,暂不做处理\n");
                close(clnt_sock);
                continue;
            }
            printf("准备发送...\n");
            if( send(clnt_sock, str, strlen(str), 0) > 0 ){
                printf("send success\n");
                printf("反弹成功！\n");
                flag = 1;
            } 
        }
    }
    if(flag == 0) printf("反弹失败!\n");


}


int main() {
    int serv_sock;
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sock_addr.sin_port = htons(PORT);

    int client_fd;
    struct timeval tv;

    bind(serv_sock, (struct sockaddr* )&sock_addr, sizeof(sock_addr));
    listen(serv_sock, 100);

    fd_set *rfds = &(c_data->all_fds);
    c_data->listen_socket = serv_sock;
    c_data->clientnum = 0;
    c_data->maxfdnum = serv_sock;
    for(int i = 0; i < MAX_CLIENT; i++) {
        c_data->client_socket[i] = -1;
    }

    while(1) {
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        FD_ZERO(rfds);
        FD_SET(serv_sock, rfds);
        for(int i = 0; i < c_data->clientnum; i++) {
            int tmp_socket = c_data->client_socket[i];

            if(tmp_socket != -1) {
                FD_SET(tmp_socket, rfds);
            }
            int tmp_max = c_data->maxfdnum;
            c_data->maxfdnum = (tmp_socket > tmp_max ? tmp_socket : tmp_max);
        }

        int ret = select(c_data->maxfdnum + 1, rfds, NULL, NULL, &tv);
        //先select 再accept 这样就不用将socket设置成非阻塞模式了？
        printf("select ret = %d\n", ret);
        if(ret == -1) {
            perror("select");
        } else if(ret == 0) {
            printf("Time out\n");
            continue;
        } else {
            if(FD_ISSET(serv_sock, rfds)) {
                printf("serv_sock可读！ 也就是有新客户端connect了！\n");
                if(acceptNaddinfo() < 0) {printf("accept failed\n");continue;}
                printf("accept and addinfo success\n");
            } else {
                rebound();
                printf("rebound successfull\n");
            }

        }
    }
        

    close(serv_sock);
	return 0;

}

