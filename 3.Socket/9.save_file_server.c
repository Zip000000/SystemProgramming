/*************************************************************************
	> File Name: 9.save_file_server.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月19日 星期三 09时53分27秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>

#include <pthread.h>

#define MAXBUF 1024
#define MAXFILENAME 50

int is_exit(char* filename) {
    if(fopen(filename, "r") == NULL) return 0;
    else return 1;
}

char* get_newfname(char* oldfname, char* newfname) {
    char temp[MAXFILENAME];
    strcpy(temp, oldfname);
    strcpy(newfname, temp);
    int count = 1;
    char add[5];
    add[0] = '_';
    while(is_exit(newfname)) {
        memset(newfname, 0, sizeof(newfname));
        sprintf(newfname, "%s_%d", oldfname, count);
        count++;
    }
    //printf("********newname = %s\n", newfname);
    return newfname;
}
void *saveit(void *arg) {
    int clnt_sock;
    clnt_sock = *(int *)arg;

        printf("进入子线程！\n");
        char filename[MAXFILENAME];
        memset(filename, 0, sizeof(filename));
        int checkfilename;
        checkfilename = recv(clnt_sock, &filename, MAXFILENAME, 0);
        send(clnt_sock, &checkfilename, 4, 0);
        char buf[MAXBUF];
        FILE *fp_to;
        char *temp;
        char oldfname[MAXFILENAME];
        strcpy(oldfname, filename);
        if(strrchr(filename, '/') != NULL) {
            printf("get temp\n");
            temp = strrchr(filename, '/');
            printf("temp = %s\n", temp);
            strcpy(oldfname, temp+1);
    }
        char newfilename[MAXFILENAME];
        if((fp_to = fopen(get_newfname(oldfname, newfilename), "wb+")) == NULL) {
            perror("fopen");
            exit(1);
        }

        int ncount;
        while((ncount = recv(clnt_sock, &buf, MAXBUF, 0)) >=0) {
            if(ncount == 0) {
                fclose(fp_to);
                break;
            }
            if((fwrite(buf, 1, ncount, fp_to)) <=0) {
                perror("fwrite");
                exit(1);
            }
            memset(buf, 0, sizeof(buf));
        }
        //printf("PID = %d : ID : %d : newfilename = %s\n", getpid(), my_id, filename);
        sleep(1);
        printf("***************************************接受文件成功！文件名为：[%s]\n", newfilename);
    close(clnt_sock);

	return NULL;

}


int main() {
    pthread_t pthread_id;
    int id_cnt = 0;
    int serv_sock;
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sock_addr.sin_port = htons(8888);
    bind(serv_sock, (struct sockaddr* )&sock_addr, sizeof(sock_addr));
    listen(serv_sock, 100);
    printf("正在等待对方发送文件......\n");

    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock;
    while(1) {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        printf("第 %d 次连接：\n", id_cnt);
        printf("连接成功，开始接收文件......\n");
        pthread_create(&pthread_id, NULL, saveit, &clnt_sock);
        //pthread_join(pthread_id, NULL);
        //printf("id_cnt = %d, pthread_id =  传输完成！\n", id_cnt);
        id_cnt++;
    }
    close(serv_sock);
}
