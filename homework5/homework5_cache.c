#include <stdio.h>
//和register不同点只在于没有register修饰符，不过还是需要查看汇编文件看是否优化成寄存器中运行。
int main(){
	int i,sum;
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
