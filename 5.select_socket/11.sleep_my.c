/*************************************************************************
	> File Name: 11.sleep_my.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年07月10日 星期三 10时24分21秒
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

void handler(int n) {
    printf("ALARM signal received\n");
}
void my_sleep2(int n) {
    alarm(n);
    signal(SIGALRM, handler);
    pause();
}
int main() {
    printf("[my_sleep2] 3 s : sleeping......\n");
    my_sleep2(3);
    printf("[2]wake up!\n");
    return 0;
}


























/*
void my_sleep1(long long n) {
    struct timeval start;
    struct timeval now;
    gettimeofday(&start, NULL);
    //printf("sec : %ld   usec : %ld \n", start.tv_sec, start.tv_usec);
    long long dis = 0;
    while (1) {
        gettimeofday(&now, NULL);
        dis = (now.tv_sec * 1000000 + now.tv_usec  )  - (start.tv_sec * 1000000 + start.tv_usec);
        if (dis >= n) break;
    }
    return ;
}
*/
