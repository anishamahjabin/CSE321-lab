#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t table_lock;

sem_t s_makerA, s_makerB, s_makerC;     
sem_t s_supplier;

int fixed_ing1[4]={0,1,0,0};
int fixed_ing2[4]={1,2,2,1};  

int N;

//int makerId[3]={1,2,3};

void *supplierFunc(void *arg) {
    int N = *((int *)arg);
    char *ing[] = {"Bread", "cheese", "Lettuce"};

    for (int i = 0; i < N; i++) {
        sem_wait(&s_supplier);
        pthread_mutex_lock(&table_lock);
        
        int first_ing= fixed_ing1[i];
        int second_ing= fixed_ing2[i];

        printf("Supplier places: %s and %s\n", ing[first_ing], ing[second_ing]);

        if(first_ing!=0 && second_ing!=0) sem_post(&s_makerA);
        else if(first_ing!=1 && second_ing!=1) sem_post(&s_makerB);
        else sem_post(&s_makerC);

        pthread_mutex_unlock(&table_lock);
        
    }
    return NULL;
}

void *maker_A(void *arg) {
    while (1) {
        sem_wait(&s_makerA);
        pthread_mutex_lock(&table_lock);
        printf("Maker A picks up cheese and Lettuce\n");
        printf("Maker A is making the sandwich...\n");
        printf("Maker A finished making the sandwich and eats it\n");
        printf("Maker A signals Supplier\n");
        sem_post(&s_supplier);
        pthread_mutex_unlock(&table_lock);
        
    }
    
}

void *maker_B(void *arg) {
    while (1) {
        sem_wait(&s_makerB);
        pthread_mutex_lock(&table_lock);
        printf("Maker B picks up Bread and Lettuce\n");
        printf("Maker B is making the sandwich...\n");
        printf("Maker B finished making the sandwich and eats it\n");
        printf("Maker B signals Supplier\n");
        sem_post(&s_supplier);
        pthread_mutex_unlock(&table_lock);
        
    }
    
}

void *maker_C(void *arg) {
    while (1) {
        sem_wait(&s_makerC);
        pthread_mutex_lock(&table_lock);
        printf("Maker C picks up Bread and cheese\n");
        printf("Maker C is making the sandwich...\n");
        printf("Maker C finished making the sandwich and eats it\n");
        printf("Maker C signals Supplier\n");
        sem_post(&s_supplier);
        pthread_mutex_unlock(&table_lock);
        
    }
    
}

int main() {
    scanf("%d", &N);
    
    pthread_mutex_init(&table_lock, NULL);
    
    sem_init(&s_supplier, 0, 1); 
    sem_init(&s_makerA, 0, 0);
    sem_init(&s_makerB, 0, 0);
    sem_init(&s_makerC, 0, 0);

    
    pthread_t sup, A, B, C;
    pthread_create(&sup, NULL, supplierFunc, &N);
    pthread_create(&A, NULL, maker_A, NULL);
    pthread_create(&B, NULL, maker_B, NULL);
    pthread_create(&C, NULL, maker_C, NULL);

    pthread_join(sup, NULL);

    pthread_cancel(A);
    pthread_cancel(B);
    pthread_cancel(C);

    pthread_mutex_destroy(&table_lock);
    sem_destroy(&s_supplier);
    sem_destroy(&s_makerA);
    sem_destroy(&s_makerB);
    sem_destroy(&s_makerC);
    return 0;
}
