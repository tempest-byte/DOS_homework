#include <xmmintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <math.h>

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
