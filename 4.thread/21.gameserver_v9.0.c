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
#include <sys/epoll.h>

#define MAX_USER_NUM 1000
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

int epollfd;
void add_event(int epollfd,struct all_user *ptr,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.ptr = ptr;
    int fd = ptr->clnt_sock;
    int ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    if(ret == -1) {perror("epoll_ctl_ADD");}
}

void delete_event(int epollfd,struct all_user *ptr,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.ptr = ptr;
    int fd = ptr->clnt_sock;
    int ret = epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, NULL);
    if(ret == -1) {perror("epoll_ctl_DEL");}
}

void modify_event(int epollfd,struct all_user *ptr,int state)
{
    struct epoll_event ev;
    ev.events = state;
    ev.data.ptr = ptr;
    int fd = ptr->clnt_sock;
    int ret = epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&ev);
    if(ret == -1) {perror("epoll_ctl_MOD");}
}



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
void getready_send() {
    for(int i = 1; i <= info.user_num; i++) {
        if(user[i].clnt_sock < 0) continue;
        modify_event(epollfd, &user[i], EPOLLIN|EPOLLOUT);
    }
}

void single_send(int now_fd) {
    int nsend = send(now_fd, &info, sizeof(info), 0);
    printf("send to  fd = %d \n", now_fd);
    if(nsend == -1) perror("send in sendpos");
    if(nsend == 0 ) printf("send 0 bits! WTF \n");
}
void reset_ball(){
    info.bpos.row = (UPEDGE + DOWNEDGE) / 2;
    info.bpos.col = (LEFTEDGE + RIGHTEDGE) / 2;
    b_mv.row = 0;
    b_mv.col = 0;
}
/*
void check_score() {
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
        } else if(col > RDOORLEFT && col < RDOORRIGHT && row > DOORUP && row < DOORDOWN) {
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
        }
    return;
}
*/

struct itimerval new_v;
void ball_move_handle(int n) {
    int ball_move_flag = 0;
    if(info.bpos.col <= LEFTEDGE || info.bpos.col >= RIGHTEDGE) ball_move_flag = 1;
    if(info.bpos.row < UPEDGE || info.bpos.row > DOWNEDGE) ball_move_flag = 1;
    if (new_v.it_value.tv_usec >= 110000 || ball_move_flag == 1) {
        if(ball_move_flag == 1) reset_ball();
        b_mv.col = 0, b_mv.row = 0;
        printf("足球停止\n");
        getready_send();
        return;
    }
    info.bpos.col += b_mv.col;
    info.bpos.row += b_mv.row;
    getready_send();

    //check_score();
    
    new_v.it_value.tv_sec = 0;
    new_v.it_value.tv_usec += 10000;
    new_v.it_interval.tv_sec = 0;
    new_v.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &new_v, NULL);
    printf("再次设置了 iTimer\n");

}
void ballmove() {
        if(info.bpos.col <= LEFTEDGE || info.bpos.col >= RIGHTEDGE) reset_ball();
        if(info.bpos.row < UPEDGE || info.bpos.row > DOWNEDGE) reset_ball();
        if(b_mv.col != 0 || b_mv.row != 0) {
            new_v.it_value.tv_sec = 0;
            new_v.it_value.tv_usec = 1000;
            new_v.it_interval.tv_sec = 0;
            new_v.it_interval.tv_usec = 0;
            
            signal(SIGALRM, ball_move_handle);
            
            setitimer(ITIMER_REAL, &new_v, NULL);
            printf("第一次设置 iTimer\n");
            return;
        }
}

void user_quit(struct all_user *user) {
    int uid = user->user_id;
    info.ppos[uid].row = 0;
    info.ppos[uid].col = 0;
    strcpy(info.scr.nandc[uid].name, "*OUT*");

    delete_event(epollfd, user, 0);
    user->clnt_sock = -1;
    close(user->clnt_sock);
}

int recv_opt(void* arg) {
    int clnt_sock;
    struct all_user* user;
    user = (struct all_user *)arg;
    clnt_sock = user->clnt_sock;
    char ch;
    int nrecv = recv(clnt_sock, &ch, sizeof(ch), 0);
    if(nrecv == 0) {
        printf("收到0字节，看来玩家%d 退游了。\n",user->user_id);
        return -1;
    }
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
                hit_dir_col = info.ppos[uid].col - info.bpos.col;
                hit_dir_row = info.ppos[uid].row - info.bpos.row;
                printf("col = %d , row = %d \n", hit_dir_col, hit_dir_row);
            #define STEP_LENGTH 2
                if(hit_dir_row == 0 && hit_dir_col == -1) {b_mv.row = 0, b_mv.col = 2;  final_shooter_id = uid; ballmove(); break;}   
                if(hit_dir_row == -1 && hit_dir_col == 0) {b_mv.row = 1, b_mv.col = 0;  final_shooter_id = uid; ballmove(); break;} 
                if(hit_dir_row == 0 && hit_dir_col == 1)  {b_mv.row = 0, b_mv.col = -2; final_shooter_id = uid; ballmove(); break;}  
                if(hit_dir_row == 1 && hit_dir_col == 0)  {b_mv.row = -1, b_mv.col = 0; final_shooter_id = uid; ballmove(); break;}  
                if(hit_dir_row == -1 && hit_dir_col == -1){b_mv.row = 1, b_mv.col = 2;  final_shooter_id = uid; ballmove(); break;} 
                if(hit_dir_row == -1 && hit_dir_col == 1) {b_mv.row = 1, b_mv.col = -2; final_shooter_id = uid; ballmove(); break;} 
                if(hit_dir_row == 1 && hit_dir_col == -1) {b_mv.row = -1, b_mv.col = 2; final_shooter_id = uid; ballmove(); break;} 
                if(hit_dir_row == 1 && hit_dir_col == 1)  {b_mv.row = -1, b_mv.col = -2;final_shooter_id = uid; ballmove(); break;}
                break;
            case 'q': 
                printf("user %s 按q 主动退出了游戏!\n", info.scr.nandc[uid].name);
                return -1;
        }
    return 0;
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

