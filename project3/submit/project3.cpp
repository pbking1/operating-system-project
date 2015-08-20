#include<iostream>
#include<queue>
#include<deque>
#include<list>
#include<ctime>
#include<cstdlib>
#include<algorithm>

using namespace std;
//The maximum number in a queue is 3
#define total_number_of_page_frame 3
//Total process number
#define total_number_of_process 20

// erase this #define to use random input
#define MANUAL_INPUT
/*** GLOBAL VARIABLES****/
#ifdef MANUAL_INPUT
	int arr[] = { 7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1 };
#endif
int number_page_fault;
/************************/


struct Process{
	int segment_number;
	int page_number;
	int offset;
	int timer; // to record time for LRU
	int next_ref; // to record next reference for Optimum(X)
};

bool find_key(deque<Process>& q, int key){
	deque<Process>::iterator it;
	for (it = q.begin(); it != q.end(); it++){
		if (it->segment_number == key){
			// reset timer
			it->timer = 0;
			return true;
		}
	}
	return false;
}

int FIFO(){
	number_page_fault = 0;
	//store all the process
	cout << "Total number of page frame is 3\n";
	cout << "Total number of process is 10\n";
	cout << "The program generate the input by itself, no input output file\n";
	list<Process> process_list;
	for (int i = 0; i < total_number_of_process; i++){
		Process temp;
		temp.segment_number = rand() % (7 - 2 + 1) + 2;

#ifdef MANUAL_INPUT
		temp.segment_number = arr[i];
#endif
		temp.page_number = i + 1;
		temp.offset = i + 2;
		process_list.push_back(temp);
	}
	list<Process>::iterator iter;
	cout << "The data of the process\n";
	for (iter = process_list.begin(); iter != process_list.end(); iter++)
		cout << iter->segment_number << " " << iter->page_number << " " << iter->offset << endl;
	deque<Process> q;
	int count = 0;
	cout << endl << "FIFO begin" << endl;
	for (iter = process_list.begin(); iter != process_list.end(); iter++){
		if (count < total_number_of_page_frame){
			if (find_key(q, iter->segment_number) == true){
				cout << "# " << iter->segment_number << " exist #";
			}
			else{
				q.push_back(*iter);
				count++;
			}
			number_page_fault++;
		}
		else{
			if (find_key(q, iter->segment_number) == true){
				cout << "# " << iter->segment_number << " exist #";
			}
			else{
				q.pop_front();
				q.push_back(*iter);
				number_page_fault++;
			}
		}
		deque<Process> temp(q);
		while (!temp.empty()){
			Process head = temp.front();
			int h = head.segment_number;
			cout << h << " ";
			temp.pop_front();
		}
		cout << endl;
	}
	cout << "FIFO end" << endl;
	cout << "Number of page fault: " << number_page_fault << endl << endl;
	return 0;
}




static bool timer_compare(const Process& a, const Process& b){
	return a.timer < b.timer;
}

void replaceLRU(deque<Process>& q, Process& p){
	// find the LRU, based on the largest timer
	deque<Process>::iterator it = max_element(q.begin(), q.end(), timer_compare);
	
	// replace with the max timer element, and reset timer to 0
	*it = p;
	it->timer = 0;
	number_page_fault++;
}

void increaseTimer(Process& P){
	P.timer++;
}




