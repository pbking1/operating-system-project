#include<iostream>
#include<cstring>
#include<typeinfo>
#include<string>
#include<fstream>
#include<algorithm>
#include<vector>
#include<cstdlib>
#include<cstdio>
#include<cmath>
#include<queue>
#include<deque>
#include<iomanip>
using namespace std;

typedef struct Node{
	int pid; //the pid of the process
	int arrive_t;  //the arrive time of the process
	int burst_t;  //the burst time of the process
	int origin_burst_t; //save the origin burst time
	int remain_t;  //the remain time of the process
	int wait_t; //the whole wait time of the process
	int begin_t; //judge when the process begin
	int response; // response time of a process
	int finish_time; //judge whether the process is finished
	int current_flag;  //judge whether the process is current process
	int priority;  //the priority of the process
	double turna_t; //turnaround time of the process
}PCB;

//define the time and other parameters
//waiting time is all the time that the process is waiting for execution
double aver_waiting_time;
//reponse time is the first waiting time (when process arrive till tje process got run the first time)
double aver_response_time;
//turn_around time is finish time - arrive time
double aver_turn_around_time;

int get_filerow(char *a){
	ifstream fin;
	string line;
	int counter = 0;
	fin.open(a);
	if(fin){
		while(getline(fin, line, '\n')){
			counter++;
		}
		fin.close();
	}
	return counter;
}

PCB jobs[20];
void import_data(char *a){
	int i = 0, j = 0;
	string line;
	int b;
	ifstream in(a);
	char c;
	int row_number = 3 * get_filerow(a);
	int array_store[row_number];
	for(i = 0; i < row_number; i++)
		array_store[i] = 0;
	//read file
	for(i = 0; i < row_number; i++){
		in>>b;
		c = in.peek();
		if('\t' == c){
			break;
		}
		array_store[j] = b;
		j++;
	}
	for(i = 0, j = 0; i < row_number; ){
		jobs[j].pid = array_store[i];
		jobs[j].arrive_t = array_store[i + 1];
		jobs[j].burst_t = array_store[i + 2];
		jobs[j].remain_t = 0;
		jobs[j].wait_t = 0;
		jobs[j].current_flag = 0;
		j++;
		i += 3;
	}
	in.close();
}

bool compare_SJF(PCB a, PCB b){
	if(a.arrive_t == b.arrive_t){
		return a.burst_t < b.burst_t;
	}
	return a.arrive_t < b.arrive_t;
}

bool compare_SRJF(PCB a, PCB b){
	return a.burst_t < b.burst_t;
}

bool compare_FCFS(PCB a, PCB b){
	return a.arrive_t < b.arrive_t;
}

