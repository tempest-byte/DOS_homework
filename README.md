README
===========================
仅包含编程作业，阅读作业未添加
****
## 目录
* [作业三](#作业三)
* [作业四](#作业四)
* [作业五](#作业五)
****
作业三
-----------
#### 要求
* 用fork创建进程。 用共享内存方式实现缓冲区。
* 有2个生产者，2个消费者。 
* 生产者将1至10000000的数据放入缓冲区，消费者从缓冲区中读取数据，并进行累加。 
* 最终输出2个消费者的累加之和。
#### 代码
* [使用SYSTEM V变量实现](https://github.com/Wanderingqiu/os_homework/blob/master/homework3_SYSTEMV.c)
* [使用POSIX变量实现](https://github.com/Wanderingqiu/os_homework/blob/master/homework3_POSIX.c)

作业四
-----------
#### 要求
* 用pthread多线程，condition variable（条件变量）解决如下问题：
* 有2个生产者，2个消费者。
* 生产者将1至10000000的数据放入缓冲区，消费者每次从缓冲区中读取至少两个数据，并进行累加。 
* 最终输出2个消费者的累加之和。
#### 代码
* [使用pthread实现](https://github.com/Wanderingqiu/os_homework/blob/master/homework4_pthread.c)

作业五
-----------
#### 要求
* 分别用三种方式实现下列代码：
```c
int i, sum;  
sum=0;  
for(i=0; i<10000000; i+=4) {  
sum ++;  
sum ++;  
sum ++;  
sum ++;  
}  
```
* 把i，sum放到寄存器中执行。每次累加时从寄存器读取数据。
* 把i，sum放到内存中执行，能够被cache。读写均在cache中完成。
* 把i，sum放到内存中执行，不能够被cache。读写均在DRAM中完成。
#### 代码
* [使用register实现](https://github.com/Wanderingqiu/os_homework/blob/master/homework5_register.c)
* [使用cache实现](https://github.com/Wanderingqiu/os_homework/blob/master/homework5_cache.c)
* [使用dram实现(mm_clflush)](https://github.com/Wanderingqiu/os_homework/blob/master/homework5_dram_clflush.c)
* [使用dram实现(movnti)](https://github.com/Wanderingqiu/os_homework/blob/master/homework5_dram_movnti.s)