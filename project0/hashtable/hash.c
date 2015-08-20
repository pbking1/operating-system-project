#include<stdio.h>
#include<stdlib.h>

int hash_function(int key){
	return ((key*4) % 10);
}

void inittable(int a[10]){
	for(int i = 0; i < 10; i++){
		a[i] = 0;
	}
}

void insert(int a[10]){
	int res_array[5] = {1, 2, 3, 4};
	for(int i = 0; i < 4; i++){
		int index = hash_function(res_array[i]);
		a[index] = res_array[i];
	}
}

void delete(int a[10], int key){
	for(int i = 0; i < 10; i++){
		if(a[i] == key)
			a[i] = 0;
	}
}

void search(int a[10], int key){
	for(int i = 0; i < 10; i++){
		if(a[i] == key)
			printf("find the recourse %d, process %d is using it\n",  a[i], i + 1);

	}
}

void print_table(int a[10]){
	for(int i = 0; i < 10; i++){
		printf("The process %d is using the resource %d\n", i+1, a[i]);
	}
	printf("\n");
}

int main(){
	int process_num = 10;
	int hashtable[10];
	inittable(hashtable);
	//allocate resource to processes
	insert(hashtable);
	print_table(hashtable);
	
	//search key in table
	search(hashtable, 1);

	//delete recource 
	delete(hashtable, 1);
	print_table(hashtable);
	return 0;
}