//bug need to fix the arrive time
void SJF(char *a){
	import_data(a);
	vector<PCB> job(get_filerow(a));
	for(int j = 0; j < get_filerow(a); j++){
		job[j].pid = jobs[j].pid;
		job[j].arrive_t = jobs[j].arrive_t;
		job[j].burst_t = jobs[j].burst_t;
		job[j].origin_burst_t = jobs[j].burst_t;
		job[j].begin_t = -1;
	}
	sort(job.begin(), job.end(), compare_SJF);	
	
	//SJF algorithm begin	
	int counter = 0;
	int process_num = get_filerow(a);
	cout<<"Schdeuling algorithm: SJF"<<endl;
	cout<<"Total "<<process_num<<" tasks are read from "<<a<<endl;
	cout<<"press 'enter' to start"<<endl;
	cout<<"==================================================="<<endl;
	//pause the program
	cin.get();

	//count the cpu run time
	double cpu_count = 0;

	//begin algorithm
	while(1){
		//cin.get();
		//if all process are done
		int done_flag = 0;
		for(int i = 0; i < process_num; i++){
			done_flag += job[i].burst_t;
		}
		if(done_flag == 0){
			break;
		}
		//time counter ++
		counter++;
		//if the process comes
		int current_process_num = 0;
		for(int j = 0; j < process_num; j++){
			if(counter > job[j].arrive_t){
				job[j].current_flag = 1;
				//current_process_num++;
			}
		}
		//calculate the current process number
		for(int i = 0; i < process_num; i++){
			if(job[i].burst_t > 0 && job[i].current_flag == 1){
				current_process_num++;
			}

		}
		if(current_process_num > 0){
			vector<PCB> current_process(current_process_num);
			//copy the current process to the vector
			for(int i = 0, j = 0; i < process_num; i++){
				if(job[i].current_flag == 1){
					if(job[i].burst_t > 0){
						current_process[j].pid = job[i].pid;
						current_process[j].arrive_t = job[i].arrive_t;
						current_process[j].burst_t = job[i].burst_t;
						j++;
					}
				}
			}
			//sort the current_process
			sort(current_process.begin(), current_process.end(), compare_SRJF);
			cout<<endl;
			//earse the leading 0
			vector<PCB>::iterator iter;
			while(current_process[0].pid <= 0){
				iter = current_process.begin();
				current_process.erase(iter);
			}
			//substract the burst time in the value of 1
			if(current_process[0].pid != 0){
				cout<<"<system time "<<counter<<"> process "<<current_process[0].pid<<" is running";
				for(int i = 0; i < process_num; i++){
					if(current_process[0].pid == job[i].pid){
						if(job[i].begin_t == -1){
							job[i].begin_t = counter;
							//cout<<endl<<job[i].pid<<" "<<counter<<endl;
						}
					}
				}

				current_process[0].burst_t -= 1;
				if(current_process[0].burst_t == 0){
					cout<<endl<<"<system time "<<counter<<"> process "<<current_process[0].pid<<" is finished........";
					for(int i = 0; i < process_num; i++){
						if(current_process[0].pid == job[i].pid){
							job[i].finish_time = counter;
						}
					}
				}
			}
			
			for(int i = 0; i < process_num; i++){
				for(int j = 0; j < current_process_num; j++){
					if(job[i].pid == current_process[j].pid){
						if(current_process[j].burst_t >= 0){
							job[i].burst_t = current_process[j].burst_t;
						}
					}
				}
			}

		}else{
			cpu_count++;
		}
	}



	cout<<"\n==================================================="<<endl; 
	//get the cpu usage time
	double cpu_all = double(counter);
	cout<<"Average cpu usage: "<<setprecision(2) <<fixed <<100*(((double)cpu_all-(double)cpu_count)/(double)cpu_all)<<"%"<<endl;

	double all_waiting_t = 0;
	double all_turnaround = 0;
	double all_response = 0;

	//calculate the statistic
	for(int i = 0; i < process_num; i++){
		job[i].turna_t = job[i].finish_time - job[i].arrive_t;
		all_turnaround += job[i].turna_t;
		job[i].response = job[i].begin_t - job[i].arrive_t;
		all_response += job[i].response;
		//waiting time = (arrive time - first begin time) + sum(next begin time - previous end time)
		job[i].wait_t = job[i].finish_time - job[i].arrive_t - job[i].origin_burst_t;
		all_waiting_t += job[i].wait_t;
	}

	double aver_waiting_time = double(all_waiting_t) / double(process_num);
	double aver_turn_around_time = double(all_turnaround) / double(process_num);
	double aver_response_time = double(all_response) / double(process_num);	
	cout<<"Average waiting time: "<<setprecision(2) <<fixed <<aver_waiting_time<<endl;
	cout<<"Average response time: "<<setprecision(2) <<fixed <<aver_response_time - 1<<endl;
	cout<<"Average turnaround time: "<<setprecision(2) <<fixed <<aver_turn_around_time<<endl;
	cout<<"==================================================="<<endl; 
}

