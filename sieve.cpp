//Alexander Montano
#include <string>
#include <iostream>
#include <sieve.h>
#include <array>
//queue I'm using
#include <queue>
#include <threadSafeQueue.cpp>
//threads
#include <pthread.h>

//semaphore
#include <semaphore.h>
using namespace std;

struct ThreadArgs{

  
  //queue<int> q; //queue 
  
  threadSafeQueue* tsq = new threadSafeQueue();
  
  int num; // this number will represent the first prime we encounter
  int count = 0;//this is going to keep track of how many primes we've see...once we get to their choice we stop...
  sem_t mutex;
};


//methods
void printq(queue<int> q);
void* filter(void* param);
void printPrimes();

array<int,1000> primes={0};//array to hold up to 1000 prime numbers

int choice; //user input of how many primes they want printed

int main(void){

  //how to access struct
  /*
  struct blah;

  blah.q.pop();
  */

  
  ThreadArgs thread;
  pthread_t tid0;

  //thread.q = new queue<int>;
  thread.num=2; //initialize first prime we encountered
  thread.count++;
  primes[0] = thread.num;//first prime = 2

  
  
  //getting user's number
  cout<<"Enter a number n, where that is how many primes you want to find"<<endl;
  cin>>choice;
  cout<<endl;
  pthread_create(&tid0,NULL,filter,&thread);
  
  //pushes numbers on to queue
  int i =2;
  while(true){//in queue class, I only hold x numbers. Change queue1.cpp to account for more
    //cout<< "enqueuing " << i<<endl; //works
    
    /*//debugging
    if(i%1000==0){
      cout<<".";
      cout.flush();
    }
    */
    
    if(primes.size()==choice){//if the amount of prime numbers in the primes array matches how many primes the user wants then we are done.
      exit(0);
    }
    else{
      thread.tsq->push(i);//if my primes array is not filled with how many numbers the user wants then put the numbers we get onto the queue in the struct
      i++;
    }
  }

  cout<<endl;
  
  // printq(*thread.q);

  
  pthread_join(tid0,NULL); //stops main from ending until thread exits
  
  return 117;
}



//prints queue
void printq(queue<int> myQ){
  
  while(!myQ.empty()){
    cout<< "dequeuing"  << myQ.front() << endl;
    myQ.pop();
  }
}


//worker thread
void* filter(void* arg){
  
  //cout<<"In worker thread!"<<endl;

  //used to take care of race conditions in the thread method
  //sem_t mutex;
  //sem_init(&mutex,0,1);

  //sem_t empty;

  //need another thread for the next prime found
  pthread_t tid0;
  
  bool first = true; //this is going to be the first time we encounter a number that is not divisable by what we send in.
    
  struct ThreadArgs *t = (struct ThreadArgs *)arg;//dereference first struct passed in

  //if we've calculated the proper amount of primes
  if(t->count==choice){
    printPrimes();
    exit(EXIT_SUCCESS);
    pthread_exit(0);
  }
  
  //sem_init(&t->mutex,0,1);//semaphore from the first struct

  
  //sem_init(&empty,0,t->q.size()); //semaphore used to wait if there is nothing on queue
  
  
  //cout<<"thread id: " <<t->num<<endl;//works

  //cout<<"front in filter: " <<q.front()<<endl; //works

  //new thread args
  ThreadArgs t2;//new struct to send to next thread

  
  //s.q = &q1;

  //printq(*t->q); //works
  
  while(true){
    //cout<<"in while"<<endl;

    //cout<<"in thread: "<<t->num<<endl;//works

    if(t->tsq->empty()){
      continue;
    }

    
    

    
    if(t->tsq->front()%t->num==0){//if front in first queue is divisible by number put in
      //sem_wait(&empty);
      t->tsq->pop();//get rid of it
      //sem_post(&empty);
    }
    

        //the first time we enter a thread, the thread id and the first thing in the queue will be the same. this will remove it, so the next else if gets the proper number that isnt divisible by the thread id. without this, for example, the twos thread would never actually pop the number 2 and you would just keep calling this thread with 2 as your id   
    else if(t->num==t->tsq->front()){
      //sem_wait(&empty);
      t->tsq->pop();
      //sem_post(&empty);
    }



    else if(first==true && (!t->tsq->front()%t->num==0)){//if it is not divisible by the number sent in and this is the first number we've seen like this 
      first=false;


      //for loop adds that first number to the prime list
      //protect this area, as it could be a race condition
      //sem_wait(&t->mutex);   
      for(int i = 0;i<choice;i++){
	if(primes[i]!=0){//if there is already a prime in that array spot
	  continue;
	}
	else{//no number in array spot
	  primes[i] = t->tsq->front();
	  t->count++;
	  break;//break here beacause we only want to add the number once
	}
      }

      //sem_post(&t->mutex);

      //t2.q = new queue<int>;


      //sem_wait(&mutex);
      t2.num=t->tsq->front();//new prime for new thread is the same number we just put onto the primes array


      t2.count = t->count;//next struct has the count of the old one, this allows for incrementing 
      
      t2.tsq->push(t->tsq->front());//put number on new queue

      t->tsq->pop(); //pop the number to get to the next num in queue
      //sem_post(&mutex);
      
      pthread_create(&tid0,NULL,filter,&t2);
    }

    else{//this else if for the rest of the numbers that aren't divisible...just not the first one
      //sem_wait(&mutex);
      t2.tsq->push(t->tsq->front());
      t->tsq->pop();//pop the number to get to the next num in queue
      //sem_post(&mutex);
    }





    
  }//end while
  
  //pthread_join(tid0,NULL);
  pthread_exit(0);

}//end filter



void printPrimes(){

  for(int i=0; i<choice;i++){
    cout<<primes[i]<<endl; 
  }
}
