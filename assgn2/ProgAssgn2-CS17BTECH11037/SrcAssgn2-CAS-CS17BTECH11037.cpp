#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <time.h>
#include <random>
#include <stdio.h>
#include <unistd.h>
#include <atomic>
#include <fstream>

using namespace std;

atomic<int> lock(0);

int n,k;
double t1,t2;
double wait_time =0;
FILE* f=fopen("CAS-Log.txt","w");
FILE* avg=fopen("Averagetime-CAS.txt","w");
double max_wait_time=0;

void *testCS(void* i)
{
	int id = (long)i;
	default_random_engine gen;
	exponential_distribution<double> distribution1(1.0/t1);
	exponential_distribution<double> distribution2(1.0/t2);
	gen.seed(4);
	for(int j=0;j<k;j++)
	{
		time_t t1 = time(NULL);   // get time now
		struct tm* cur = localtime(&t1);
		fprintf(f,"%dth CS Request at %.2d:%.2d:%.2d by thread %d.\n",j+1,cur->tm_hour,cur->tm_min,cur->tm_sec,id);

		while(true)
		{
			int x=0,y=1;
			if(lock.compare_exchange_strong(x,y))
				break;
		}

		time_t t2 = time(NULL);
		struct tm* start = localtime(&t2);
		max_wait_time=max(max_wait_time,difftime(t2,t1));
		wait_time += difftime(t2,t1);
		fprintf(f,"%dth CS Entry at %.2d:%.2d:%.2d by thread %d.\n",j+1,start->tm_hour,start->tm_min,start->tm_sec,id);

		sleep(distribution1(gen));

		time_t t3 = time(NULL);
		struct tm* end = localtime(&t3);
		fprintf(f,"%dth CS Exit at %.2d:%.2d:%.2d by thread %d.\n",j+1,end->tm_hour,end->tm_min,end->tm_sec,id);
		lock=0;
		sleep(distribution2(gen));
	}
	pthread_exit(0);
}

int main()
{

   ifstream input;
   input.open("inp-params.txt");
   input>>n>>k>>t1>>t2;

   pthread_t threads[n];
   pthread_attr_t attr;
   pthread_attr_init(&attr);

   //int *arg1 = (int *)malloc(sizeof(*arg1));
   for(int i=0;i<n;i++)
   { 
   		//*arg1=i;
   		pthread_create(&threads[i], &attr, testCS,(void*)(intptr_t)(i+1));
   }
   for (int i = 0; i < n; i++) 
   	{
   		pthread_join(threads[i], NULL);
   	}
   	fprintf(avg,"Average Waiting time %lf.\n",(wait_time/(n*k)));
   	fprintf(avg,"Worst Waiting time for process to enter CS %lf.\n",max_wait_time);
   	fclose(avg);
   	fclose(f);
   	input.close();
}