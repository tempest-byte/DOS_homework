README
===========================
仅包含编程作业，阅读作业未添加
****
## 目录
* [作业三](#作业三)
* [作业四](#作业四)
* [作业五](#作业五)
* [作业六](#作业六)
* [作业七](#作业七)
****
作业三
-----------
#### 要求
* 用fork创建进程。 用共享内存方式实现缓冲区。
* 有2个生产者，2个消费者。 
* 生产者将1至10000000的数据放入缓冲区，消费者从缓冲区中读取数据，并进行累加。 
* 最终输出2个消费者的累加之和。
#### 代码
* [使用SYSTEM V变量实现](https://github.com/Wanderingqiu/os_homework/blob/master/homework3/homework3_SYSTEMV.c)
* [使用POSIX变量实现](https://github.com/Wanderingqiu/os_homework/blob/master/homework3/homework3_POSIX.c)

作业四
-----------
#### 要求
* 用pthread多线程，condition variable（条件变量）解决如下问题：
* 有2个生产者，2个消费者。
* 生产者将1至10000000的数据放入缓冲区，消费者每次从缓冲区中读取至少两个数据，并进行累加。 
* 最终输出2个消费者的累加之和。
#### 代码
* [使用pthread实现](https://github.com/Wanderingqiu/os_homework/blob/master/homework4/homework4_pthread.c)

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
* [使用register实现](https://github.com/Wanderingqiu/os_homework/blob/master/homework5/homework5_register.c)
* [使用cache实现](https://github.com/Wanderingqiu/os_homework/blob/master/homework5/homework5_cache.c)
* [使用dram实现(mm_clflush)](https://github.com/Wanderingqiu/os_homework/blob/master/homework5/homework5_dram_clflush.c)
* [使用dram实现(movnti)](https://github.com/Wanderingqiu/os_homework/blob/master/homework5/homework5_dram_movnti.s)

作业六
-----------
#### 要求
* 设计一个分布式处理系统。一个节点（进程）作为Master,负责节点注册，任务分配；其它多个节点（进程）作为worker，执行客户端发送的请求。系统执行流程为：
* Master首先启动
* Worker节点启动后向Master进行“注册”；Master维护所有已注册节点的信息（比如IP地址和TCP/UDP端口，服务类型等）；
* 客户端提交任务前首先向Master发送“查询”消息，询问将任务提交给哪个worker
* Master根据一定策略（如轮询）从所有Worker中选择一个worker，比如A，将其信息发送给Client
* Worker A完成任务后将结果返回给客户端
* 计算任务为：两个数的+，-，*，/。数据类型支持32位整数、64位整数、浮点数
* 要求消息编码格式：google protocol buffer
* Worker端要支持多个客户端请求的并发处理
* 测试要求：
    * Worker数量不少于3个
    * 客户端数量不少于3个
    * 单个客户端发送请求数要大于100万个，或者测试时间不少于10分钟

#### 代码
* [使用golang实现](https://github.com/Wanderingqiu/golang/tree/master/socket%20RPC)

作业七
-----------
#### 要求
* 使用spark编写程序
#### 代码
* [spark的word统计](https://github.com/Wanderingqiu/DOS_homework/blob/master/homework7/spark_wordcount.py)
* [spark的机器学习](https://github.com/Wanderingqiu/DOS_homework/blob/master/homework7/spark_ml.py)