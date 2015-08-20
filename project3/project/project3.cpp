#include<iostream>
#include<queue>
#include<deque>
#include<list>
#include<ctime>
#include<cstdlib>
using namespace std;
//The maximum number in a queue is 3
#define total_number_of_page_frame 3
//Total process number
#define total_number_of_process 10

struct Process{
	int segment_number;
	int page_number;
	int offset;
};

bool find_key(deque<Process> q ,int key){
	deque<Process>::iterator it;
	for(it = q.begin(); it != q.end(); it++){
		if(it->segment_number == key)
			return true;
	}
	return false;
}

int main(){
	//store all the process
	cout<<"Total number of page frame is 3\n";
	cout<<"Total number of process is 10\n";
	cout<<"The program generate the input by itself, no input output file\n";
	list<Process> process_list;
	for(int i = 0; i < total_number_of_process; i++){
		Process temp;
		temp.segment_number = rand() % (7 - 2 + 1) + 2;
		temp.page_number = i + 1;
		temp.offset = i + 2;
		process_list.push_back(temp);
	}
	list<Process>::iterator iter;
	cout<<"The data of the process\n";
	for(iter = process_list.begin(); iter != process_list.end(); iter++)
		cout<<iter->segment_number<<" "<<iter->page_number<<" "<<iter->offset<<endl;
	deque<Process> q;
	int count = 0;
	int hit_count = 0;
	cout<<endl<<"FIFO begin"<<endl;
	for(iter = process_list.begin(); iter != process_list.end(); iter++){
		if(count < total_number_of_page_frame){
			if(find_key(q,iter->segment_number) == true){
				cout<<"# "<<iter->segment_number<<" exist #";
				hit_count = hit_count + 1;
			}else{
				q.push_back(*iter);
				count++;
			}
		}else{
			if(find_key(q,iter->segment_number) == true){
				cout<<"# "<<iter->segment_number<<" exist #";
				hit_count = hit_count + 1;
			}else{
				q.pop_front();
				q.push_back(*iter);
			}
		}
		deque<Process> temp(q);
		while(!temp.empty()){
			Process head = temp.front();
			int h = head.segment_number;
			cout<<h<<" ";
			temp.pop_front();
		}
		cout<<endl;
	}
	cout<<"FIFO end and the hit rate is "<<hit_count*1.0/total_number_of_process*100<<"%"<<endl;
	return 0;
}
