#include<bits/stdc++.h>
using namespace std;
class process{

	public :
		string pid; //process id
		int t;      //processing time
		int no;     //number associated with each process i.e. for P1 it will be 1
		int period;
		int k;      //number of time process repeats
		int remaining_time;
		int instance;
		int s_time; // start time of process
		int arrive_time;// arrival time of process in ready queue
};
bool cmp(const pair<int,process> &a,const pair<int,process> &b) // comparator function which compares arrival time if they are equal then period is considered
{
	if(a.first!=b.first)
		return a.first<b.first;
	else
		return (a.second).period<(b.second).period;
}
struct compar //Sorting function comparator for the queue
{
    inline bool operator() (const process &process1, const process &process2) //if deadline is same then we take process with smaller process number otherwise one with earlier deadline
    {
    	if((process1.instance*process1.period) == (process2.instance*process2.period))
        	return process1.no < process2.no;
        return ((process1.instance*process1.period) < (process2.instance*process2.period));
    }
};
int main()
{

	ifstream input;
	ofstream out1,out2;
	input.open("inp-params.txt");
	out1.open("EDF-Log.txt");
	out2.open("EDF-Stats.txt");
	int n;
	input>>n;
	process arr[n];                     // taking the input parameters
	vector< pair<int,process> > event;  // stores the instances of each process along with their arrival time
	int total_process=0;                //total instances of all process
	int missed_process=0;
	int l;
	long int wait=0,turnaround=0;
	double average_wait;
	double average_turnaround;
	for(int i=0;i<n;i++)
	{
		input>>arr[i].pid;
		input>>arr[i].t;
		input>>arr[i].period;
		input>>arr[i].k;
		arr[i].remaining_time=arr[i].t;
	}
	for(int i=0;i<n;i++)
	{
		int start_time=0;
		int iteration=arr[i].k;
		for(int j=0;j<iteration;j++)
		{
			process p=arr[i];
			p.instance=j+1;
			p.no=i;
			p.s_time=start_time;
			event.push_back(make_pair(start_time,p));
			total_process++;
			start_time+=arr[i].period;
		}
	}
	sort(event.begin(), event.end(),cmp);
	set<process,compar> Q;	//ready queue
	process current_process;
	int current_time=0; //stores the current time
	int i=0;            //pointer to store the first element whose arrival time is greater than current time
	while(i<n)          //inserting first instances of each process and putting them in appropriate queue
	{
		event[i].second.arrive_time=0;
		process temp = event[i].second;
		out1<<"Process "<<temp.pid<<": processing time="<<temp.t<<";deadline:"<<temp.period<<";period:"<<temp.period<<" joined the system at time 0\n";
		Q.insert(event[i].second);
		i++;
	}
	while(1)
	{
		if(Q.empty() && i>=total_process)   // if ready queue is empty and there are no instances to come then exit the program
		{
			break;
		}
		else if( i >= total_process)    // if all the instances have arrived but some are in ready queue
		{
			while(!Q.empty())
			{
				current_process=*Q.begin(); //remove the element with earliest deadline
				Q.erase(Q.begin());
				if((current_process.remaining_time+current_time) > (current_process.instance * current_process.period)) // if it has missed its deadline
				{
					missed_process++;
					if(current_time > current_process.instance * current_process.period )
					{
						wait+=(current_process.instance * current_process.period - current_process.arrive_time);
						out1<<"Process "<<current_process.pid<<" misses its deadline.\n";
					}
					else
					{
						wait+=current_time - current_process.arrive_time;
						out1<<"Process "<<current_process.pid<<" starts execution at time "<<current_time<<endl;
						current_time = current_process.instance * current_process.period;
						out1<<"Process "<<current_process.pid<<" misses its deadline.\n";
					}
				}
				else if(current_process.remaining_time == current_process.t)
				{
				    wait+=(current_time - current_process.arrive_time);
					out1<<"Process "<<current_process.pid<<" starts execution at time "<<current_time<<endl;
					out1<<"Process "<<current_process.pid<< " finishes execution at time "<<current_process.remaining_time+current_time<<endl;
					current_time=current_process.remaining_time+current_time;
					turnaround+=(current_time - current_process.s_time);
				}
				else
				{
				    wait+=(current_time - current_process.arrive_time);
					out1<<"Process "<<current_process.pid<<" resumes execution at time "<<current_time<<endl;
					current_time=current_process.remaining_time+current_time;
					out1<<"Process "<<current_process.pid<< " finishes execution at time "<<current_time<<endl;
					turnaround+=(current_time - current_process.s_time);
				}
			}
		}
		else if(Q.empty())  // if ready queue is empty
		{
			event[i].second.arrive_time=event[i].first;
			Q.insert(event[i].second);
			if(event[i].first > current_time)
				out1<<"CPU is idle till time " <<event[i].first<< ".\n";
			current_time=event[i].first;
			i++;
		}
		else
		{
			current_process=*(Q.begin());
			Q.erase(Q.begin());
			wait+=(current_time - current_process.arrive_time);
			int end_time = current_process.remaining_time + current_time;
			if(current_process.remaining_time + current_time <= event[i].first )    //comparing  execution end time with arrival of next process
			{
				if(current_process.instance * current_process.period < end_time)
				{
					missed_process++;
					if(current_time >= current_process.instance * current_process.period)
					{
						out1<<"Process "<<current_process.pid<<" misses its deadline.\n";
					}
					else
					{
						if(current_process.remaining_time == current_process.t)
							out1<<"Process "<<current_process.pid<<" starts execution at time "<<current_time<<endl;
						else
							out1<<"Process "<<current_process.pid<<" resumes execution at time "<<current_time<<endl;
						out1<<"Process "<<current_process.pid<<" misses its deadline.\n";
						current_time=current_process.instance * current_process.period;
					}
				}
				else if(current_process.remaining_time == current_process.t)
				{
					out1<<"Process "<<current_process.pid<<" starts execution at time "<<current_time<<endl;
					out1<<"Process "<<current_process.pid<< " finishes execution at time "<<current_process.remaining_time+current_time<<endl;
					current_time=current_process.remaining_time+current_time;
					turnaround+=(current_time - current_process.s_time);
				}
				else
				{
					out1<<"Process "<<current_process.pid<<" resumes execution at time "<<current_time<<endl;
					current_time=current_process.remaining_time+current_time;
					out1<<"Process "<<current_process.pid<< " finishes execution at time "<<current_time<<endl;
					turnaround+=(current_time - current_process.s_time);
				}
				while(i<total_process && event[i].first==end_time)
				{
					set<process,compar> :: iterator it;
					if(!Q.empty())
					{
						for(it=Q.begin();it!=Q.end();it++)
						{
							if((*it).no == event[i].second.no)
							{
								break;
							}
						}
						if(it!=Q.end())
						{
							missed_process++;
							process p = *it;
							out1<<"Process "<<p.pid<<" misses its deadline.\n";//if there is another instance of a process in ready queue then that instance misses deadline
							wait+=(p.instance*p.period-p.arrive_time);
							Q.erase(it);
						}
					}
					event[i].second.arrive_time=event[i].first;
					Q.insert(event[i].second);
					i++;
				}
			}
			else
			{
				int end_time = min(current_process.remaining_time+current_time , current_process.period*current_process.instance);
				int flag=0;
				if(current_process.remaining_time == current_process.t)
					out1<<"Process "<<current_process.pid<<" starts execution at time "<<current_time<<endl;
				else
					out1<<"Process "<<current_process.pid<<" resumes execution at time "<<current_time<<endl;
				while(i<total_process)
				{
					if(end_time <= event[i].first)  //running till end of execution of process or deadline
							{
								if(end_time == current_process.remaining_time+current_time)
									out1<<"Process "<<current_process.pid<< " finishes execution at time "<<current_process.remaining_time+current_time<<endl;
								else
									{
										missed_process++;
										out1<<"Process "<<current_process.pid<<" misses its deadline.\n";
									}
								current_time=end_time;
								turnaround+=(current_time - current_process.s_time);
								while(i<total_process && event[i].first==end_time)
								{
									set<process,compar> :: iterator it;
									if(!Q.empty())
									{
										for(it=Q.begin();it!=Q.end();it++)
										{
											if((*it).no==event[i].second.no)
											{
												break;
											}
										}
										if(it!=Q.end()) //if there is another instance of a process in ready queue then that instance misses deadline
										{
											missed_process++;
											process p = *it;
											out1<<"Process "<<p.pid<<" misses its deadline.\n";
											wait+=(p.instance*p.period-p.arrive_time);
											Q.erase(it);
										}
									}
									event[i].second.arrive_time=event[i].first;
									Q.insert(event[i].second);
									i++;
								}
								flag=1;
								break;
							}
							else if((event[i].second.period*event[i].second.instance) >= (current_process.period * current_process.instance))//lower priority process comes then push it into ready queue
							{
								event[i].second.arrive_time=current_time;
								set<process,compar> :: iterator it;
								if(!Q.empty())
								{
									for(it=Q.begin();it!=Q.end();it++)
									{
										if((*it).no==event[i].second.no)
										{
											break;
										}
									}
									if(it!=Q.end()) //if there is another instance of a process in ready queue then that instance misses deadline
									{
										missed_process++;
										process p = *it;
										out1<<"Process "<<p.pid<<" misses its deadline.\n";
										wait+=(p.instance*p.period-p.arrive_time);
										Q.erase(it);
									}
								}
								if(current_process.pid == event[i].second.pid)  //if another instance of a running process arrives then current process misses deadline
									{
										missed_process++;
										out1<<"Process "<<current_process.pid<<" misses its deadline.\n";
										current_time=current_process.instance*current_process.period;
										event[i].second.arrive_time=event[i].first;
										Q.insert(event[i].second);
										i++;
										flag=1;break;
									}
									event[i].second.arrive_time=event[i].first;
									Q.insert(event[i].second);
							}
							else     //higher priority process comes, preempts the current process and push it into ready queue
							{

								out1<<"Process "<<current_process.pid<< " is preempted by process "<<event[i].second.pid<<" at time "<<event[i].first<<".";
								current_process.remaining_time-=(event[i].first-current_time);
								out1<<"Remaining processing time : "<<current_process.remaining_time <<"\n";
								current_time=event[i].first;
								current_process.arrive_time=current_time;
								event[i].second.arrive_time=current_time;
								set<process,compar> :: iterator it;
								if(!Q.empty())
								{
									for(it=Q.begin();it!=Q.end();it++)
									{
										if((*it).no==event[i].second.no)
										{
											break;
										}
									}
									if(it!=Q.end())
									{
										missed_process++;
										process p = *it;
										out1<<"Process "<<p.pid<<" misses its deadline.\n";
										wait+=(p.instance*p.period-p.arrive_time);
										Q.erase(it);
									}
								}
								event[i].second.arrive_time=event[i].first;
								Q.insert(event[i].second);
								if(current_process.pid == event[i].second.pid)//if another instance of a running process arrives then current process misses deadline
								{
									missed_process++;
									out1<<"Process "<<current_process.pid<<" misses its deadline.\n";
									current_time=current_process.instance*current_process.period;
								}
								else
								{
									current_process.arrive_time=current_time;
									Q.insert(current_process);
								}
								i++;
								flag=1;
								break;
							}
						i++;
				}
				if(flag==0) //if all the lower priority process comes after a process
				{
					if(end_time == current_process.remaining_time+current_time)
						out1<<"Process "<<current_process.pid<< " finishes execution at time "<<current_process.remaining_time+current_time<<endl;
					else
						{
							missed_process++;
							out1<<"Process "<<current_process.pid<<" misses its deadline.\n";
						}
					current_time=current_process.remaining_time+current_time;
					turnaround+=(current_time - current_process.s_time);
				}
			}
		}
	}
	average_wait=(wait*1.0)/(total_process*1.0);
	out2<<"Total number of processes that came into the system = "<<total_process<<" ."<<endl;
	out2<<"Number of processes that successfully completed = "<<total_process - missed_process<<" .\n";
	out2<<"Number of processes that missed their deadlines = "<<missed_process<<" .\n";
	out2<<"Average wait time = "<<average_wait<<" .\n";
	input.close();
	out1.close();
	out1.close();
	return 0;
}
