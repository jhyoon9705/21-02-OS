#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>

// LINUX semaphore
int total_balance = 1000000;
sem_t bin_sem;


void borrow(void) {
  
    printf("Borrow is started\n");
    int amount = 0;
   
    int borrowMoney = rand()%10000;
    amount = borrowMoney;
    total_balance -= amount;
    printf("Success Borrowing money: %d won\n", amount);
    printf("Borrowing is finished: Total_Balance: %d\n", total_balance);
    usleep(rand()%1000000);
   
    
}

void repay(void) {
 
    printf("Repay is started\n");
    int needToRepay = 0;
    
    needToRepay = 1000000 - total_balance;
    if(needToRepay !=0) {
        total_balance += needToRepay;
        printf("Success Repaying money: %d won\n", needToRepay);
    }
    else {
        printf("No money to repay!\n");
    }
    printf("Repaying is finished: Total_Balance: %d\n", total_balance);
    usleep(rand()%1000000);

  
}

void noncritical_region(){
    usleep(rand()%1000000);
}

static void* f1(void* p) {
    for(int i=0; i<10; i++) {
        puts("Borrowing wait for Repaying");
        sem_wait(&bin_sem);
        printf("Borrow start its critical section\n");
        borrow();
        printf("Borrow end its critical section\n");
        sem_post(&bin_sem);
        noncritical_region();
    }
    return NULL;
}

static void* f2(void* p) {
    for(int i=0; i<10; i++) {
        puts("Repaying wait for Borrowing");
        sem_wait(&bin_sem);
        printf("Repay start its critical section\n");
        repay();
        printf("Repay end its critical section\n");
        sem_post(&bin_sem);
        noncritical_region();
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    int rc;
    pthread_t t1, t2;
    
    sem_init(&bin_sem, 0, 1);    

    rc = pthread_create(&t1, NULL, f1, "f1");

    if(rc != 0) {
        fprintf(stderr, "pthread borrow failed\n");
        return EXIT_FAILURE;
    }
    rc=pthread_create(&t2, NULL, f2, "f2");

    if(rc != 0) {
        fprintf(stderr, "pthread repay failed\n");
        return EXIT_FAILURE;
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    
    sem_destroy(&bin_sem);
    
    puts("All threads finished.");
    printf("Final Woori-bank balance: %d won\n", total_balance);
    return 0;
} 
