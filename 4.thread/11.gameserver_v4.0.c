/*************************************************************************
	> File Name: 11.gameserver_v4.0.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月23日 星期日 13时44分11秒
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
#include <curses.h>
#include <unistd.h>
#define MAX_USER_NUM 20
#define LEFTEDGE 10
#define RIGHTEDGE 150
#define UPEDGE 2
#define DOWNEDGE 37

struct my_ballpos {
    int row;
    int col;
};
struct my_peoplepos {
    int row;
    int col;
};
struct my_info {
    struct my_ballpos bpos;
    struct my_peoplepos ppos[MAX_USER_NUM];
    int user_num;
};
struct my_info info;

struct all_user {
    struct sockaddr_in clnt_addr;
    int clnt_sock;
    int user_id;
	pthread_t pthread_id[5];
};
struct all_user user[MAX_USER_NUM];

int get_serv_sock() {
    int serv_sock;
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr("192.168.2.90");
    sock_addr.sin_port = htons(1234);
    bind(serv_sock, (struct sockaddr* )&sock_addr, sizeof(sock_addr));
    return serv_sock;
}

void *sendpos(void *arg) {
    int clnt_sock;
    clnt_sock = *(int *)arg;
    while(1){
        printf("人物的坐标为 : %d %d \n", info.ppos[1].row, info.ppos[1].col);
        send(clnt_sock, &info, sizeof(info), 0);
        usleep(50000);
    }
}

void* recv_opt(void* arg) {
    int clnt_sock;
    
    struct all_user* user;
    user = (struct all_user *)arg;
    clnt_sock = user->clnt_sock;
    
    //clnt_sock = (((struct all_user *)arg)->clnt_sock);
    char ch;
	while(1) {
        recv(clnt_sock, &ch, sizeof(ch), 0);
        switch(ch) {
            case 'a': 
                if(info.ppos[user->user_id].col > LEFTEDGE)  info.ppos[user->user_id].col -= 1; break;
            case 'd': 
                if(info.ppos[user->user_id].col < RIGHTEDGE) info.ppos[user->user_id].col += 1; break;
            case 's': 
                if(info.ppos[user->user_id].row < DOWNEDGE)  info.ppos[user->user_id].row += 1; break;
            case 'w': 
                if(info.ppos[user->user_id].row > UPEDGE)    info.ppos[user->user_id].row -= 1; break;
        }
        
        printf("你输入的是 %c\n", ch);
        //send(clnt_sock, &ppos, sizeof(ppos), 0);
    }
}

struct name_and_camp{
    char name[4];
    int camp;
};

int main() {
    info.bpos.row = (UPEDGE + DOWNEDGE) / 2;
    info.bpos.col = (LEFTEDGE + RIGHTEDGE) / 2;

	int serv_sock = get_serv_sock();
    int c_uid = 0;
    while(1) {
        c_uid += 1;
        listen(serv_sock, 100);
        printf("正在监听新玩家的加入......\n");
        socklen_t clnt_addr_size = sizeof(user[c_uid].clnt_addr);

        user[c_uid].clnt_sock = accept(serv_sock, (struct sockaddr*)&user[c_uid].clnt_addr, &clnt_addr_size);
        printf("第 %d 个玩家连接成功！\n", c_uid);
        printf("正在接收昵称与阵营选择......\n");
        struct name_and_camp nNc;
        recv(user[c_uid].clnt_sock, &nNc, sizeof(nNc), 0);
        char check_nNc = 'Y';
        send(user[c_uid].clnt_sock, &check_nNc, sizeof(check_nNc), 0);
        printf("初始化个人信息，姓名： %s , 阵营 %d \n", nNc.name, nNc.camp);
        
        user[c_uid].user_id = c_uid;
        info.user_num = c_uid; //用户总人数
        if(nNc.camp == 1) {
            info.ppos[c_uid].row = UPEDGE / 2 + DOWNEDGE / 2;
            info.ppos[c_uid].col = LEFTEDGE * 3 / 4 + RIGHTEDGE / 4;
        } else {
            info.ppos[c_uid].row = UPEDGE / 2 + DOWNEDGE / 2;
            info.ppos[c_uid].col = LEFTEDGE / 4 + RIGHTEDGE * 3 / 4;

        }
        printf("玩家位置初始化成功\n");

        pthread_create(&user[c_uid].pthread_id[0], NULL, sendpos, &user[c_uid].clnt_sock);
        printf("sendpos is Ready!\n");
        pthread_create(&user[c_uid].pthread_id[1], NULL, recv_opt, &user[c_uid]);
        printf("recv_opt is Ready!\b");
    }

	return 0;
}




/*
int clear_row;
int clear_col;
char ball[2] = "o";
char blank[2] = " ";
void draw(struct my_ballpos* ppos) {
    move(ppos->row, ppos->col);
    addstr(ball);
    
    move(LINES - 1, COLS - 1);
    refresh();
    //sleep(1);

    move(clear_row, clear_col);
    addstr(blank);
    
    clear_row = ppos->row;
    clear_col = ppos->col;
}
void makescreen() {
    initscr();
    clear();
    int x = LEFTEDGE - 1;
    int y = UPEDGE - 1;
    refresh();
    for(int i = LEFTEDGE - 1; i <= RIGHTEDGE + 1; i++) {
        move(UPEDGE - 1, i);
        addstr("*");
        move(DOWNEDGE + 1, i);
        addstr("*");
    }
    for(int j = UPEDGE - 1; j <= DOWNEDGE + 1; j++) {
        move(j, LEFTEDGE - 1);
        addstr("*");
        move(j, RIGHTEDGE + 1);
        addstr("*");
    }
    refresh();
}
*/

