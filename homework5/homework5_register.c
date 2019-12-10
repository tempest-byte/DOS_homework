#include <stdio.h>
//注意regiter只是建议分配寄存器，但不是强制。所以需要使用gcc
int main(){
	register int i,sum;
	sum=0;
	i=0;
	for(;i<10000000;i+=4){
		sum++;
		sum++;
		sum++;
		sum++;
	}
printf("%d",sum);
}