int LRU(){

	number_page_fault = 0;

	//store all the process
	cout << "Total number of page frame is 3\n";
	cout << "Total number of process is 10\n";
	cout << "The program generate the input by itself, no input output file\n";

	list<Process> process_list;
	for (int i = 0; i < total_number_of_process; i++){
		Process temp;
		temp.segment_number = rand() % (7 - 2 + 1) + 2;
#ifdef MANUAL_INPUT
		temp.segment_number = arr[i];
#endif
		temp.page_number = i + 1;
		temp.offset = i + 2;
		temp.timer = 0;
		process_list.push_back(temp);
	}

	

	list<Process>::iterator iter;
	cout << "The data of the process\n";
	for (iter = process_list.begin(); iter != process_list.end(); iter++)
		cout << iter->segment_number << " " << iter->page_number << " " << iter->offset << endl;
	deque<Process> q;
	int count = 0;


	cout << endl << "LRU begin" << endl;
	for (iter = process_list.begin(); iter != process_list.end(); iter++){
		// increase timer
		for_each(q.begin(), q.end(), &increaseTimer);

		if (count < total_number_of_page_frame){
			number_page_fault++;
			if (find_key(q, iter->segment_number) == true){
				cout << "# " << iter->segment_number << " exist #";
			}
			else{
				q.push_back(*iter);
				count++;
			}
		}
		else{
			if (find_key(q, iter->segment_number) == true){
				cout << "# " << iter->segment_number << " exist # ";
			}
			else{
				replaceLRU(q, *iter);
			}
		}

		for (deque<Process>::iterator it = q.begin(); it != q.end(); ++it){
			cout << it->segment_number << " ";
		}
		cout << endl;
	}
	cout << "LRU end" << endl;
	cout << "Number of page fault: " << number_page_fault << endl << endl;


	return 0;
}


// Look ahead x times in the process list, record nearest occurence
void updateOptimum(Process& p, const list<Process>& process_list, list<Process>::iterator iter, int x){
	
	for (int i = 0; i < x; i++){
		if (iter == process_list.end() || p.segment_number == iter->segment_number){ // in case the window goes past the list
			p.next_ref = i;
			return;
		}
		iter++;
	}
	p.next_ref = x;
	return;
}

static bool next_ref_compare(const Process& a, const Process& b){
	return a.next_ref < b.next_ref;
}

void replaceOPT(deque<Process>& q, Process& p){
	// find the OPT, based on the largest next_ref
	deque<Process>::iterator it = max_element(q.begin(), q.end(), next_ref_compare);

	// replace
	*it = p;
	number_page_fault++;
}

// Optimum look-ahead X
int OPT(int x){
	number_page_fault = 0;

	//store all the process
	cout << "Total number of page frame is 3\n";
	cout << "Total number of process is 10\n";
	cout << "The program generate the input by itself, no input output file\n";

	list<Process> process_list;
	for (int i = 0; i < total_number_of_process; i++){
		Process temp;
		temp.segment_number = rand() % (7 - 2 + 1) + 2;
#ifdef MANUAL_INPUT
		temp.segment_number = arr[i];
#endif
		temp.page_number = i + 1;
		temp.offset = i + 2;
		temp.timer = 0;
		process_list.push_back(temp);
	}

	list<Process>::iterator iter;
	cout << "The data of the process\n";
	for (iter = process_list.begin(); iter != process_list.end(); iter++)
		cout << iter->segment_number << " " << iter->page_number << " " << iter->offset << endl;
	deque<Process> q;
	int count = 0;


	cout << endl << "OPT begin" << endl;
	for (iter = process_list.begin(); iter != process_list.end(); iter++){
		// update next_ref
		for (deque<Process>::iterator it = q.begin(); it != q.end(); ++it){
			updateOptimum(*it, process_list, iter, x);
		}


		if (count < total_number_of_page_frame){
			number_page_fault++;
			if (find_key(q, iter->segment_number) == true){
				cout << "# " << iter->segment_number << " exist #";
			}
			else{
				q.push_back(*iter);
				count++;
			}
		}
		else{
			if (find_key(q, iter->segment_number) == true){
				cout << "# " << iter->segment_number << " exist # ";
			}
			else{
				replaceOPT(q, *iter);
			}
		}

		for (deque<Process>::iterator it = q.begin(); it != q.end(); ++it){
			cout << it->segment_number << " ";
		}
		cout << endl;
	}
	cout << "OPT end" << endl;
	cout << "Number of page fault: " << number_page_fault << endl << endl;


	return 0;

}

int main(){
	FIFO();
	LRU();
	OPT(99);

	return 0;
}