void user_info_init(int c_uid) {
    struct name_and_camp *nNc;
    nNc = &info.scr.nandc[c_uid];
    recv(user[c_uid].clnt_sock, nNc, sizeof(struct name_and_camp), 0);
    //char check_nNc = 'Y';
    send(user[c_uid].clnt_sock, &c_uid, sizeof(c_uid), 0);
    printf("初始化个人信息，姓名： %s , 阵营 %d \n", nNc->name, nNc->camp);

    if(nNc->camp == 1) {
        info.ppos[c_uid].row = UPEDGE / 2 + DOWNEDGE / 2;
        info.ppos[c_uid].col = LEFTEDGE * 3 / 4 + RIGHTEDGE / 4;
    } else {
        info.ppos[c_uid].row = UPEDGE / 2 + DOWNEDGE / 2;
        info.ppos[c_uid].col = LEFTEDGE / 4 + RIGHTEDGE * 3 / 4;
    }
    info.scr.nandc[c_uid].score = 0;
    printf("玩家位置初始化成功\n");

}

int main() {
    reset_ball();
	int serv_sock = get_serv_sock();

    struct epoll_event events[MAX_USER_NUM];
    epollfd = epoll_create(MAX_USER_NUM);
    if(epollfd == -1) {perror("epoll_create"); exit(1);}
    int c_uid = 0;
    user[c_uid].clnt_sock = serv_sock;
    add_event(epollfd, &user[c_uid], EPOLLIN);
    pthread_create(&user[c_uid].pthread_id[2], NULL, score_get, NULL);
    printf("score_get is Ready!\b");

    sigset_t sigmask;
    sigemptyset(&sigmask);
    sigaddset(&sigmask, SIGALRM);
    listen(serv_sock, 20);
    printf("正在监听新玩家的加入......\n");
    while(1) {
        printf("等待玩家操作......\n");
        int nfds = epoll_pwait(epollfd, events, MAX_USER_NUM, -1, &sigmask);
        if(nfds == -1) {perror("epoll_wait"); exit(1);}
        printf("epoll_wait got message! nfds = %d\n", nfds);
        for (int i = 0; i < nfds; i++) {
            int now_fd = ((struct all_user *)events[i].data.ptr)->clnt_sock;
            if(now_fd == serv_sock) {
                c_uid += 1;
                socklen_t clnt_addr_size = sizeof(user[c_uid].clnt_addr);
                user[c_uid].clnt_sock = accept(serv_sock, (struct sockaddr*)&user[c_uid].clnt_addr, &clnt_addr_size);
                if(user[c_uid].clnt_sock == -1) {perror("accept");}
                else printf("第 %d 个玩家连接成功！\n", c_uid);

                user[c_uid].user_id = c_uid;
                info.user_num = c_uid; //连接过的用户总人数
                
                printf("正在接收昵称与阵营选择......\n");
                user_info_init(c_uid); // 初始化个人信息
                
                add_event(epollfd, &user[c_uid], EPOLLOUT);
            } else if(events[i].events & EPOLLIN) {
                int now_id = ((struct all_user *)(events[i].data.ptr))->user_id;
                printf("玩家 %d  整事了!\n", now_id);
                int ret_recv = recv_opt(&user[now_id]);
                if(ret_recv == -1 ) {
                    user_quit(&user[now_id]);
                    printf("玩家后事处理完毕\n");
                }
                getready_send();

            } else if(events[i].events & EPOLLOUT) {
                single_send(now_fd);
                modify_event(epollfd, events[i].data.ptr, EPOLLIN);
            } else {
                printf("??????WHAT THE FUCK ????\n");
            }
        }
        
    }

	return 0;
}




    //pthread_create(&user[c_uid].pthread_id[0], NULL, ballmove, NULL);
    //printf("ballmove is Ready!\n");
    //pthread_create(&user[c_uid].pthread_id[1], NULL, sendpos, NULL);
    //printf("sendpos is Ready!\n");
    //

                //pthread_create(&user[c_uid].pthread_id[0], NULL, recv_opt, &user[c_uid]);
                
/*
void sendpos() {
    //while(1) {
    printf("开始 send 数据 :  user_num = %d\n", info.user_num);
        for(int i = 1; i <= info.user_num; i++) {
            if(user[i].clnt_sock == -1) continue;
            int nsend = send(user[i].clnt_sock, &info, sizeof(info), 0);
            if(nsend == -1) perror("send in sendpos");
            if(nsend == 0 ) printf("send 0 bits! WTF : user %d\n", i);
        }
    printf("send over\n");
        //usleep(50000);
    //}
}
*/