void FCFS(char *a){
	import_data(a);
	vector<PCB> job(get_filerow(a));
	for(int j = 0; j < get_filerow(a); j++){
		job[j].pid = jobs[j].pid;
		job[j].arrive_t = jobs[j].arrive_t;
		job[j].burst_t = jobs[j].burst_t;
	}
	//FCFS algorithm begin
	int counter = 0;
	int cpu_count = 0, cpu_all = 0;
	int process_num = get_filerow(a);
	cout<<"Schdeuling algorithm: FCFS"<<endl;
	cout<<"Total "<<process_num<<" tasks are read from "<<a<<endl;
	cout<<"press 'enter' to start"<<endl;
	cout<<"==================================================="<<endl;
	//pause the program
	cin.get();
	//calculate the time
	aver_waiting_time = 0;
	aver_response_time = 0;
	aver_turn_around_time = 0;
	
	//algorithm process
	sort(job.begin(), job.end(), compare_FCFS);
	job[0].begin_t = 0;	
	for(int i = 0; i < process_num; i++){
		for(int j = 0; j < job[i].burst_t; j++){
			cout<<"<system time "<<counter<<"> process "<<job[i].pid<<" is running"<<endl;
			if(j == job[i].burst_t - 1)
				job[i + 1].begin_t = counter + 1;
			counter++;
			//if the jobs are done and the next job is not arriving
			//increase the counter of cpu
			if(job[i+1].arrive_t > counter)
				cpu_count++;
		}
		cout<<"<system time "<<counter<<"> process "<<job[i].pid<<" finished......"<<endl;
		job[i].finish_time = counter;
	}
	cout<<"<system time "<<counter<<"> All process finished......."<<endl;
	cout<<"==================================================="<<endl; 
	//get the cpu usage time
	cpu_all = double(counter);
	cout<<"Average cpu usage: "<<setprecision(2) <<fixed <<100*(((double)cpu_all-(double)cpu_count)/(double)cpu_all)<<"%"<<endl;
	double all_waiting_t = 0;
	double all_turnaround = 0;
	double all_response = 0;
	for(int i = 0; i < process_num; i++){
		job[i].wait_t = job[i].begin_t - job[i].arrive_t;
		job[i].turna_t = job[i].finish_time - job[i].arrive_t;
		job[i].response = job[i].begin_t - job[i].arrive_t;
		all_turnaround += job[i].turna_t;
		all_waiting_t += job[i].wait_t;
		all_response += job[i].response;
	}
	aver_waiting_time = double(all_waiting_t) / double(process_num);
	aver_turn_around_time = double(all_turnaround) / double(process_num);
	aver_response_time = double(all_response) / double(process_num);	
	cout<<"Average waiting time: "<<setprecision(2) <<fixed <<aver_waiting_time<<endl;
	cout<<"Average response time: "<<setprecision(2) <<fixed <<aver_response_time<<endl;
	cout<<"Average turnaround time: "<<setprecision(2) <<fixed <<aver_turn_around_time<<endl;
	cout<<"==================================================="<<endl; 
}

