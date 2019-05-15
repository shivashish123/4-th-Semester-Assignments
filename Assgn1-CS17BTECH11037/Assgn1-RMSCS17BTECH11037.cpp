#include<bits/stdc++.h>
using namespace std;
class process{

	public:
		string pid; //process id
		int t;      //processing time
		int period;
		int k;      //number of time process repeats
		int remaining_time;
		int instance;
		int s_time;     // start time of process
		int arrive_time;// arrival time of process in ready queue
};
bool cmp(const pair<int,process> &a,const pair<int,process> &b) // comparator function which compares arrival time if they are equal then period is considered
{
	if(a.first!=b.first)
		return a.first<b.first;
	else
		return (a.second).period<(b.second).period;
}
int main()
{
	ifstream input;
	ofstream out1,out2;
	input.open("inp-params.txt");
	out1.open("RMS-Log.txt");
	out2.open("RMS-Stats.txt");
	int n;
	input>>n;
	process arr[n];                         // taking the input parameters
	vector< pair<int,process> > event;     // stores the instances of each process along with their arrival time
	map<string,int> id;                     // maps every process id to a integer value used for accessing a particular queue given the process id
	int total_process=0;
	int missed_process=0;
	int l;
	vector< pair<int,string> > priority;   //  stores the sorted order of process based on their period
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
		priority.push_back(make_pair(arr[i].period,arr[i].pid));
		id.insert(make_pair(arr[i].pid,i));
	}
	for(int i=0;i<n;i++)
	{
		int start_time=0;
		int iteration=arr[i].k;
		for(int j=0;j<iteration;j++)
		{
			process p=arr[i];
			p.instance=j+1;
			p.s_time=start_time;
			event.push_back(make_pair(start_time,p));
			total_process++;
			start_time+=arr[i].period;
		}
	}
	sort(event.begin(), event.end(),cmp);
	sort(priority.begin(),priority.end());
	queue<process> Q[n];
	process current_process;
	int current_time=0; //stores the current time
	int i=0;        //pointer to store the first element whose arrival time is greater than current time
	while(i<n)      //inserting first instances of each process and putting them in appropriate queue
	{
		event[i].second.arrive_time=0;
		process temp = event[i].second;
		out1<<"Process "<<temp.pid<<": processing time="<<temp.t<<";deadline:"<<temp.period<<";period:"<<temp.period<<" joined the system at time 0\n";
		Q[id[event[i].second.pid]].push(event[i].second);
		i++;
	}
	while(1)
	{
		bool isempty = true; //denotes if ready queue is empty or not
		for(int j=0;j<n;j++)
		{
			if(!Q[j].empty())
			{
				isempty=false;
				break;
			}
		}
		if(isempty && i>=total_process)  // if ready queue is empty and there are no instances to come then exit the program
		{
			break;
		}
		else if( i >= total_process)   // if all the instances have arrived but some are in ready queue
		{
			for(int j=0;j<n;j++)
				{
					while(!Q[id[priority[j].second]].empty())
					{
						current_process=Q[id[priority[j].second]].front();
						Q[id[priority[j].second]].pop();
						if((current_process.remaining_time+current_time) > (current_process.instance * current_process.period))
						{
							missed_process++;
							if(current_time >= current_process.instance * current_process.period )
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
		}
		else if(isempty) // if ready queue is empty
		{
			event[i].second.arrive_time=event[i].first;
			Q[id[event[i].second.pid]].push(event[i].second);
			if(event[i].first > current_time)
			out1<<"CPU is idle till time " <<event[i].first<< ".\n";
			current_time=event[i].first;
			i++;
		}
		else
		{
			for(int j=0;j<n;j++)
				{
					if(!Q[id[priority[j].second]].empty()) //remove the highest priority process
					{
						current_process=Q[id[priority[j].second]].front();
						Q[id[priority[j].second]].pop();
						wait+=(current_time - current_process.arrive_time);
						break;
					}
				}
			if(current_process.remaining_time + current_time <= event[i].first ) //comparing  execution end time with arrival of next process
			{
				int end_time = current_process.remaining_time+current_time;
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
					if(!Q[id[event[i].second.pid]].empty()) //if there is another instance of a process in ready queue then that instance misses deadline
					{
						missed_process++;
						process p = Q[id[event[i].second.pid]].front();
						out1<<"Process "<<p.pid<<" misses its deadline.\n";
						wait+=(p.instance*p.period-p.arrive_time);
						Q[id[event[i].second.pid]].pop();
					}
					event[i].second.arrive_time=event[i].first;
					Q[id[event[i].second.pid]].push(event[i].second);
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
					if(end_time <= event[i].first) //running till end of execution of process or deadline
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
									if(!Q[id[event[i].second.pid]].empty())//if there is another instance of a process in ready queue then that instance misses deadline
									{
										missed_process++;
										process p = Q[id[event[i].second.pid]].front();
										out1<<"Process "<<p.pid<<" misses its deadline.\n";
										wait+=(p.instance*p.period-p.arrive_time);
										Q[id[event[i].second.pid]].pop();
									}
									event[i].second.arrive_time=event[i].first;
									Q[id[event[i].second.pid]].push(event[i].second);
									i++;
								}
								flag=1;
								break;
							}
							else if(event[i].second.period >= current_process.period) //lower priority process comes then push it into ready queue
							{
								event[i].second.arrive_time=current_time;
								if(!Q[id[event[i].second.pid]].empty())//if there is another instance of a process in ready queue then that instance misses deadline
									{
										missed_process++;
										process p = Q[id[event[i].second.pid]].front();
										out1<<"Process "<<p.pid<<" misses its deadline.\n";
										wait+=(p.instance*p.period-p.arrive_time);
										Q[id[event[i].second.pid]].pop();
										event[i].second.arrive_time=event[i].first;
										Q[id[event[i].second.pid]].push(event[i].second);
									}
								else
									{
										if(current_process.pid == event[i].second.pid) //if another instance of a running process arrives then current process misses deadline
											{
												missed_process++;
												out1<<"Process "<<current_process.pid<<" misses its deadline.\n";
												flag=1;break;
											}
										event[i].second.arrive_time=event[i].first;
										Q[id[event[i].second.pid]].push(event[i].second);
									}
							}
							else                        //higher priority process comes, preempts the current process and push it into ready queue
							{

								out1<<"Process "<<current_process.pid<< " is preempted by process "<<event[i].second.pid<<" at time "<<event[i].first<<".";
								current_process.remaining_time-=(event[i].first-current_time);
								out1<<"Remaining processing time : "<<current_process.remaining_time <<"\n";
								current_time=event[i].first;
								current_process.arrive_time=current_time;
								event[i].second.arrive_time=current_time;
								if(!Q[id[event[i].second.pid]].empty())
									{
										missed_process++;
										process p = Q[id[event[i].second.pid]].front();
										out1<<"Process "<<p.pid<<" misses its deadline.\n";
										wait+=(p.instance*p.period-p.arrive_time);
										Q[id[event[i].second.pid]].pop();
										event[i].second.arrive_time=event[i].first;
										Q[id[event[i].second.pid]].push(event[i].second);
									}
								else
									{
										event[i].second.arrive_time=event[i].first;
										Q[id[event[i].second.pid]].push(event[i].second);
									}
								if(current_process.pid == event[i].second.pid)//if another instance of a running process arrives then current process misses deadline
								{
									missed_process++;
									out1<<"Process "<<current_process.pid<<" misses its deadline.\n";
									current_time=current_process.instance*current_process.period;
								}
								else
								{
									current_process.arrive_time=current_time;
									Q[id[current_process.pid]].push(current_process);
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
