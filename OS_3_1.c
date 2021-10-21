#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

// no mutex
int total_balance = 1000000;
int run_now = 0;

void *borrow(void *);
void *repay(void *);

void *borrow(void *arg) {
    for(int i=0; i<10; i++) {
      if(run_now==0) {
        printf("Borrow is started\n");
        int amount = 0;
        int borrowMoney = rand()%10000;
        amount = borrowMoney;
        total_balance -= amount;
        printf("Success Borrowing money: %d won\n", amount);
        printf("Borrowing is finished: Total_Balance: %d\n", total_balance);
        run_now++;
      }
      else i--;
      usleep(rand()%1000000);
    }
    
}

void *repay(void *arg) {
    for(int i=0; i<10; i++) {
      if(run_now==1) {
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
      run_now--;
      }
      else i--;
      usleep(rand()%1000000);
    }
  
}


int main() {
    srand(time(NULL));
    pthread_t t1, t2;
    
    pthread_create(&t1, NULL, borrow, NULL);
    pthread_create(&t2, NULL, repay, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Final Woori-Bank Balance: %d won\n", total_balance); 
    return 0;
} 
