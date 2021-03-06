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
   	n denotes capacity of buffer, np and nc denotes number of producer and consumer processes respectively,
   	cnt1 denotes count of each producer process and cnt2 denotes count of each consumer process.
   	t1 and t2 are delay times
*/
int* buffer;
int n,np,nc,cnt1,cnt2;
int in,out;
int count=0;
double t1,t2;
FILE* f=fopen("Mutex-Log.txt","w");
pthread_mutex_t lock;
double wait_cons=0;
double wait_prod=0;
void *producer(void* i)
{
	int id = (long)i;
	exponential_distribution<double> distribution1(1.0/t1);
	default_random_engine gen;
	for(int j=0;j<cnt1;j++)
	{
      struct timeval start,end;

		//Inserting element into buffer
      gettimeofday(&start,NULL);
		int item = rand()%100;
      int position;
      while(true)
      {
         pthread_mutex_lock(&lock);
         if(count < n)
         {
            buffer[in]=item;
            position=in+1;
            in=(in+1)%n;
            count=count+1;
            time_t t1 = time(NULL);   // get time now
            struct tm* cur = localtime(&t1);
            fprintf(f,"%dth item produced by thread %d at %.2d:%.2d:%.2d into buffer location %d.\n",j+1,id,cur->tm_hour,cur->tm_min,cur->tm_sec,position);
            gettimeofday(&end,NULL);
            wait_prod+=((end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) * 0.000001)); 
            pthread_mutex_unlock(&lock);
            break;
         }
         else
            pthread_mutex_unlock(&lock);
      }

      //Sleep
		double h1=distribution1(gen);
		usleep(h1*(1000000));
	}
}

void *consumer(void* i)
{
	int id = (long)i;
	exponential_distribution<double> distribution2(1.0/t2);
	default_random_engine gen;
	for(int j=0;j<cnt2;j++)
	{
      //Consuming an item
      int position;
      struct timeval start,end;
      gettimeofday(&start,NULL);
      while(true)
      {
         pthread_mutex_lock(&lock);
      	if(count>0)
         {
            int item = buffer[out];
            position=out+1;
         	out=(out+1)%n;
            count=count-1;
            time_t t1 = time(NULL);   // get time now
            struct tm* cur = localtime(&t1);
            fprintf(f,"%dth item consumed by thread %d at %.2d:%.2d:%.2d from buffer location %d.\n",j+1,id,cur->tm_hour,cur->tm_min,cur->tm_sec,position);
            gettimeofday(&end,NULL);
            wait_cons+=((end.tv_sec - start.tv_sec) + ((end.tv_usec - start.tv_usec) * 0.000001)); 
            pthread_mutex_unlock(&lock);
            break;
         }
         else
            pthread_mutex_unlock(&lock);
      }
		
		
      //Sleep
      double h2=distribution2(gen);
		usleep(h2*(1000000));
	}
}

int main()
{
   ifstream input;
   ofstream output;
  
  	/*taking input*/
   input.open("inp-params.txt");
   input>>n>>np>>nc>>cnt1>>cnt2>>t1>>t2;
   buffer =new int [n]; 
   input.close();

   /*initialising threads*/
   pthread_t prod[np];	//producer threads
   pthread_t cons[nc];	//consumer threads
   pthread_attr_t attr;
   pthread_attr_init(&attr);

    /*Initialising mutex lock*/
   pthread_mutex_init(&lock, NULL);

   in=0;out=0;
    /*Creating threads*/
   for(int i=0;i<max(np,nc);i++)
   { 
         if(i<np)
            pthread_create(&prod[i], &attr, producer,(void*)(intptr_t)(i+1));
         if(i<nc)
            pthread_create(&cons[i], &attr, consumer,(void*)(intptr_t)(i+1));
   }

   /*Joining Threads*/
   for(int i=0;i<np;i++)
   {
   		pthread_join(prod[i],NULL);
   }

   for(int i=0;i<nc;i++)
   {
   		pthread_join(cons[i],NULL);
   }
   delete[] buffer;
   pthread_mutex_destroy(&lock); 
   fclose(f);
   f=fopen("Averagetime-Mutex.txt","w");
   fprintf(f,"Average Waiting Time for producers in case of mutex is : %lf\n",(wait_prod/cnt1));
   fprintf(f,"Average Waiting Time for consumers in case of mutex is : %lf\n",(wait_cons/cnt2));
   fclose(f);
	return 0;
}