/*************************************************************************
	> File Name: 12.itimer.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年07月11日 星期四 19时23分35秒
 ************************************************************************/

#include<stdio.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
//#include <bits/stdc++.h>

struct itimerval new_v;
void handle(int a) {
    printf("Hello Zip!\n");
    new_v.it_value.tv_sec += 1;
    new_v.it_value.tv_usec = 0;
    printf("sec = %ld now usec = %ld\n",new_v.it_value.tv_sec, new_v.it_value.tv_usec);
    setitimer(ITIMER_REAL, &new_v, NULL);
}

int main() {
    signal(SIGALRM, handle);

    new_v.it_value.tv_sec = 1;
    new_v.it_value.tv_usec = 0;
    new_v.it_interval.tv_sec = 1;
    new_v.it_interval.tv_usec = 0;

    setitimer(ITIMER_REAL, &new_v, NULL);
    while(1) {
    }
}
