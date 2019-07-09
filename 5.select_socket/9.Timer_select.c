/*************************************************************************
	> File Name: 9.Timer_select.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年07月09日 星期二 18时07分58秒
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

#define DEFALT_TIME 1000000

int main(int argc, char *argv[]) {
    int sleeptime = DEFALT_TIME;
    struct timeval tv;
    if(argc > 1) {
        sleeptime = atoi(argv[1]);
    } 
    tv.tv_sec = 0;
    tv.tv_usec = sleeptime;
    int ret = select(0, NULL, NULL, NULL, &tv);
    if(ret == 0) {
        printf("sleep %d um ! Time out !\n", sleeptime);
    }
	return 0;
}