void RR(char *a, double quan){
	import_data(a);
	vector<PCB> job(get_filerow(a));
	for(int j = 0; j < get_filerow(a); j++){
		job[j].pid = jobs[j].pid;
		job[j].arrive_t = jobs[j].arrive_t;
		job[j].burst_t = jobs[j].burst_t;
		job[j].origin_burst_t = jobs[j].burst_t;
		job[j].begin_t = -1;
	}
	//RR algorithm begin
	int _round = int(quan); //quan time
	int counter = 0;
	int process_num = get_filerow(a);
	cout<<"Schdeuling algorithm: RR"<<endl;
	cout<<"Total "<<process_num<<" tasks are read from "<<a<<endl;
	cout<<"press 'enter' to start"<<endl;
	cout<<"==================================================="<<endl;
	//pause the program
	cin.get();
	double cpu_count = 0;
	//use a queue to store the current process
	//and give the time slice to the top 
	//and after the top finish, let the new comer pump in the queue
	//then pop the top process, and then pump into the back(if the burst time is not zero)
	deque<PCB> current_proces;
	PCB temp;
	temp.pid = temp.arrive_t = temp.burst_t = 0;
	while(1){
		int done_flag = 0;
		for(int i = 0; i < process_num; i++)
			done_flag += job[i].burst_t;
		if(done_flag == 0)
			break;
		//time counter ++
		//counter++;
		for(int i = 0; i < process_num; i++){
			if(counter >= job[i].arrive_t)
				if(job[i].current_flag != 1){
					job[i].current_flag = 1;
					//find out the current time which process is coming
					//and add to the deque
					current_proces.push_back(job[i]);
				}
		}
		//counter--;
		if(temp.burst_t > 0)
			current_proces.push_back(temp);

		if(current_proces.size() == 0){
			cpu_count++;
		}else{	
			deque<PCB>::iterator iter = current_proces.begin();
			/*deque<PCB>::iterator iter1 = current_proces.begin();
			while(iter1 != current_proces.end()){
				cout<<(*iter1).pid<<" "; 
				iter1++;
			}
			cout<<endl;*/
			//renew the deque
			for(int i = 0; i < process_num; i++){
				if( (*iter).pid == job[i].pid)
					(*iter).burst_t = job[i].burst_t;
			}

			if( (*iter).burst_t < _round){
				int n1 = (*iter).burst_t;
				for(int j = 0; j < n1; j++){
					cout<<"<system time "<<counter<<" >"<<(*iter).pid<<" is running";
					for(int i = 0; i < process_num; i++){
						if((*iter).pid== job[i].pid){
							if(job[i].begin_t == -1){
								job[i].begin_t = counter;
								cout<<endl<<job[i].pid<<" "<<counter<<endl;
							}
						}
					}
					//cout<<endl;
					for(int i = 0; i < process_num; i++){
						if((*iter).pid == job[i].pid){
							--job[i].burst_t;
							if(job[i].burst_t == 0){
								int mm = counter + 1;
								cout<<endl<<"<system time "<<mm<<" >"<<(*iter).pid<<" is finished......"<<endl;
								for(int i = 0; i < process_num; i++){
									if((*iter).pid == job[i].pid){
										job[i].finish_time = counter + 1;
									}
								}
							}
							break;
						}
					}
					counter++;
				}
			}else{
				int n1 = _round;
				while(n1--){
					cout<<"<system time "<<counter<<" >"<<(*iter).pid<<" is running";
					for(int i = 0; i < process_num; i++){
						if((*iter).pid== job[i].pid){
							if(job[i].begin_t == -1){
								job[i].begin_t = counter;
								cout<<endl<<job[i].pid<<" "<<counter<<endl;
							}
						}
					}
					cout<<endl;
					for(int i = 0; i < process_num; i++){
						if( (*iter).pid == job[i].pid){
							--job[i].burst_t;
							if(job[i].burst_t == 0){
								cout<<"<system time "<<counter<<" >"<<(*iter).pid<<" is finished......"<<endl; 
								for(int i = 0; i < process_num; i++){
									if((*iter).pid == job[i].pid){
										job[i].finish_time = counter + 1;
									}
								}
							}
							break;
						}
					}
					counter++;
				}
			}
			PCB front_process = current_proces.front();
			current_proces.pop_front();
			temp = front_process;
		}
	}

	cout<<"<system time "<<counter<<"> All process finished......."<<endl;	
	cout<<"==================================================="<<endl; 
	//get the cpu usage time
	double cpu_all = double(counter);
	cout<<"Average cpu usage: "<<setprecision(2) <<fixed <<100*(((double)cpu_all-(double)cpu_count)/(double)cpu_all)<<"%"<<endl;
	double all_waiting_t = 0;
	double all_turnaround = 0;
	double all_response = 0;

	//calculate the statistic
	for(int i = 0; i < process_num; i++){
		job[i].turna_t = job[i].finish_time - job[i].arrive_t;
		all_turnaround += job[i].turna_t;

		job[i].response = job[i].begin_t - job[i].arrive_t;
		all_response += job[i].response;

		job[i].wait_t = job[i].finish_time - job[i].arrive_t - job[i].origin_burst_t;
		all_waiting_t += job[i].wait_t;
	}

	double aver_waiting_time = double(all_waiting_t) / double(process_num);
	double aver_turn_around_time = double(all_turnaround) / double(process_num);
	double aver_response_time = double(all_response) / double(process_num);	
	cout<<"Average waiting time: "<<aver_waiting_time<<endl;
	cout<<"Average response time: "<<aver_response_time<<endl;
	cout<<"Average turnaround time: "<<aver_turn_around_time<<endl;
	cout<<"==================================================="<<endl;
}

