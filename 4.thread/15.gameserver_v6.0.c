/*************************************************************************
	> File Name: 15.gameserver_v6.0.c
	> Author: Zip 
	> Mail: 307110017@qq.com 
	> Created Time: 2019年06月28日 星期五 20时53分04秒
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
#define SHOOT_DIST 18

#define UD_MID (UPEDGE/2+DOWNEDGE/2)
#define LR_MID (LEFTEDGE/2+RIGHTEDGE/2)
#define DOORWIDTH 12
#define DOORDEEPTH 8
#define LDOORLEFT (LEFTEDGE+2)
#define LDOORRIGHT (LEFTEDGE+2+DOORDEEPTH)
#define RDOORRIGHT (RIGHTEDGE-2)
#define RDOORLEFT (RIGHTEDGE-2-DOORDEEPTH)
#define DOORUP (UD_MID - (DOORWIDTH/2))
#define DOORDOWN (UD_MID + (DOORWIDTH/2))


int final_shooter_id;
struct my_ballpos {
    int row;
    int col;
};
struct my_peoplepos {
    int row;
    int col;
};

struct name_and_camp{
    char name[4];
    int camp;
    int score;
};

struct my_score {
    struct name_and_camp nandc[MAX_USER_NUM];
    int score[2];  //
};


struct my_info {
    struct my_ballpos bpos;
    struct my_peoplepos ppos[MAX_USER_NUM];
    int user_num;
    struct my_score scr;
};
struct my_info info;

struct all_user {
    struct sockaddr_in clnt_addr;
    int clnt_sock;
    int user_id;
	pthread_t pthread_id[5];
};
struct all_user user[MAX_USER_NUM];

struct ball_move {
    int row;
    int col;
};
struct ball_move b_mv;

int get_serv_sock() {
    int serv_sock;
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(sock_addr));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sock_addr.sin_port = htons(8888);
    bind(serv_sock, (struct sockaddr* )&sock_addr, sizeof(sock_addr));
    return serv_sock;
}

void *sendpos(void *arg) {
    int clnt_sock;
    clnt_sock = *(int *)arg;
    while(1){
        //printf("人物的坐标为 : %d %d \n", info.ppos[1].row, info.ppos[1].col);
        send(clnt_sock, &info, sizeof(info), 0);
        usleep(50000);
    }
}

void reset_ball(){
    info.bpos.row = (UPEDGE + DOWNEDGE) / 2;
    info.bpos.col = (LEFTEDGE + RIGHTEDGE) / 2;
    b_mv.row = 0;
    b_mv.col = 0;
}
void *ballmove(void *arg) {
    int clnt_sock;
    clnt_sock = *(int *)arg;
    int ball_moved_route = 0;
    while(1){
        if(info.bpos.col <= LEFTEDGE || info.bpos.col >= RIGHTEDGE) reset_ball();
        if(info.bpos.row < UPEDGE || info.bpos.row > DOWNEDGE) reset_ball();
        int times = 5;
        if(b_mv.col != 0 || b_mv.row != 0) {
            info.bpos.col += b_mv.col;
            info.bpos.row += b_mv.row;
            ball_moved_route += 1;
            //printf("ball_move_route = %d\n", ball_moved_route);
            if(ball_moved_route >= SHOOT_DIST) b_mv.col = 0, b_mv.row = 0;
        }
        if(b_mv.col == 0 && b_mv.row == 0) ball_moved_route = 0;
        usleep(20000);
    }
}

void* recv_opt(void* arg) {
    int clnt_sock;
    int my_last_row;
    int my_last_col;
    
    struct all_user* user;
    user = (struct all_user *)arg;
    clnt_sock = user->clnt_sock;
    
    //clnt_sock = (((struct all_user *)arg)->clnt_sock);
    char ch;
	while(1) {
        recv(clnt_sock, &ch, sizeof(ch), 0);
        //int col_change = 0;
        //int row_change = 0;
        int uid = user->user_id;
        int hit_dir_col;
        int hit_dir_row;
        switch(ch) {
            case 'a': 
                if(info.ppos[user->user_id].col > LEFTEDGE)  info.ppos[user->user_id].col -= 1; break;
            case 'd': 
                if(info.ppos[user->user_id].col < RIGHTEDGE) info.ppos[user->user_id].col += 1; break;
            case 's': 
                if(info.ppos[user->user_id].row < DOWNEDGE)  info.ppos[user->user_id].row += 1; break;
            case 'w': 
                if(info.ppos[user->user_id].row > UPEDGE)    info.ppos[user->user_id].row -= 1; break;
            case ' ':
                //printf("%d 号玩家射了 门！\n", uid);
                hit_dir_col = info.ppos[uid].col - info.bpos.col;
                hit_dir_row = info.ppos[uid].row - info.bpos.row;
                printf("col = %d , row = %d \n", hit_dir_col, hit_dir_row);
                if(hit_dir_row == 0 && hit_dir_col == -1) {b_mv.row = 0, b_mv.col = 1;  final_shooter_id = uid; break;}   
                if(hit_dir_row == -1 && hit_dir_col == 0) {b_mv.row = 1, b_mv.col = 0;  final_shooter_id = uid; break;} 
                if(hit_dir_row == 0 && hit_dir_col == 1)  {b_mv.row = 0, b_mv.col = -1; final_shooter_id = uid; break;}  
                if(hit_dir_row == 1 && hit_dir_col == 0)  {b_mv.row = -1, b_mv.col = 0; final_shooter_id = uid; break;}  
                if(hit_dir_row == -1 && hit_dir_col == -1){b_mv.row = 1, b_mv.col = 1;  final_shooter_id = uid; break;} 
                if(hit_dir_row == -1 && hit_dir_col == 1) {b_mv.row = 1, b_mv.col = -1; final_shooter_id = uid; break;} 
                if(hit_dir_row == 1 && hit_dir_col == -1) {b_mv.row = -1, b_mv.col = 1; final_shooter_id = uid; break;} 
                if(hit_dir_row == 1 && hit_dir_col == 1)  {b_mv.row = -1, b_mv.col = -1;final_shooter_id = uid; break;}
        }
        //printf("你输入的是 %c\n", ch);
        //send(clnt_sock, &ppos, sizeof(ppos), 0);
    }
}
void *score_get(void *arg) {
    int clnt_sock;
    clnt_sock = *(int *)arg;
    
    while(1){
        //进了左球门
        int col = info.bpos.col;
        int row = info.bpos.row;

        if(col > LDOORLEFT && col < LDOORRIGHT && row > DOORUP && row < DOORDOWN) {
            printf("%d 号玩家 %s 射了！左门！\n", final_shooter_id, info.scr.nandc[final_shooter_id].name);
        }
        if(col > RDOORLEFT && col < RDOORRIGHT && row > DOORUP && row < DOORDOWN) {
            printf("%d 号玩家 %s 射了！右门！\n", final_shooter_id, info.scr.nandc[final_shooter_id].name);
        }
        /*
        if(info.bpos.col <)
        if(info.bpos.col <= LEFTEDGE || info.bpos.col >= RIGHTEDGE) reset_ball();
        if(info.bpos.row < UPEDGE || info.bpos.row > DOWNEDGE) reset_ball();
        int times = 5;
        if(b_mv.col != 0 || b_mv.row != 0) {
            info.bpos.col += b_mv.col;
            info.bpos.row += b_mv.row;
            ball_moved_route += 1;
            printf("ball_move_route = %d\n", ball_moved_route);
            if(ball_moved_route >= SHOOT_DIST) b_mv.col = 0, b_mv.row = 0;
        }
        if(b_mv.col == 0 && b_mv.row == 0) ball_moved_route = 0;
        */
        usleep(20000);
    }
}

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
        struct name_and_camp *nNc;
        nNc = &info.scr.nandc[c_uid];
        recv(user[c_uid].clnt_sock, nNc, sizeof(struct name_and_camp), 0);
        char check_nNc = 'Y';
        send(user[c_uid].clnt_sock, &check_nNc, sizeof(check_nNc), 0);
        printf("初始化个人信息，姓名： %s , 阵营 %d \n", nNc->name, nNc->camp);
        
        user[c_uid].user_id = c_uid;
        info.user_num = c_uid; //用户总人数
        if(nNc->camp == 1) {
            info.ppos[c_uid].row = UPEDGE / 2 + DOWNEDGE / 2;
            info.ppos[c_uid].col = LEFTEDGE * 3 / 4 + RIGHTEDGE / 4;
        } else {
            info.ppos[c_uid].row = UPEDGE / 2 + DOWNEDGE / 2;
            info.ppos[c_uid].col = LEFTEDGE / 4 + RIGHTEDGE * 3 / 4;

        }
        printf("玩家位置初始化成功\n");

        pthread_create(&user[c_uid].pthread_id[0], NULL, ballmove, &user[c_uid].clnt_sock);
        printf("ballmove is Ready!\n");
        pthread_create(&user[c_uid].pthread_id[1], NULL, sendpos, &user[c_uid].clnt_sock);
        printf("sendpos is Ready!\n");
        pthread_create(&user[c_uid].pthread_id[2], NULL, recv_opt, &user[c_uid]);
        printf("recv_opt is Ready!\b");
        pthread_create(&user[c_uid].pthread_id[3], NULL, score_get, &user[c_uid].clnt_sock);
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

