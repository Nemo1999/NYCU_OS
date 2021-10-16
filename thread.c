#include<stdlib.h>
#include<stdio.h>
#include <sys/time.h>
#include <errno.h>

// #define ARR_SIZE 1000000
struct timeval t1, t2;

int main(int argc, char* argv[]){

    // read ARR_SIZE and target number from arguments
    char* p;
    errno = 0; // not 'int errno', because the '#include' already defined it

    long ARR_SIZE = strtol(argv[1], &p, 10);
    if (*p != '\0' || errno != 0) {printf("error parsing arguments\n");return 1;}
    int target = strtol(argv[2], &p, 10);
    if (*p != '\0' || errno != 0) {printf("error parsing arguments\n");return 1;}

    srand(0); // fix seed for same result every time
    int* arr = malloc(sizeof(int)*ARR_SIZE);
    if(arr==0){printf("malloc fail for arr\n");return 2;}
    for(int i=0;i<ARR_SIZE;i++){
        arr[i] = rand() % (ARR_SIZE/5);
    }

    gettimeofday(&t1, NULL);
    int cnt = 0;
    for(int i=0;i<ARR_SIZE;i++){
        if(arr[i] == target){
            cnt ++;
        }
    }

    gettimeofday(&t2, NULL);
    printf("The number %d appears %d times in the array\n", target, cnt);

    int macros = (t2.tv_usec > t1.tv_usec)  ? (t2.tv_usec - t1.tv_usec)
        : (1000000 - t1.tv_usec + t2.tv_usec);
    int seconds =  (t2.tv_usec > t1.tv_usec) ? (t2.tv_sec - t1.tv_sec)
        : (t2.tv_sec - 1 - t1.tv_sec);
    printf("total time spent = %d seconds and %d usecs\n",seconds, macros);

    return 0;
}