void import_data_version2(char *a){
	int i = 0, j = 0;
	string line;
	int b;
	ifstream in(a);
	char c;
	int row_number = 4 * get_filerow(a);
	int array_store[row_number];
	for(i = 0; i < row_number; i++)
		array_store[i] = 0;
	//read file
	for(i = 0; i < row_number; i++){
		in>>b;
		c = in.peek();
		if('\t' == c){
			break;
		}
		array_store[j] = b;
		j++;
	}
	for(i = 0, j = 0; i < row_number; ){
		jobs[j].pid = array_store[i];
		jobs[j].arrive_t = array_store[i + 1];
		jobs[j].burst_t = array_store[i + 2];
		jobs[j].priority = array_store[i + 3];
		jobs[j].remain_t = 0;
		jobs[j].wait_t = 0;
		jobs[j].current_flag = 0;

		j++;
		i += 4;
	}
	in.close();
}

bool compare_PRIO(PCB a, PCB b){
	return a.priority > b.priority;
}

//when run PRIO and PRIO_NON use input.7
void PRIO(char *a){
	//this priority scheduling is preemptive
	import_data_version2(a);
	vector<PCB> job(get_filerow(a));
	for(int j = 0; j < get_filerow(a); j++){
		job[j].pid = jobs[j].pid;
		job[j].arrive_t = jobs[j].arrive_t;
		job[j].burst_t = jobs[j].burst_t;
		job[j].priority = jobs[j].priority;
		job[j].origin_burst_t = jobs[j].burst_t;
		job[j].begin_t = -1;
	}
	double cpu_count = 0;
	//PRIO algorithm begin	
	int counter = 0;
	int process_num = get_filerow(a);
	cout<<"Schdeuling algorithm: PRIO"<<endl;
	cout<<"Total "<<process_num<<" tasks are read from "<<a<<endl;
	cout<<"press 'enter' to start"<<endl;
	cout<<"==================================================="<<endl;
	//pause the program
	cin.get();
	while(1){
		int done_flag = 0;
		for(int i = 0; i < process_num; i++){
			done_flag += job[i].burst_t;
		}
		if(done_flag == 0)
			break;
		//time counter increase
		counter++;
		//calculate the current process
		int current_process_num = 0;
		for(int j = 0; j < process_num; j++){
			if(counter > job[j].arrive_t)
				job[j].current_flag = 1;
		}
		for(int i = 0; i < process_num; i++){
			if(job[i].burst_t > 0 && job[i].current_flag == 1)
				current_process_num++;
		}
		if(current_process_num > 0){
			vector<PCB> current_process(current_process_num);
			for(int i = 0, j = 0; i < process_num; i++){
				if(job[i].current_flag == 1){
					if(job[i].burst_t > 0){
						current_process[j].pid = job[i].pid;
						current_process[j].arrive_t = job[i].arrive_t;
						current_process[j].burst_t = job[i].burst_t;
						current_process[j].priority = job[i].priority;
						j++;
					}
				}
			}
			sort(current_process.begin(), current_process.end(), compare_PRIO);
			cout<<endl;
			//earse the leading zero
			vector<PCB>::iterator iter;
			while(current_process[0].pid <= 0){
				iter = current_process.begin();
				current_process.erase(iter);
			}
			if(current_process[0].pid != 0){
				if(current_process[0].burst_t > 0){
					cout<<"<system time "<<counter<<"> process "<<current_process[0].pid<<" is running";
					for(int i = 0; i < process_num; i++){
						if(current_process[0].pid== job[i].pid){
							if(job[i].begin_t == -1){
								job[i].begin_t = counter;
								cout<<endl<<job[i].pid<<" "<<counter<<endl;
							}
						}
					}
					current_process[0].burst_t -= 1;
				}
				if(current_process[0].burst_t <= 0){
					cout<<endl<<"<system time "<<counter<<"> process "<<current_process[0].pid<<" is finished........";
					for(int i = 0; i < process_num; i++){
						if(current_process[0].pid == job[i].pid){
							job[i].finish_time = counter;
						}
					}
				}
			}
			for(int i = 0; i < process_num; i++){
				for(int j = 0; j < current_process_num; j++){
					if(job[i].pid == current_process[j].pid){
						if(current_process[j].burst_t >= 0){
							job[i].burst_t = current_process[j].burst_t;
						}
					}
				}
			}
		}else{
			cpu_count++;
		}
	}
	cout<<endl<<"<system time "<<counter<<"> All process finished......."<<endl;
	cout<<"\n==================================================="<<endl; 
	
	double cpu_all = double(counter);
	cout<<"Average cpu usage: "<<setprecision(2) <<fixed <<100*(((double)cpu_all-(double)cpu_count)/(double)cpu_all)<<"%"<<endl;
	double all_waiting_t = 0;
	double all_turnaround = 0;
	double all_response = 0;

	//calculate the statistic
	for(int i = 0; i < process_num; i++){
		job[i].turna_t = job[i].finish_time - job[i].arrive_t;
		all_turnaround += job[i].turna_t;
		job[i].response = job[i].begin_t - job[i].arrive_t;
		all_response += job[i].response;
		job[i].wait_t = job[i].finish_time - job[i].arrive_t - job[i].origin_burst_t;
		all_waiting_t += job[i].wait_t;
	}

	double aver_waiting_time = double(all_waiting_t) / double(process_num);
	double aver_turn_around_time = double(all_turnaround) / double(process_num);
	double aver_response_time = double(all_response) / double(process_num);	
	cout<<"Average waiting time: "<<aver_waiting_time<<endl;
	cout<<"Average response time: "<<aver_response_time<<endl;
	cout<<"Average turnaround time: "<<aver_turn_around_time<<endl;
	cout<<"==================================================="<<endl; 
}

