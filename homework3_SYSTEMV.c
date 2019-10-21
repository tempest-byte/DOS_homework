# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/sem.h>
# include <stdio.h>
# include <wait.h>
# include <unistd.h>
# include "semaphore.h"
# include <fcntl.h>
# define SHMKEY1 18001                          //共享内存1，对应缓冲区
# define SHMKEY2 18002                          //共享内存2，对应最后输出
# define SK1SIZE 1024                           //共享内存1的字节数
# define SK2SIZE 8                              //共享内存2的字节数
# define RANGE 10000000                         //数据长度


int main(){
    int* segaddr;
    long long* resaddr;
    int segid, resgid;
    sem_t *the_mutex, *products_left,*rooms_left;
    if ((segid = shmget(SHMKEY1, SK1SIZE, IPC_CREAT | 0666)) == -1) 
        perror("shmget error");
    if ((resgid = shmget(SHMKEY2, SK2SIZE, IPC_CREAT | 0666)) == -1) 
        perror("shmget error");
    segaddr = shmat(segid, 0, 0);                                   //共享内存1，其中segaddr[0]记录缓冲区存放的商品数，其余为缓冲区
    resaddr = shmat(resgid,0,0);                                    //共享内存2，最终输出单元
    segaddr[0]=0;                                                   //初始化
    resaddr[0]=0;
    the_mutex =  sem_open("the_mutex", O_CREAT, 0664, 1);           //信号量1，缓冲区lock
    products_left = sem_open("products_left", O_CREAT, 0664, 0);    //信号量2，缓冲区是否可消费。开始由于没数据，所以为0
    rooms_left = sem_open("rooms_left", O_CREAT, 0664, 255);        //信号量3，缓冲区是否可生产。长度为1024/4-1

    int pid1=fork();                            //创建一个子进程。与最初的父进程共为生产者
    if(pid1<0)
        perror("fork producer error");
    int pid2 = fork();
    if(pid2==-1)                                //创建两个子进程，这两个子进程共为消费者
        perror("fork consumer error");
    else if(pid2==0){                           //消费者进程
	    long long sum=0;                        //sum为消费者累加数据
	    int count;                              //count作为中间值计算得到从缓冲区取的位置
        int i=0;                                //退出flag，消费者消费总数据的一半就退出
        while(i<RANGE){
            sem_wait(products_left);            //判断是否还有商品可消费
            sem_wait(the_mutex);                //lock
            count=--segaddr[0];                 //缓冲区商品数-1，
            i++;
            sum=sum+segaddr[1+count];           //取出缓冲区最后一个元素
            sem_post(rooms_left);               //可生产数+1
            sem_post(the_mutex);                //unlock
        }
        resaddr[0]+=sum;
    } else {                                    //生产者进程
        int count;
        int i=0;
        int rate ;                              //以下四个变量负责可视化输出进度
        char bar[102] = {0};
	    bar[0]='=';
        const char* sta = "-\\|/";
        while(i<RANGE){
            sem_wait(rooms_left);                      //判断是否可生产
            sem_wait(the_mutex);                       //lock
            i++;
            if(pid1>0&&(i%(RANGE/100))==0){	//打印操作，记录完成状态
                int rate=i/(RANGE/100);
                fflush(stdout); 
                printf("[%-100s],%d%%,[%c]\r", bar, rate, sta[rate % 4]);
                bar[rate] = '=';
                rate++;
                bar[rate] = '\0';
            }
            count=++segaddr[0];                         //商品数+1
            segaddr[count]=i;                           //在第一个没有使用的位置存入数据
            sem_post(products_left);                    //产品数+1
            sem_post(the_mutex);                        //unlock
        }
        waitpid(pid2,NULL,0);                           //两个父进程等待各自的子进程
        if(pid1>0){
            waitpid(pid1,NULL,0);                       //最初的父进程等待另一个生产者进程
            printf("\nsum=%lld\n",resaddr[0]);          //最初进程输出数据
                sem_close(the_mutex);
                sem_unlink("the_mutex");
                sem_close(products_left);
                sem_unlink("products_left");
                sem_close(rooms_left);
                sem_unlink("rooms_left");
        }
        return 0; 
    }    
}
