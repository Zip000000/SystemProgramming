/*************************************************************************
	> File Name: 6.homework_cd_improved.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年04月30日 星期二 17时54分11秒
 ************************************************************************/


/*************************************************************************
	> File Name: 4.homework_4_25_improved.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年04月27日 星期六 11时03分30秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <signal.h>
//ls:
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#define MAX_SIZE 200
#define MIN_SIZE 50
	
struct passwd* passwd;
enum DAY {
    CMD_NOT_FOUND = 0, CD_CMD = 1, LS_CMD
};

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

void cd_handle() {
    char addr[MAX_SIZE];    //存放键入的 不完整路径
    char history[MAX_SIZE];     //存放上次的记录
    char history_new[MAX_SIZE];  //存放本次的记录
    char wd[MAX_SIZE];        //存放当前工作路径
    memset(wd, 0, strlen(wd));
    getcwd(wd,sizeof(wd));
    strncpy(history, wd, strlen(wd));  // history初始化
    int count = 0;   //第一次为0   其余情况都为1
    char all_addr[MAX_SIZE];   //存放完整的路径

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
    return ;
}
void ls_handle() {

}

int cmd_judge(char* cmd) {
    printf("cmd in judge = %s\n\n", cmd);
    if(strcmp(cmd, "exit") == 0) {
        return -1;
    } else if(strcmp(cmd, "cd") == 0) {
        return CD_CMD;
    } else if(strcmp(cmd, "ll") == 0) {
        return LS_CMD;
    } else {     //若输入的命令不是cd两个字母
        while(getchar() != 10) {}
		return 0;
    }

}

int main() {
    signal(SIGINT,SIG_IGN);   //忽略ctrl c
    char wd[MAX_SIZE];        //存放当前工作路径
    char cmd[MIN_SIZE];
    
    while(1) {
        memset(wd, 0, strlen(wd));
        getcwd(wd, sizeof(wd));
        show();
        scanf("%s", cmd);
        int judge = cmd_judge(cmd);
        if(judge == -1) {
            break;
        }else if(judge == CMD_NOT_FOUND) {
            printf("%s：未找到命令\n",cmd);
        } else if(judge == CD_CMD) {
            cd_handle();
        } else if(judge == LS_CMD) {
            ls_handle();

        }

    }
    return 0;
}
