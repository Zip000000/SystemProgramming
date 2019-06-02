/*************************************************************************
	> File Name: pid.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年05月26日 星期日 17时00分04秒
 ************************************************************************/

#include<stdio.h>

    #include <sys/wait.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <stdio.h>


int main(int argc, char* argv[]) {
    pid_t cpid;
    int status;
    int my_id = 1;
    for(int i = 0; i < 9; i++) {
        if (cpid != 0) {
            cpid = fork();
            my_id++;
        }
    }

    printf("%d", my_id);

    sleep(10000);

}
