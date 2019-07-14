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
#include <signal.h>
#include <sys/time.h>
#define MAX_USER_NUM 20
#define LEFTEDGE 6
#define RIGHTEDGE 146
#define UPEDGE 2
#define DOWNEDGE 37

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

#define SHOOT_DIST 18

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
    char name[10];
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

void *sendpos() {
    while(1) {
        for(int i = 1; i <= info.user_num; i++) {
            send(user[i].clnt_sock, &info, sizeof(info), 0);
        }
        usleep(50000);
    }
}

void reset_ball(){
    info.bpos.row = (UPEDGE + DOWNEDGE) / 2;
    info.bpos.col = (LEFTEDGE + RIGHTEDGE) / 2;
    b_mv.row = 0;
    b_mv.col = 0;
}

struct itimerval new_v;
int ball_move_flag = 0;
void ball_move_handle(int n) {
    if (new_v.it_value.tv_usec >= 100000) {
        ball_move_flag = 0;
        return;
    }
    info.bpos.col += b_mv.col;
    info.bpos.row += b_mv.row;
    new_v.it_value.tv_sec = 0;
    new_v.it_value.tv_usec += 5000;
    new_v.it_interval.tv_sec = 0;
    new_v.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &new_v, NULL);

}
void *ballmove() {
    //int clnt_sock;
    //clnt_sock = *(int *)arg;
    //int ball_moved_route = 0;
    while(1){
        if(info.bpos.col <= LEFTEDGE || info.bpos.col >= RIGHTEDGE) reset_ball();
        if(info.bpos.row < UPEDGE || info.bpos.row > DOWNEDGE) reset_ball();
        if(b_mv.col != 0 || b_mv.row != 0) {
            ball_move_flag = 1;
            signal(SIGALRM, ball_move_handle);
            new_v.it_value.tv_sec = 0;
            new_v.it_value.tv_usec = 5000;
            new_v.it_interval.tv_sec = 0;
            new_v.it_interval.tv_usec = 0;
            setitimer(ITIMER_REAL, &new_v, NULL);
            while(ball_move_flag) {
                usleep(20000);
            }
            b_mv.col = 0, b_mv.row = 0;
            printf("足球停止\n");
            //info.bpos.col += b_mv.col;
            //info.bpos.row += b_mv.row;
            //ball_moved_route += 1;
            //printf("ball_move_route = %d\n", ball_moved_route);
            //if(ball_moved_route >= SHOOT_DIST) b_mv.col = 0, b_mv.row = 0;
        }
        //if(b_mv.col == 0 && b_mv.row == 0) ball_moved_route = 0;
        usleep(20000);
    }
}

void user_quit(int uid) {
    info.ppos[uid].row = 0;
    info.ppos[uid].col = 0;
    strcpy(info.scr.nandc[uid].name, "****");
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
                break;
            case 'q': 
                close(clnt_sock);
                printf("user %s quit the game!\n", info.scr.nandc[uid].name);
                user_quit(uid);
                return NULL;
        }
        //printf("你输入的是 %c\n", ch);
        //send(clnt_sock, &ppos, sizeof(ppos), 0);
    }
}
void *score_get() {
    while(1){
        //进了左球门
        int col = info.bpos.col;
        int row = info.bpos.row;

        if(col > LDOORLEFT && col < LDOORRIGHT && row > DOORUP && row < DOORDOWN) {
            printf("%d 号玩家 %s 射了！左门！\n", final_shooter_id, info.scr.nandc[final_shooter_id].name);
            if(info.scr.nandc[final_shooter_id].camp == 2) {
                info.scr.nandc[final_shooter_id].score += 10;
                info.scr.score[1]+=10;
            }
            if(info.scr.nandc[final_shooter_id].camp == 1) {
                info.scr.nandc[final_shooter_id].score -= 5;
                info.scr.score[1] += 5;
                info.scr.score[0] -= 5;
            }
            reset_ball();
            usleep(2000000);
        }
        if(col > RDOORLEFT && col < RDOORRIGHT && row > DOORUP && row < DOORDOWN) {
            printf("%d 号玩家 %s 射了！右门！\n", final_shooter_id, info.scr.nandc[final_shooter_id].name);
            if(info.scr.nandc[final_shooter_id].camp == 1) {
                info.scr.nandc[final_shooter_id].score += 10;
                info.scr.score[0]+=10;
            }
            if(info.scr.nandc[final_shooter_id].camp == 2) {
                info.scr.nandc[final_shooter_id].score -= 5;
                info.scr.score[0] += 5;
                info.scr.score[1] -= 5;
            }
            reset_ball();
            usleep(2000000);
        }

        usleep(20000);
    }
}

int main() {
    info.bpos.row = (UPEDGE + DOWNEDGE) / 2;
    info.bpos.col = (LEFTEDGE + RIGHTEDGE) / 2;

	int serv_sock = get_serv_sock();
    int c_uid = 0;
    pthread_create(&user[c_uid].pthread_id[0], NULL, ballmove, NULL);
    printf("ballmove is Ready!\n");
    pthread_create(&user[c_uid].pthread_id[1], NULL, sendpos, NULL);
    printf("sendpos is Ready!\n");
    pthread_create(&user[c_uid].pthread_id[2], NULL, score_get, NULL);
    printf("score_get is Ready!\b");

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
        info.scr.nandc[c_uid].score = 0;

        pthread_create(&user[c_uid].pthread_id[0], NULL, recv_opt, &user[c_uid]);
        printf("recv_opt is Ready!\b");
    }

	return 0;
}

