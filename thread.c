#include<stdlib.h>
#include<stdio.h>
#include<sys/time.h>
#include<errno.h>
#include<pthread.h>
// #define ARR_SIZE 1000000

struct timeval t1, t2;

pthread_mutex_t cnt_mutex;

int* arr;
int cnt;
int target;

struct search_range{
    int start;
    int end;
};

void* routine(void* arg){
    int local_cnt = 0;
    int start = ((struct search_range*) arg) -> start;
    int end = ((struct search_range *) arg) -> end;

    for(int i=start;i<end;i++)
        if(arr[i]==target)
            local_cnt++;

    pthread_mutex_lock(&cnt_mutex);
    cnt+=local_cnt;
    pthread_mutex_unlock(&cnt_mutex);
}

int main(int argc, char* argv[]){

    // read ARR_SIZE and target number from arguments
    char* p;
    errno = 0; // not 'int errno', because the '#include' already defined it

    int ARR_SIZE = strtol(argv[1], &p, 10);
    if (*p != '\0' || errno != 0) {printf("error parsing arguments\n");return 1;}
    target = strtol(argv[2], &p, 10);
    if (*p != '\0' || errno != 0) {printf("error parsing arguments\n");return 1;}
    int NUM_THREAD = strtol(argv[3], &p, 10);
    if (*p != '\0' || errno != 0) {printf("error parsing arguments\n");return 1;}

    srand(0); // fix seed for same result every time
    // generate array data
    arr = malloc(sizeof(int)*ARR_SIZE);
    if(arr==0){printf("malloc fail for arr\n");return 2;}
    for(int i=0;i<ARR_SIZE;i++){
        arr[i] = rand() % (ARR_SIZE/5);
    }

    //start timer
    gettimeofday(&t1, NULL);
    //init mutex
    pthread_mutex_init(&cnt_mutex, NULL);
    // create threads
    pthread_t* th = malloc(sizeof(pthread_t)*NUM_THREAD);
    struct search_range* ranges = malloc(sizeof(struct search_range)*NUM_THREAD);
    for(int i=0;i<NUM_THREAD;i++){
        struct search_range* range = &ranges[i];
        range->start = (ARR_SIZE/NUM_THREAD) * i;
        range->end = (i== NUM_THREAD-1)? ARR_SIZE : (ARR_SIZE/NUM_THREAD) * (i+1);
        if(pthread_create(&th[i], NULL, &routine, range)){
            printf("error creating thread\n");return 2;
        }
    }
    //join threads
    for(int i=0;i<NUM_THREAD;i++){
        if(pthread_join(th[i],NULL)!=0){
            printf("fail to join thread\n");return 3;
        }
    }
    //destroy mutex
    pthread_mutex_destroy(&cnt_mutex);
    //end timer
    gettimeofday(&t2, NULL);
    printf("The number %d appears %d times in the array\n", target, cnt);

    int macros = (t2.tv_usec > t1.tv_usec)  ? (t2.tv_usec - t1.tv_usec)
        : (1000000 - t1.tv_usec + t2.tv_usec);
    int seconds =  (t2.tv_usec > t1.tv_usec) ? (t2.tv_sec - t1.tv_sec)
        : (t2.tv_sec - 1 - t1.tv_sec);
    printf("total time spent = %d seconds and %d usecs\n",seconds, macros);
    //free memory
    free(th);
    free(ranges);
    free(arr);

    return 0;
}
