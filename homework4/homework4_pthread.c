#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#define RANGE 10000000                          //生产者生产范围
#define ARRAY_NUM 200                           //缓冲区大小


pthread_cond_t condc,condp;                     //condc：消费者条件变量,condp：生产者条件变量
pthread_mutex_t the_mutex;                      //线程锁
int array[ARRAY_NUM];                           //缓冲区
int count=0;                                    //目前缓冲区滞留商品数目
long long res=0;                                //最终输出


void *producer1(void *ptr){                     //生产者1
    int rate;                                   //以下四个变量负责可视化输出进度
    char bar[102] = {0};
    bar[0] = '=';
    const char* sta = "-\\|/";
    for(int i = 1; i <= RANGE; i++){
        if ((i % (RANGE / 100)) == 0) {         //打印操作，记录完成状态
            int rate = i / (RANGE / 100);
            fflush(stdout);
            printf("[%-100s],%d%%,[%c]\r", bar, rate, sta[rate % 4]);
            bar[rate] = '=';
            rate++;
            bar[rate] = '\0';
        }
        pthread_mutex_lock(&the_mutex);
        while(count==ARRAY_NUM) pthread_cond_wait(&condp, &the_mutex);  //每次唤醒首先检查缓冲区是否还有位置生产商品
        array[count++]=i;                       //在第一个可用位置放入商品，并且count+1
        if(count>=2)pthread_cond_signal(&condc);//如果商品数大于2，唤醒两个消费者线程
        pthread_mutex_unlock(&the_mutex);
    }
    pthread_exit(NULL);
}

void *producer2(void *ptr){                     //生产者2，和生产者1只在可视化进度方面不同
    for(int i = 1; i <= RANGE; i++){
        pthread_mutex_lock(&the_mutex);
        while(count==ARRAY_NUM) pthread_cond_wait(&condp, &the_mutex);
        array[count++]=i;
        if(count>=2)pthread_cond_broadcast(&condc);
        pthread_mutex_unlock(&the_mutex);
    }
    pthread_exit(NULL);
}

void *consumer1(void *ptr){                     //消费者1，消费者每消费RANGE个数据就结束
    long long sum=0;                            //消费者1的商品总和
    for(int i = 1; i <= RANGE/2; i++){
        pthread_mutex_lock(&the_mutex);
        while(count<2) pthread_cond_wait(&condc, &the_mutex);//每次唤醒首先检查缓冲区商品数是否够读取的
        sum+=array[--count];                    //连续从尾部取出两个商品，所以需要RANGE/2步操作
        sum+=array[--count];    
        pthread_cond_signal(&condp);
        pthread_mutex_unlock(&the_mutex);
    }
    res+=sum;                                   //每个消费者结束将总值提供给res
    pthread_exit(NULL);
}
void *consumer2(void *ptr){                     //消费者2，同1不再解释
    long long sum=0;
    for(int i = 1; i <= RANGE/2; i++){
        pthread_mutex_lock(&the_mutex);
        while(count<2) pthread_cond_wait(&condc, &the_mutex);
        sum+=array[--count];
        sum+=array[--count];    
        pthread_cond_signal(&condp);
        pthread_mutex_unlock(&the_mutex);
    }
    res+=sum;
    pthread_exit(NULL);
}

int main(void){
    pthread_t pro1,pro2, con1, con2;            //pro1，pro2代表两个生产者，con1，con2代表两个消费者
    pthread_mutex_init(&the_mutex,0);           //初始化线程锁
    pthread_cond_init(&condc,0);                //初始化条件变量
    pthread_cond_init(&condp,0);
    pthread_create(&con1, 0, consumer1, 0);     //建立线程
    pthread_create(&pro1, 0, producer1, 0);
    pthread_create(&pro1, 0, producer2, 0);
    pthread_create(&con2, 0, consumer2, 0);
    pthread_join(con1, 0);                      //主进程只需要等待两个消费者结束即可
    pthread_join(con2, 0);
    pthread_cond_destroy(&condc);               //销毁条件变量
    pthread_cond_destroy(&condp);
    pthread_mutex_destroy(&the_mutex);          //销毁线程锁
    printf("\nsum=%lld\n",res);                  //输出最终值
    return 0;
}
