/*************************************************************************
	> File Name: 1.exec.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年07月28日 星期日 14时34分25秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
int main(int argc, char *argv[]) {
    if (argc != 2) {printf("请输入一个参数\n"); exit(1);}
    char filename[100];;
    strcpy(filename, argv[1]);
    int flag = 0;
    pid_t pid = fork();
    if (pid != 0) {
        int status;
        wait(&status);
        printf("wait1 over\n");
        flag = 1; 
        pid = 0;
        pid = fork();
        wait(&status);
        printf("wait2 over\n");
        flag = 2; 
        pid = 0;
        pid = fork();
        wait(&status);
        printf("wait3 over\n");
    }

    if (pid == 0 && flag == 0) {
        printf("in vim\n");
        if (execl("vim", "vim", filename, (char *)0) < 0) perror("execvp1");
    }
    if (pid == 0 && flag == 1) {
        printf("in gcc\n");
        if (execl("gcc", "gcc", filename, "-o", "hello", (char *)0) < 0) {
            perror("execvp2");
        } 
    }
    if (pid == 0 && flag == 2) {
        printf("in do\n");
        if (execl("./hello", "./hello", (char *)0) < 0) perror("execvp3");
    }
    return 0;
}
