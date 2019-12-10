#include <fcntl.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <xmmintrin.h>
//每次调用前都清除变量所在的cache_line
int main() {
    int i, sum;
    sum = 0;
    i = 0;
    for (; i < 10000000; i += 4) {
        _mm_clflush(&sum);
        sum++;
        _mm_clflush(&sum);
        sum++;
        _mm_clflush(&sum);
        sum++;
        _mm_clflush(&sum);
        sum++;
        _mm_clflush(&i);
    }
    printf("%d", sum);
}
