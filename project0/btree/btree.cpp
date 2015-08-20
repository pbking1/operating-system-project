#include<iostream>
#include<string>
#include<cstdio>
#include<cstdlib>
using namespace std;

#define QueueMaxSize 20
#define StackMaxSize 10
typedef int ElementType;
struct Node{
	ElementType data;
	struct Node *left;
	struct Node *right;
};

//recursive find
ElementType *find(struct Node *root, ElementType x){
	if(root == NULL)
		return NULL;
	else{
		if(root -> data == x)
			return &(root -> data);
		else if(x < root -> data)
			return find(root -> left, x);
		else
			return find(root -> right, x);
	}
}

//non recursive find
ElementType *find_non_rec(struct Node *root, ElementType x){
	while(root != NULL){
		if(root -> data == x)
			return &(root -> data);
		else if(root -> data > x)
			root = root -> left;
		else
			root = root -> right;
	}
	return NULL;
}

//recursive insert
void insert(struct Node **root, ElementType x){
	if(*root == NULL){
		struct Node *p = (Node *)malloc(sizeof(struct Node));
		p -> data = x;
		p -> left = p -> right = NULL;
		*root = p;
	}else if((*root) -> data > x){
		insert(&((*root) -> left), x);
	}else{
		insert(&((*root) -> right), x);
	}
}

//non recursive insert
void insert_non_rec(struct Node **root, ElementType x){
	struct Node *p;
	struct Node *t = *root, *parent = NULL;
	//find the insert location
	while(t != NULL){
		parent = t;
		if(t -> data < x)
			t = t -> right;
		else
			t = t -> left;
	}
	p = (Node *)malloc(sizeof(struct Node));
	p -> data = x;
	p -> left = p -> right = NULL;
	if(parent == NULL)
		*root = p;
	else if(parent -> data > x)
		parent -> left = p;
	else
		parent -> right = p;
}

void create_btree(struct Node **root, ElementType a[], int n){
	*root = NULL;
	for(int i = 0; i < n; i++){
		insert(root, a[i]);
	}
}

int del(struct Node **root, ElementType x){
	struct Node *temp;
	temp = *root;
	//if the tree is empty, then return 0
	if(*root == NULL)
		return 0;
	//if the data is smaller than the root, delete the left
	if(x < (*root) -> data)
		return del(&((*root) -> left), x);
	//if the data is bigger than the root, delete the right
	else if(x > (*root) -> data)
		return del(&((*root) -> right), x);
	//if the value to be deleted is tha same as root and the left tree is empty 
	if((*root) -> left == NULL){
		*root = (*root) -> right;
		free(temp);
		return 1;
	}
	//if the value to be deleted is the same as root and the right tree is empty
	else if((*root) -> right == NULL){
		*root = (*root) -> left;
		free(temp);
		return 1;
	}
	else{
	//if the value to be deleted is the same as root and the right and left tree are not empty
	//so we need to find the smallest point under the root
		if((*root) -> left -> right == NULL){
			(*root) -> data = (*root) -> left -> data;
			return del(&((*root) -> left), (*root) -> data);
		}else{
			struct Node *p = *root, *p1 = p -> left;
			while(p1 -> right != NULL){
				p = p1;
				p1 = p1 -> right;
			}
			(*root) -> data = p1 -> data;
			return del(&((*root) -> right), p1 -> data);
		}
	}

}

int resource[4];
int i = 0;
void preorder(struct Node* root){
	if(root != NULL){
		if(i < 4){
			resource[i] = root -> data;
			i++;
		}
		cout<<root -> data<<" ";
		preorder(root -> left);
		preorder(root -> right);
	}
}

void inorder(struct Node *root){
	if(root != NULL){
		inorder(root -> left);
		cout<<root -> data<<" ";
		inorder(root -> right);
	}
}

void postorder(struct Node *root){
	if(root != NULL){
		postorder(root -> left);
		postorder(root -> right);
		cout<<root -> data<<" ";
	}
}

void print_tree(struct Node *root){
	if(root != NULL){
		printf("%d", root -> data);
		if(root -> left != NULL || root -> right != NULL){
			printf("(");
			print_tree(root -> left);
			if(root -> right != NULL)
				printf(",");
			print_tree(root -> right);
			printf(")");
		}
	}
}

void clear(struct Node **root){
	if(*root != NULL){
		clear(&(*root) -> left);
		clear(&(*root) -> right);
		free(*root);
		*root = NULL;
	}
}

int main(){
	ElementType processes[9] = {1,2,3,4,5,6,7,8,9};
	struct Node *root = NULL;
	
	//create tree
	create_btree(&root, processes, 9);
	cout<<"print the processes tree";
	print_tree(root);

	//search tree
	ElementType key;
	ElementType *p;
	cout<<"enter the process you want to search"<<endl;
	cin>>key;
	p = find(root,key);
	if(p){
		cout<<"Find the process "<<*p<<endl;
	}else{
		cout<<"The process is not exist"<<endl;
	}

	//insert process
	cout<<"please enter the new process to insert"<<endl;
	cin>>key;
	insert(&root, key);
	print_tree(root);

	//delete the process
	cout<<"delete the process 1"<<endl;
	if(del(&root, 1) == 0)
		cout<<"The tree is empty\n";
	else
		cout<<"delete success\n";
	print_tree(root);

	//insert back the process 1
	insert(&root, 1);

	//traverse the tree
	cout<<endl<<"Pre order"<<endl;
	preorder(root);
	cout<<endl<<"In order"<<endl;
	inorder(root);
	cout<<endl<<"Post order"<<endl;
	postorder(root);

	cout<<endl;
	cout<<"allocate resource in preorder traverse"<<endl;
	for(int i = 0; i < 4; i++)
		cout<<"Resource "<<resource[i]<<" is allocate to process "<<i + 1<<endl;
	cout<<endl;
	//release the tree
	clear(&root);
	return 0;
}

























