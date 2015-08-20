#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef int datatype;

typedef struct Node{
	datatype pro_name;
	datatype res_name;
	struct Node *next;
}linkedlist;

linkedlist *createlist(){
	linkedlist *head, *new, *tail;
	int process;
	head = (linkedlist*)malloc(sizeof(linkedlist));
	if(head == NULL){
		printf("error\n");
		exit(1);
	}
	tail = head;
	for(int i = 1; i <= 10; i++){
		new = (linkedlist*)malloc(sizeof(linkedlist));
		if(new == NULL){
			printf("error\n");
			exit(1);
		}
		new -> pro_name = i;
		new -> res_name = 0;
		tail -> next = new;
		tail = new;
	}
	//let the tail pointer point to the head
	//make the list circular
	tail -> next = head;
	return head;

}

void insertlist(linkedlist *head, int processor_name, int resource_name){
	linkedlist *p;
	p = head;
	while(p != NULL){
		if(p -> pro_name == processor_name){
			p -> res_name = resource_name;
			break;
		}else{
			p = p -> next;
		}
	}

}

void printlist(linkedlist *head){
	linkedlist *p;
	p = head -> next;
	int number = 0;
	while(number < 10){
		printf("The process name is %d, The resource name is %d\n", p -> pro_name, p -> res_name);
		p = p -> next;
		number ++;
	}
}

void searchlist(linkedlist *head, int key){
	linkedlist *p;
	p = head;
	int number = 0;
	while(number < 10){
		if(p -> res_name == key){
			printf("The processor %d is using the resource %d", p -> pro_name, key);
			printf("\n");
			break;
		}
		else
			p = p -> next;
		number++;
	}
}

void del(linkedlist *head, int res){
	linkedlist *p, *q;
	p = head;
	int number = 0;
	while(number < 10){
		if(p -> res_name == res){
			p -> res_name = 0;
			break;
		}else{
			p = p -> next;
		}
		number ++;
	}
}

void freelist(linkedlist *head){
	linkedlist *p;
	int number = 0;
	while(number < 10){
		p = head;
		head = head -> next;
		free(p);
		number ++;
	}
}

int main(){
	linkedlist *head;
	head = createlist();
	printf("create list complete\n");
	printlist(head);
	printf("print the origin list complete\n");
	
	//allocate resource
	insertlist(head, 1, 2);
	insertlist(head, 2, 1);
	insertlist(head, 6, 3);
	insertlist(head, 9, 4);

	//print the result state of the resource allocate
	printlist(head);

	//find the which processor are using particular resource
	searchlist(head, 3);

	//delete resource from processor
	del(head, 1);

	//print the list after delete the resource from processor
	printlist(head);
	freelist(head);
	return 0;
}
