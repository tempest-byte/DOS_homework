#include <stdio.h>
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
