/*************************************************************************
	> File Name: 10.epoll_echo_server_my.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年07月09日 星期二 19时30分58秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <sys/types.h>

#define IPADDRESS   "127.0.0.1"
#define PORT        8888
#define MAX_BUF_SIZE     1024

#define MAX_EVENTS 1024

struct info{
    int fd;
    char buf[MAX_BUF_SIZE];
} info[MAX_EVENTS];


int get_listen_socket() {
    int listen_socket;
    if ((listen_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket() error");
        return -1;
    }
    struct sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = inet_addr(IPADDRESS);
    if (bind(listen_socket, (struct sockaddr *)&my_addr, sizeof(my_addr)) < 0) {
        perror("bind");
        return -1;
    }
    if (listen(listen_socket, 20) < 0) {
        perror("listen");
    }
    return listen_socket;
}

int  accept_clnt(int listen_socket) {
    struct sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    int clnt_socket = accept(listen_socket, (struct sockaddr *)&client_addr, &len);
    if(clnt_socket < 0) {
        perror("accept");
        return -1;
    }
    getpeername(clnt_socket, (struct sockaddr *)&client_addr, &len);
    printf("<%s> : Login\n",inet_ntoa(client_addr.sin_addr));
    return clnt_socket;
}

int do_read(int epfd, int fd, char *buf) {
    int nread;
    nread = read(fd, buf, MAX_BUF_SIZE);
    if(nread == -1) {
        perror("read");
        return -1;
    } else if(nread == 0) {
        printf("对方关闭链接\n");
        close(fd);
        return -1;
    } else {
        printf("read from peer : %s\n", buf);
        return 0;
    }
}
int do_write(int epfd, int fd, char *buf) {
    int nwrite;
    nwrite = write(fd, buf, strlen(buf));
    if(nwrite == -1) {
        perror("write");
        return -1;
    } else {
        //memset(buf, 0, MAX_BUF_SIZE);
        printf("反弹成功！\n");
        return 0;
    }
}

int main() {
    int listen_socket = get_listen_socket();
    if(listen_socket < 0) exit(1);
    int clnt_socket;

    struct epoll_event ev, events[MAX_EVENTS];

    int epfd = epoll_create(1);
    if (epfd == -1) {
        perror("epoll_create");
        exit(1);
    }
    int info_cnt = 0;
    info[info_cnt].fd = listen_socket;
    ev.events = EPOLLIN;
    //ev.data.fd = listen_socket;
    ev.data.ptr = &(info[info_cnt].fd);
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, listen_socket, &ev) == -1) {
        perror("epoll_ctl");
        exit(1);
    }
    char buf[MAX_BUF_SIZE] = {0};
    int w_cnt = 0;
    while(1) {
        w_cnt++;
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        if(nfds == -1) {
            perror("epoll_wait");
            exit(1);
        }
        int f_cnt = 0;
        for(int i = 0; i < nfds; i++) {
            int now_fd = *((int *)events[i].data.ptr);
            printf("w_cnt = %d  f_cnt = %d\n", w_cnt, f_cnt);
            if(now_fd == listen_socket) {
                clnt_socket = accept_clnt(listen_socket);
                info_cnt++;
                info[info_cnt].fd = clnt_socket;
                ev.events = EPOLLIN;
                ev.data.fd = clnt_socket;
                if(epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_socket, &ev) == -1 ) {
                    perror("epoll_ctl");
                    exit(1);
                }
            } else if(events[i].events & EPOLLIN) {
                char *now_buf = ((struct info *)(events[i].data.ptr))->buf;
                if(do_read(epfd, now_fd, now_buf) < 0) continue;
                ev.events = EPOLLOUT;
                ev.data.ptr = &now_fd;
                epoll_ctl(epfd, EPOLL_CTL_MOD, now_fd, &ev);
            } else if(events[i].events & EPOLLOUT) {
                char *now_buf = ((struct info *)(events[i].data.ptr))->buf;
                if(do_write(epfd, now_fd, now_buf) < 0) continue;
                ev.events = EPOLLIN;
                ev.data.ptr = &now_fd;
                epoll_ctl(epfd, EPOLL_CTL_MOD, now_fd, &ev);
            }
        }
    }
    close(epfd);
    
    return 0;
}
