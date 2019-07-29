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
    pid_t pid = fork();
    int flag = 0;
    if (pid != 0) {
        int status;
        wait(&status);
    }
    if (pid != 0) {flag = 1; pid = fork();};
    if (pid != 0) {
        int status;
        wait(&status);
    }
    /*
    if (pid != 0) {flag = 2; pid = fork();};
    if (pid != 0) {
        int status;
        wait(&status);
    }
    */
    if (pid == 0 && flag == 0) {
        char *cmd[] = {"vim", filename, (char *)0};
        if (execvp("vim", cmd) < 0) perror("execvp1");
    }
    if (pid == 0 && flag == 1) {
        char *cmd_run[] = {"run", filename, (char *)0};
        if (execvp("run", cmd_run) < 0) perror("execvp2");
    }
    /*
    if (pid == 0 && flag == 2) {
        char *cmd_run[] = {"./", filename, (char *)0};
        if (execvp("./", cmd_run) < 0) perror("execvp2");
    }
    */
    return 0;
}
