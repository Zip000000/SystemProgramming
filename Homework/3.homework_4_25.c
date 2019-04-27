/*************************************************************************
	> File Name: homework_4_25.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年04月26日 星期五 09时56分10秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <signal.h>
#define MAX_SIZE 200
#define MIN_SIZE 50
	
struct passwd* passwd;

void show() {
    char wd[MAX_SIZE];
    memset(wd, 0, strlen(wd));
    getcwd(wd, sizeof(wd));       //得到当前工作路径
    passwd = getpwuid(getuid());   //！！得到passwd结构体
    char hostname[32];
    gethostname(hostname, sizeof(hostname));   //得到主机名 <unistd.h>
    char nwd[MAX_SIZE];  //存放家目录经过～处理的路径
    memset(nwd, 0, strlen(nwd));

    if((strncmp(wd, passwd->pw_dir, strlen(passwd->pw_dir)) == 0)) {  //家目录～处理
        sprintf(nwd, "~%s", wd+strlen(passwd->pw_dir));
    } else {
        strncpy(nwd, wd, strlen(wd));
    }
    printf("\033[;32m%s\033[0m", passwd->pw_name);
    printf("\033[;31m@\033[0m");
    printf("\033[;33m%s\033[0m", hostname);
    printf("\033[;31m:\033[0m");
    printf("\033[;34m%s\033[0m", nwd);

    if(strcmp(passwd->pw_name, "root") == 0) {
        printf("# ");
    } else {
        printf("$ ");
    }
}

int main() {
    char name[MIN_SIZE];
    char addr[MAX_SIZE];    //存放键入的 不完整路径
    char history[MAX_SIZE];     //存放上次的记录
    char history_new[MAX_SIZE];  //存放本次的记录
    char wd[MAX_SIZE];        //存放当前工作路径
    memset(wd, 0, strlen(wd));
    getcwd(wd,sizeof(wd));
    strncpy(history, wd, strlen(wd));  // history初始化
    int count = 0;   //第一次为0   其余情况都为1
    char all_addr[MAX_SIZE];   //存放完整的路径

    signal(SIGINT,SIG_IGN);   //忽略ctrl c
    
    while(1) {
        memset(wd, 0, strlen(wd));
        getcwd(wd, sizeof(wd));
        show();
        scanf("%s", name);

        if(strcmp(name, "cd") == 0) {
            if(getchar() == 10) {   //只输入了cd  返回家目录
                memset(addr, 0, strlen(addr));
                addr[0] = '~';
            } else {
                scanf("%s", addr);
            }
            
            memset(all_addr, 0, strlen(all_addr));  //重构
            if(addr[0] == '/') {                  //键入的是绝对路径
                strncpy(all_addr, addr, strlen(addr));
            } else if(addr[0] == '~') {            //～替换为家目录
                //char home[MIN_SIZE]= "/home/zip";
                sprintf(all_addr,"%s/%s", passwd->pw_dir, addr+1);
            } else if(addr[0] == '-') {           //返回历史地址
                strncpy(all_addr,history, strlen(history));
                printf("%s\n", history);
            }else {                              //以当前路径为基准 寻找相对路径
                sprintf(all_addr,"%s/%s",wd, addr);
            }

            int judge = chdir(all_addr);

            if(judge != 0) {    //chdir 失败
                printf("bash: cd: %s: 没有那个文件或目录\n", addr);
            } else {
                if(count != 0) {    //保存历史记录
                    memset(history, 0, sizeof(history));
                    strncpy(history, history_new , strlen(history_new));
                }
                memset(history_new, 0, sizeof(history_new));
                strncpy(history_new, all_addr, strlen(all_addr));
                count=1;
            }
        } else if(strcmp(name, "exit") == 0) {
            break;
        } else {     //若输入的命令不是cd两个字母
            while(getchar() != 10) {}
            printf("%s：未找到命令\n",name);
            continue;
        }
    }
    return 0;
}
