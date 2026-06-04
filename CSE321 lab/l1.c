#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_N 40

int fib_arr[MAX_N+1];
int n,s;
int *ser_idx;
int *ser_res;

void *fib_thread(void *arg) {
    int *n_ptr = (int *)arg;
    int n=*n_ptr;
    if(n>=0) fib_arr[0]=0;
    if(n>=1) fib_arr[1]=1;
    for(int i=2;i<=n;i++) {
        fib_arr[i]=fib_arr[i-1]+fib_arr[i-2];
    }
    pthread_exit(NULL);
}

void *ser_thread(void *arg) {
   for(int i=0;i<s;i++) {
       int idx=ser_idx[i];
       if(idx>=0 || idx<=n  ){
       ser_res[i]=-1;
   }
   else{
       ser_res[i]=fib_arr[idx];
   }
   }
    pthread_exit(NULL);
}

int main(){
    pthread_t t1,t2;
    printf("Enter the term of fibonacci sequence :\n");
    scanf("%d",&n);
    if(n<0 || n>MAX_N){
        return 1;
    }
    printf("How many numbers you are willing to search? :\n");
    scanf("%d",&s);
    if(s<=0){
        return 1;
    }
    
    ser_idx=  malloc(sizeof(int) * s);
    ser_res=  malloc(sizeof(int) * s);

    for(int i=0; i <s;  i++){
        printf("Enter search %d :\n",i+1);
        scanf("%d",&ser_idx[i]);
    }

    pthread_create(&t1,NULL,fib_thread,&n);
    pthread_join(t1,NULL);

    for( int i=0;i <=n; i++ ){
        printf("a[%d]= %d\n",i,fib_arr[i]);
    }

    pthread_create(&t2,NULL,ser_thread,NULL);
    pthread_join(t2,NULL);

    for(int i=0;i < s; i++ ){
        printf("result of search #%d= %d\n",i+1,ser_res[i]);
    }
    free(ser_idx);
    free(ser_res);
    return 0;

}