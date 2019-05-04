#include <iostream>
#include <fstream>
#include <pthread.h>
#include <random>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <cstdlib>
#include <sys/time.h> 
#define philnum 20
using namespace std;

enum { THINKING, HUNGRY, EATING } state[philnum] ;
pthread_mutex_t mutex;
pthread_cond_t self[philnum];

/*n stores the number of philosophers, h is number of requests made by a philosopher and
t1 and t2 are time delays in critical and remainder section*/

int n,h;
double t1,t2;
FILE* f=fopen("dphil-log.txt","w");
double average_wait=0;  //average time taken by a philosopher to enter the CS
double worst_time=0;    // worst case wait time to enter the CS

class myMonitor{

   public : 
      /*Constructor initializing the states and conditional variables*/  
      myMonitor() 
      {
         for(int i = 0; i < philnum; i++)
            {
               state[i] = THINKING;
               pthread_cond_init (&self[i],NULL);
            }
         pthread_mutex_init(&mutex,NULL);
      }

      /*Destructor destroying conditional variables and mutex lock*/  
      ~myMonitor()
      {
         for(int i = 0; i < philnum; i++)
            {
               pthread_cond_destroy(&self[i]);
            }
         pthread_mutex_destroy(&mutex);
      }

      /*Pickup function is called when philosopher tries to obtain a chopstick (request to CS) */
      void pickup(int i) 
      {
         pthread_mutex_lock(&mutex);
         state[i] = HUNGRY;
         test(i);
         if (state[i] != EATING)
            pthread_cond_wait(&self[i], &mutex);
         pthread_mutex_unlock(&mutex);
      }

      /*Putdown function is called when philosopher finishes eating (exit from CS) */
      void putdown(int i) 
      {
         pthread_mutex_lock(&mutex);
         state[i] = THINKING;
         test((i - 1 + n) % n);
         test((i + 1) % n);
         pthread_mutex_unlock(&mutex);
      }

      /*Test function is used to check whether neighbours are eating or not */
      void test(int i) {
         if ((state[(i - 1 + n) % n] != EATING) && (state[i] == HUNGRY) && (state[(i + 1) % n] != EATING)) 
         {
            state[i] = EATING;
            pthread_cond_signal(&self[i]);
         }
      }

};
myMonitor diningPhilosopher;

void *writer(void* i)
{
   int id = (long)i;
   exponential_distribution<double> distribution1(1.0/t1);
   exponential_distribution<double> distribution2(1.0/t2);
   default_random_engine gen;

   for(int j=0;j<h;j++)
   {
      /*Request to enter the CS*/
      struct timeval start,end;
      gettimeofday(&start,NULL);
      time_t t1 = time(NULL);   // Request time
      struct tm* cur = localtime(&t1);
      fprintf(f,"%dth eat request by Philosopher Thread %d at %.2d:%.2d:%.2d .\n",j+1,id,cur->tm_hour,cur->tm_min,cur->tm_sec); 

      diningPhilosopher.pickup(id-1);

      /*Entered the CS*/
      time_t t2 = time(NULL);   // Entry time
      struct tm* cur2 = localtime(&t2);
      fprintf(f,"%dth CS entry by Philosopher Thread %d at %.2d:%.2d:%.2d .\n",j+1,id,cur2->tm_hour,cur2->tm_min,cur2->tm_sec); 
      gettimeofday(&end,NULL);
      double diff=((end.tv_sec - start.tv_sec)*1000 + ((end.tv_usec - start.tv_usec) * 0.001));
      average_wait+=diff;
      worst_time=max(worst_time,diff);

      //Sleep
      double h1=distribution1(gen);
      usleep(h1*(1000));

      diningPhilosopher.putdown(id-1);

      /*Remainder section*/
      time_t t3 = time(NULL);   // Exit time 
      struct tm* cur3 = localtime(&t3);
      fprintf(f,"%dth CS exit by Philosopher Thread %d at %.2d:%.2d:%.2d .\n",j+1,id,cur3->tm_hour,cur3->tm_min,cur3->tm_sec);

      //sleep
      double h2=distribution2(gen);
      usleep(h2*(1000));

   }
   pthread_exit(0);
}

int main()
{
   ifstream input;
   ofstream output;
  
   /*taking input*/
   input.open("inp-params.txt");
   input>>n>>h>>t1>>t2;
   input.close();

   /*initialising threads*/
   pthread_t phil[n];  //philosopher threads
   pthread_attr_t attr;
   pthread_attr_init(&attr);

   /*Creating threads*/
   for(int i=0;i<n;i++)
   { 
      pthread_create(&phil[i], &attr, writer,(void*)(intptr_t)(i+1));
   }

   /*Joining Threads*/
   for(int i=0;i<n;i++)
   {
         pthread_join(phil[i],NULL);
   }

   fclose(f);
   f=fopen("Times.txt","w");
   fprintf(f,"Average Waiting Time for philosophers is : %lf\n",(average_wait/n));
   fprintf(f,"Worst Case Waiting Time for philosophers is : %lf\n",worst_time);
   fclose(f);
   return 0;
}