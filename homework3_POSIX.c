# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/sem.h>
# include <stdio.h>
# include <wait.h>
# include <unistd.h>
# define SHMKEY1 18001                          //共享内存1，对应缓冲区
# define SHMKEY2 18002                          //共享内存2，对应最后输出
# define SK1SIZE 1024                           //共享内存1的字节数
# define SK2SIZE 8                              //共享内存2的字节数
# define SEMKEYS1 19001                         //信号量1，lock，负责只有一个进程在缓冲区操作
# define SEMKEYS2 19002                         //信号量2，代表缓冲区目前是否有商品
# define SEMKEYS3 19003                         //信号量3，代表缓冲区目前是否可生产
# define RANGE 10000000                         //数据长度

int createsem(key_t key,int num);               //创建和初始化信号量，key为信号量编号，num为信号量初始值
void P(int sid);                                //P操作，信号量-1
void V(int sid);                                //V操作，信号量+1
void semcall(int sid, int op);                  //实际调用函数，负责加减信号量

int main(){
    int* segaddr;
    long long* resaddr;
    int segid, resgid,the_mutex, products_left,rooms_left;
    if ((segid = shmget(SHMKEY1, SK1SIZE, IPC_CREAT | 0666)) == -1) 
        perror("shmget error");
    if ((resgid = shmget(SHMKEY2, SK2SIZE, IPC_CREAT | 0666)) == -1) 
        perror("shmget error");
    segaddr = shmat(segid, 0, 0);               //共享内存1，其中segaddr[0]记录缓冲区存放的商品数，其余为缓冲区
    resaddr = shmat(resgid,0,0);                //共享内存2，最终输出单元
    segaddr[0]=0;                               //初始化
    resaddr[0]=0;
    the_mutex = createsem(SEMKEYS1,1);          //信号量1，缓冲区lock
    products_left = createsem(SEMKEYS2,0);      //信号量2，缓冲区是否可消费。开始由于没数据，所以为0
    rooms_left = createsem(SEMKEYS3,255);       //信号量3，缓冲区是否可生产。长度为1024/4-1

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
            P(products_left);                   //判断是否还有商品可消费
            P(the_mutex);                       //lock
            count=--segaddr[0];                 //缓冲区商品数-1，
            i++;
            sum=sum+segaddr[1+count];           //取出缓冲区最后一个元素
            V(rooms_left);                      //可生产数+1
            V(the_mutex);                       //unlock
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
            P(rooms_left);                      //判断是否可生产
            P(the_mutex);                       //lock
            i++;
            if(pid1>0&&(i%(RANGE/100))==0){	//打印操作，记录完成状态
                int rate=i/(RANGE/100);
                fflush(stdout); 
                printf("[%-100s],%d%%,[%c]\r", bar, rate, sta[rate % 4]);
                bar[rate] = '=';
                rate++;
                bar[rate] = '\0';
            }
            count=++segaddr[0];                 //商品数+1
            segaddr[count]=i;                   //在第一个没有使用的位置存入数据
            V(products_left);                   //产品数+1
            V(the_mutex);                       //unlock
        }
        waitpid(pid2,NULL,0);                   //两个父进程等待各自的子进程
        if(pid1>0){
            waitpid(pid1,NULL,0);               //最初的父进程等待另一个生产者进程
            printf("\nsum=%lld\n",resaddr[0]);         //最初进程输出数据
        }
        return 0; 
    }
    
}
int createsem(key_t key,int num) {
    int sid;
    union semun{
        int val;
        struct semid_ds* buf;
        ushort* array;
    } arg;
    if ((sid=semget(key,1,0666|IPC_CREAT))==-1)
        perror("semget error");
    arg.val = num;
    if(semctl(sid,0,SETVAL,arg)==-1)
        perror("semctl error");
    return (sid);
}

void P(int sid){
    semcall(sid, -1);
}

void V(int sid){
    semcall(sid, 1);
}

void semcall(int sid,int op){
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = op;
    sb.sem_flg = 0;
    if(semop(sid,&sb,1)==-1)
        perror("semcall error");
}
