#include <iostream>
#include <fstream>
#include <pthread.h>
#include <random>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <cstdlib>
#include <sys/time.h> 

using namespace std;

/*
      nr and nw denotes number of reader and writer threads respectively,
      kr denotes count of each reader thread is repeated and kw denotes count of each writer thread is repeated.
      t1 and t2 are delay times
*/
int nr,nw,kr,kw;
double t1,t2;
FILE* f=fopen("FairRW-Log.txt","w");
sem_t in,queue,mutex;
int readCount=0;
double wait_writer=0;
double wait_reader=0;
double worst_reader=0;
double worst_writer=0;

void *reader(void* i)
{
   int id = (long)i;
   exponential_distribution<double> distribution1(1.0/t1);
   exponential_distribution<double> distribution2(1.0/t2);
   default_random_engine gen;

   for(int j=0;j<kr;j++)
   {
      /*Request to enter the CS*/
      struct timeval start,end;
      gettimeofday(&start,NULL);
      time_t t1 = time(NULL);   // Request time
      struct tm* cur = localtime(&t1);
      fprintf(f,"%dth CS request by Reader Thread %d at %.2d:%.2d:%.2d .\n",j+1,id,cur->tm_hour,cur->tm_min,cur->tm_sec); 

      sem_wait(&queue);           
      sem_wait(&mutex);       
      if (readCount == 0)         
         sem_wait(&in);  
      readCount++;              
      sem_post(&mutex);           
      sem_post(&queue);      

      /*Entered the CS*/
      time_t t2 = time(NULL);   // Entry time 
      struct tm* cur2 = localtime(&t2);
      fprintf(f,"%dth CS entry by Reader Thread %d at %.2d:%.2d:%.2d .\n",j+1,id,cur2->tm_hour,cur2->tm_min,cur2->tm_sec); 
      gettimeofday(&end,NULL);
      double diff=((end.tv_sec - start.tv_sec)*1000 + ((end.tv_usec - start.tv_usec) * 0.001));
      wait_reader+=diff;
      worst_reader=max(worst_reader,diff);

      //Sleep
      double h1=distribution1(gen);
      usleep(h1*(1000));

      sem_wait(&mutex);       
      readCount--;                
      if(readCount == 0)         
        sem_post(&in);     
      sem_post(&mutex); 

      /*Remainder section*/
      time_t t3 = time(NULL);   // Exit time
      struct tm* cur3 = localtime(&t3);
      fprintf(f,"%dth CS exit by Reader Thread %d at %.2d:%.2d:%.2d .\n",j+1,id,cur3->tm_hour,cur3->tm_min,cur3->tm_sec);

      double h2=distribution2(gen);
      usleep(h2*(1000));
   }
}
void *writer(void* i)
{
   int id = (long)i;
   exponential_distribution<double> distribution1(1.0/t1);
   exponential_distribution<double> distribution2(1.0/t2);
   default_random_engine gen;

   for(int j=0;j<kw;j++)
   {
      /*Request to enter the CS*/
      struct timeval start,end;
      gettimeofday(&start,NULL);
      time_t t1 = time(NULL);   // Request time
      struct tm* cur = localtime(&t1);
      fprintf(f,"%dth CS request by Writer Thread %d at %.2d:%.2d:%.2d .\n",j+1,id,cur->tm_hour,cur->tm_min,cur->tm_sec); 

      sem_wait(&queue);
      sem_wait(&in);

      /*Entered the CS*/
      time_t t2 = time(NULL);   // Entry time
      struct tm* cur2 = localtime(&t2);
      fprintf(f,"%dth CS entry by Writer Thread %d at %.2d:%.2d:%.2d .\n",j+1,id,cur2->tm_hour,cur2->tm_min,cur2->tm_sec); 
      gettimeofday(&end,NULL);
      double diff=((end.tv_sec - start.tv_sec)*1000 + ((end.tv_usec - start.tv_usec) * 0.001));
      wait_writer+=diff;
      worst_writer=max(worst_writer,diff);

      //Sleep
      double h1=distribution1(gen);
      usleep(h1*(1000));

      sem_post(&in);
      sem_post(&queue);
      /*Remainder section*/
      time_t t3 = time(NULL);   // Exit time 
      struct tm* cur3 = localtime(&t3);
      fprintf(f,"%dth CS exit by Writer Thread %d at %.2d:%.2d:%.2d .\n",j+1,id,cur3->tm_hour,cur3->tm_min,cur3->tm_sec);

      double h2=distribution2(gen);
      usleep(h2*(1000));

   }
}

int main()
{
   ifstream input;
   ofstream output;
  
   /*taking input*/
   input.open("inp-params.txt");
   input>>nr>>nw>>kr>>kw>>t1>>t2;
   input.close();

   /*initialising threads*/
   pthread_t read[nr];  //reader threads
   pthread_t write[nw];  //writer threads
   pthread_attr_t attr;
   pthread_attr_init(&attr);

    /*Initialising semapores*/
   sem_init(&in, 0, 1); 
   sem_init(&queue, 0, 1);
   sem_init(&mutex,0,1);

   /*Creating threads*/
   for(int i=0;i<max(nr,nw);i++)
   { 
         if(i<nr)
            pthread_create(&read[i], &attr, reader,(void*)(intptr_t)(i+1));
         if(i<nw)
            pthread_create(&write[i], &attr, writer,(void*)(intptr_t)(i+1));
   }

   /*Joining Threads*/
   for(int i=0;i<nr;i++)
   {
         pthread_join(read[i],NULL);
   }

   for(int i=0;i<nw;i++)
   {
         pthread_join(write[i],NULL);
   }
   sem_destroy(&in);
   sem_destroy(&queue);
   sem_destroy(&mutex);
   fclose(f);
   f=fopen("Average_time-FairRW.txt","w");
   fprintf(f,"Average Waiting Time for readers is : %lf\n",(wait_reader/kr));
   fprintf(f,"Average Waiting Time for writers is : %lf\n",(wait_writer/kw));
   fprintf(f,"Worst Case Waiting Time for readers is : %lf\n",worst_reader);
   fprintf(f,"Worst Case Waiting Time for writers is : %lf\n",worst_writer);
   fclose(f);
   return 0;
}