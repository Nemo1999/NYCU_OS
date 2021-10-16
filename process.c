#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include <sys/time.h>
#include<errno.h>
//#define ARR_SIZE 1000000
//#define NUM_PROC 2

struct timeval t1, t2;


int main(int argc, char** argv){


    // read ARR_SIZE and target number from arguments
    char* p;
    errno = 0; // not 'int errno', because the '#include' already defined it
    long ARR_SIZE = strtol(argv[1], &p, 10);
    if (*p != '\0' || errno != 0) {printf("error parsing arguments\n");return 1;}
    int target = strtol(argv[2], &p, 10);
    if (*p != '\0' || errno != 0) {printf("error parsing arguments\n");return 1;}
    long NUM_PROC = strtol(argv[3], &p, 10);
    if (*p != '\0' || errno != 0) {printf("error parsing arguments\n");return 1;}

    srand(0); // fix seed for same result every time
    int* arr = malloc(sizeof(int)*ARR_SIZE);
    for(int i=0;i<ARR_SIZE;i++){
        arr[i] = rand() % (ARR_SIZE/5);
    }


    gettimeofday(&t1, NULL);
    for(int i=0;i< NUM_PROC;i++){
        int f = fork();
        if(f==-1){
            printf("unable to fork new process\n");
            return 1;
        }
        else if(f == 0){
            // child process
            int cnt = 0;
            int start = (ARR_SIZE/NUM_PROC) * i;
            int end = (i == NUM_PROC)? ARR_SIZE : (ARR_SIZE/NUM_PROC) * (i+1) ;
            for(int j=start;j<end;j++){
                if(arr[j] == target){
                    cnt++;
                }
            }
            exit(cnt);
        }
    }

    //parent process
    int cnt = 0;
    for(int i=0;i<NUM_PROC;i++){
        int wstatus;
        if(wait(&wstatus)==-1){
            printf("wait fail! \n");
            return 2;
        }
        if(WIFEXITED(wstatus)){
            cnt += WEXITSTATUS(wstatus);
        }
        else{
            printf("child process didn't terminate normally");
            return 3;
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
