/*************************************************************************
	> File Name: homework_4_23.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年04月24日 星期三 10时05分51秒
 ************************************************************************/


#include<stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 200
#define MIN_SIZE 50

void show() {
    char wd[MAX_SIZE];
    getcwd(wd, sizeof(wd));
	struct passwd* passwd;
    passwd = getpwuid(getuid());
    char hostname[32];
    gethostname(hostname,sizeof(hostname));   //<unistd.h>
    printf("\033[;32m%s\033[0m", passwd->pw_name);
    printf("\033[;31m@\033[0m");
    printf("\033[;33m%s\033[0m", hostname);
    printf("\033[;31m:\033[0m");
    printf("\033[;34m%s\033[0m", wd);
    if(strcmp(passwd->pw_name, "root") == 0) {
        printf("# ");
    } else {
        printf("$ ");
    }
}


int main() {
    char name[MIN_SIZE];
    char addr[MAX_SIZE];
    char history[MAX_SIZE];
    char history_new[MAX_SIZE];
    char wd[MAX_SIZE];
    getcwd(wd,sizeof(wd));
    //memset(history, 0, sizeof(history));
    strncpy(history, wd, strlen(wd));
    int count = 0;
    char all_addr[MAX_SIZE];
    while(1) {
        getcwd(wd,sizeof(wd));
        show();
        scanf("%s",name);

        if(name[0] == 'c' && name[1] == 'd' && strlen(name) == 2) {
            //printf("sizeof(name) = %d\n",strlen(name));
            if(getchar() == 10) {
                memset(addr, 0, strlen(addr));
                addr[0] = '~';
            } else {
                scanf("%s",addr);
            }
            if(addr[0] == '/') {
                memset(all_addr, 0, strlen(all_addr));  //可以重构一下
                strncpy(all_addr,addr,strlen(addr));
            } else if(addr[0] == '~') {
                char home[MIN_SIZE]= "/home/zip";
                memset(all_addr, 0, strlen(all_addr));
                sprintf(all_addr,"%s/%s", home, addr+1);
            } else if(addr[0] == '-') {
                memset(all_addr, 0, strlen(all_addr));
                strncpy(all_addr,history, strlen(history));
                printf("%s\n", history);
            }else {
                memset(all_addr, 0, strlen(all_addr));
                sprintf(all_addr,"%s/%s",wd, addr);
            }

            int judge = 0;
            judge = chdir(all_addr);

            if(judge != 0) {
                printf("bash: cd: %s: 没有那个文件或目录\n", addr);
            } else {
                if(count != 0) {
                    memset(history, 0, sizeof(history));
                    strncpy(history, history_new , strlen(history_new));
                }
                memset(history_new, 0, sizeof(history_new));
                strncpy(history_new, all_addr, strlen(all_addr));
                count=1;
            }
        } else if(name[0] == 'e' && name[1] == 'x' && name[2] == 'i' && name[3] == 't') {
            break;
        } else {
            while(getchar()!=10) {}
            printf("%s：未找到命令\n",name);
            continue;
        }
    }
    return 0;
}
