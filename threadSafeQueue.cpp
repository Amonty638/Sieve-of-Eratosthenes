#include <queue>
#include <iostream>
#include <semaphore.h>
#include <threadSafeQueue.h>
using namespace std;

class threadSafeQueue{

private:

  queue<int> q;
  sem_t mutex;
  
  /*
  int size();
  bool empty();
  int front();
  void push(int x);
  void  pop();
  int back();
  */

public: 


  threadSafeQueue(){

    sem_init(&mutex,0,1);

  }
  

  int size(){
    sem_wait(&mutex);
    int s = q.size();
    sem_post(&mutex);
    
    return s;
    
    
  }
  
  bool empty(){
    
    sem_wait(&mutex);
    bool empty = (q.size()==0);
    sem_post(&mutex);
    
    return empty;
    
  }
  
  int front(){
    
    sem_wait(&mutex);
    int f = q.front();
    sem_post(&mutex);
    
    return f;
    
  }
  
  void push(int x){
    sem_wait(&mutex);
    q.push(x);
    sem_post(&mutex);
  }
  
  void pop(){
    
    sem_wait(&mutex);
    q.pop();
    sem_post(&mutex);
    
    
  }
  
  int back(){
    
    sem_wait(&mutex);
    int b = q.back();
    sem_post(&mutex);
    
    
    return b;
    
  }
  
};