bool compare_PRIO_NON(PCB a, PCB b){
	if(a.arrive_t == b.arrive_t)
		return a.priority > b.priority;
	return a.arrive_t < b.arrive_t;
}

void PRIO_NON(char *a){
	//this priority scheduling is preemptive
	import_data_version2(a);
	vector<PCB> job(get_filerow(a));
	for(int j = 0; j < get_filerow(a); j++){
		job[j].pid = jobs[j].pid;
		job[j].arrive_t = jobs[j].arrive_t;
		job[j].burst_t = jobs[j].burst_t;
		job[j].priority = jobs[j].priority;
		//cout<<job[j].pid<<" "<<job[j].arrive_t<<" "<<job[j].burst_t<<" "<<job[j].priority<<endl;
	}
	//PRIO algorithm begin	
	int counter = 0;
	double cpu_count = 0;
	int process_num = get_filerow(a);
	cout<<"Schdeuling algorithm: PRIO_NON"<<endl;
	cout<<"Total "<<process_num<<" tasks are read from "<<a<<endl;
	cout<<"press 'enter' to start"<<endl;
	cout<<"==================================================="<<endl;
	//pause the program
	cin.get();
	while(1){
		int done_flag = 0;
		for(int i = 0; i < process_num; i++){
			done_flag += job[i].burst_t;
		}
		if(done_flag == 0)
			break;
		//time counter increase
		counter++;
		//calculate the current process
		int current_process_num = 0;
		for(int j = 0; j < process_num; j++){
			if(counter > job[j].arrive_t)
				job[j].current_flag = 1;
		}
		for(int i = 0; i < process_num; i++){
			if(job[i].burst_t > 0 && job[i].current_flag == 1)
				current_process_num++;
		}
		if(current_process_num > 0){
			vector<PCB> current_process(current_process_num);
			for(int i = 0, j = 0; i < process_num; i++){
				if(job[i].current_flag == 1){
					if(job[i].burst_t > 0){
						current_process[j].pid = job[i].pid;
						current_process[j].arrive_t = job[i].arrive_t;
						current_process[j].burst_t = job[i].burst_t;
						current_process[j].priority = job[i].priority;
						j++;
					}
				}
			}
			sort(current_process.begin(), current_process.end(), compare_PRIO_NON);
			cout<<endl;
			//earse the leading zero
			vector<PCB>::iterator iter;
			while(current_process[0].pid <= 0){
				iter = current_process.begin();
				current_process.erase(iter);
			}
			if(current_process[0].pid != 0){
				if(current_process[0].burst_t > 0){
					cout<<"<system time "<<counter<<"> process "<<current_process[0].pid<<" is running";
					current_process[0].burst_t -= 1;
				}
				if(current_process[0].burst_t <= 0){
					cout<<endl<<"<system time "<<counter<<"> process "<<current_process[0].pid<<" is finished........";
					for(int i = 0; i < process_num; i++){
						if(current_process[0].pid == job[i].pid){
							job[i].finish_time = counter;
							job[i].begin_t = counter - job[i].burst_t;
						}
					}

				}
			}
			for(int i = 0; i < process_num; i++){
				for(int j = 0; j < current_process_num; j++){
					if(job[i].pid == current_process[j].pid){
						if(current_process[j].burst_t >= 0){
							job[i].burst_t = current_process[j].burst_t;
						}
					}
				}
			}
		}else{
			cpu_count++;
		}
	}
	cout<<endl<<"<system time "<<counter<<"> All process finished......."<<endl;
	cout<<"\n==================================================="<<endl; 
	
	double cpu_all = double(counter);
	cout<<"Average cpu usage: "<<setprecision(2) <<fixed <<100*(((double)cpu_all-(double)cpu_count)/(double)cpu_all)<<"%"<<endl;
	double all_waiting_t = 0;
	double all_turnaround = 0;
	double all_response = 0;

	//calculate the statistic
	for(int i = 0; i < process_num; i++){
		job[i].turna_t = job[i].finish_time - job[i].arrive_t;
		all_turnaround += job[i].turna_t;
		job[i].response = job[i].begin_t - job[i].arrive_t;
		all_response += job[i].response;
		job[i].wait_t = job[i].begin_t - job[i].arrive_t;
		all_waiting_t += job[i].wait_t;
	}

	double aver_waiting_time = double(all_waiting_t) / double(process_num);
	double aver_turn_around_time = double(all_turnaround) / double(process_num);
	double aver_response_time = double(all_response) / double(process_num);	
	cout<<"Average waiting time: "<<aver_waiting_time<<endl;
	cout<<"Average response time: "<<aver_response_time<<endl;
	cout<<"Average turnaround time: "<<aver_turn_around_time<<endl;
	cout<<"==================================================="<<endl; 
}
int main(int argc, char *argv[]){

	if(strcmp(argv[2],"FCFS") == 0){
		cout<<"Scheduling algorithm: FCFS"<<endl;
		cout<<"The input file is "<<argv[1]<<endl;
		FCFS(argv[1]);
	}else if(strcmp(argv[2],"SJF") == 0){
		cout<<"Scheduling algorithm: SJF"<<endl;
		cout<<"The input file is "<<argv[1]<<endl;
		SJF(argv[1]);
	}else if(strcmp(argv[2],"RR") == 0){
		cout<<"Scheduling algorithm: RR"<<endl;
		cout<<"The input file is "<<argv[1]<<endl;
		double qua = 0;;
		char *q;
		q = argv[3];
		double j;
		for(int i = 0; i < strlen(q); i++){
			j = pow(10, double(strlen(q) - 1 - i));
			qua += j * (q[i] - '0');
		}
		RR(argv[1], qua);
	}else if(strcmp(argv[2],"PRIO") == 0){
		cout<<"Scheduling algorithm: PRIO"<<endl;
		cout<<"The input file is "<<argv[1]<<endl;
		PRIO(argv[1]);
	}else if(strcmp(argv[2],"PRIO_NON") == 0){
		cout<<"Scheduling algorithm: PRIO_NON_PREEMPTIVE"<<endl;
		cout<<"The input file is "<<argv[1]<<endl;
		PRIO_NON(argv[1]);
	}else{
		cout<<"Usage: proj1 input_file FCFS|SJF|RR|PRIO [quantum]\n";
	}
	return 0;
}
