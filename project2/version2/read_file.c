#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<pthread.h>
#include<ctime>
#include<unistd.h>
#include<algorithm>
using namespace std;

struct thread_info{
	int id;
	int reader_or_writer;
	int resource_id;
	int arrive_t;
	int burst_t;
};

int main(){
	//read file
	struct thread_info a[10];
	int number;
	FILE *fin;
	if((fin = fopen("input.1", "r")) == NULL){
	//read file fail
		printf("read input file failed, please modify you input as input.1 format and name\n");
		exit(0);
	}
	fscanf(fin, "%d", &number);
	for(int i = 0; i < number; i++){
		a[i].id = a[i].resource_id = a[i].reader_or_writer = a[i].arrive_t = a[i].burst_t = 0;
	}
	for(int i = 0; i < number; i++){
		fscanf(fin, "%d %d %d %d %d\n", &a[i].id, &a[i].reader_or_writer, &a[i].resource_id, &a[i].arrive_t, &a[i].burst_t);
	}
	fclose(fin);
	for(int i = 0; i < number; i++){
		printf("%d %d %d %d %d\n", a[i].id, a[i].reader_or_writer, a[i].resource_id, a[i].arrive_t, a[i].burst_t);
	}
	
	return 0;
}
