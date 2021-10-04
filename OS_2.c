// OS_2.c (Thread Programming)

// 1. 두개의 thread를 만들어서 한 개는 ‘producer()’로, 다른 한 개는 ‘consumer()’라고 명명하시오.
// 2. producer thread는 0~99 사이의 정수 중 하나를 random하게 생성하여 gNumber라는 이름의 global 변수에 저장하고 자신이 만든 random number를 화면(stdout)에 출력하시오.
// 3. consumer thread는 producer가 random number를 생성하여 gNumber라는 이름의 global 변수에 저장하면 즉시 이를 화면(stdout)에 출력한다.
// 4. producer와 consumer는 위의 2, 3 단계의 동작을 각각 100번 반복한 후에 자신이 그동안 화면에 출력한 gNumber의 합을 main thread에 보내고 종료한다.
// 5. main thread는 producer와 consumer가 종료될 때까지 기다렸다가 producer와 consumer가 자신에게 보내 온 gNumber의 합이 일치하면 ‘success’를 화면에 출력하고, 아니면 ‘fail’을 화면에 출력하고 종료한다.
//    위의 5단계에서 ‘fail’이 출력되면 왜 그런지 이유를 분석하시오.


#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

void* consumer(void* arg);
void* producer(void* arg);

int run_now = 1;
int gNumber;
int p_counter=0;
int c_counter=0;

int main() {
  int p_res, c_res;
  pthread_t p_thread;
  pthread_t c_thread;
  void* p_thread_result;
  void* c_thread_result;
  
  p_res = pthread_create(&p_thread, NULL, producer, NULL);
  c_res = pthread_create(&c_thread, NULL, consumer, NULL);
 
  p_res = pthread_join(p_thread, &p_thread_result);
  c_res = pthread_join(c_thread, &c_thread_result);

  if((int*)p_thread_result == (int*)c_thread_result){
    printf("SUCCESS\n");
  }
  else{
    printf("FAIL");
  }
  exit(EXIT_SUCCESS);
}

void* producer(void* arg) {
  void* p_sum=0;
  srand(time(NULL));
  while(p_counter<100) {
    if(run_now==1){
      
      gNumber=rand()%100;
      run_now=2;
      printf("%d ", gNumber);
      fflush(stdout);
      p_sum+=gNumber;
      p_counter++;
    }
  }

  pthread_exit((void*)p_sum);
}

void* consumer(void* arg) {
  void* c_sum=0;

  while(c_counter<100){
    if(run_now==2){
      printf("%d ", gNumber);
      fflush(stdout);
      run_now=1;
      c_sum+=gNumber;
      c_counter++;
    }
  }
  
  pthread_exit((void*)c_sum);
}
