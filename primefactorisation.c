//C CODE TO GENERATE AND RENDER A PRIME FACTOR TREE, THE RENDERING SUCKS BUT IS WORKABLE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct tree_node{
	int left;
	int right;
	struct tree_node* next;
} node_t;

int isPrime(long num){
	for(long i = 2; i < num-1; i++){
		if (i != num){
			if(num%i == 0)
				return 0;
		}
	}
	return 1;
}

node_t* primeGen(long num){
	for(long i = 2; i < (num-1/2); i++){
		if(isPrime(i)){
			if(num%i == 0){
				node_t* out = calloc(1,sizeof(node_t));
				out->left = i;
				out->right = num/i;
				out->next = NULL;  
				return out; // Where the earlier node is, the DEFINITIVE prime, the divisor, next node from right branch
			}
		}
	}
}

node_t* treeGen(long num){
	if(isPrime(num)){
		node_t* out = calloc(1,sizeof(node_t));
		out->left = num;
		out->right= 1;
		out->next = NULL; // is start and end of tree
		return out;
	}
	else{
		node_t* tree = calloc(1,sizeof(node_t));
		size_t treeSize = 1;
		long lastRight = primeGen(num)->right;
		tree[0].left = primeGen(num)->left;
		tree[0].right = primeGen(num)->right;
		tree[0].next = NULL;
		while(!isPrime(lastRight)){
			tree = realloc(tree, (++treeSize*sizeof(node_t)));
			tree[treeSize-1].left = primeGen(lastRight)->left;
			tree[treeSize-1].right = primeGen(lastRight)->right;
			tree[treeSize-1].next = NULL;
			tree[treeSize-2].next = &tree[treeSize-1];
			lastRight = tree[treeSize-1].right;
		}
		return tree;
	}
}

void printTree(node_t* tree){
	int treeLen = 0;
	for(; tree[treeLen].next; treeLen++);
	for(int j = 0; j<(((treeLen+1)/3+2));j++)
                putchar(' ');
        printf("%d", tree[0].left*tree[0].right);
        for(int j = 0; j<(((treeLen+1)/3)+1);j++)
                putchar(' ');
        putchar('\n');
	for(int i = 0; i<=treeLen; i++){
		for(int j = 0; j<(((treeLen+1)/3)+1+(i*3));j++)
		        putchar(' ');
		printf("%d", tree[i].left);
		for(int j = 0; j<(((treeLen+1)/3)+1);j++)
		        putchar(' ');
		printf("%d",tree[i].right);
		putchar('\n');
	}
}

void freeList(node_t* head){
	node_t* tmp;
	while (head != NULL){
		tmp = head;
		head = tmp->next;
		free(tmp);
	}
}


int main(){
	long input;
	scanf("%ld", &input);
	node_t* tree = treeGen(input);
	printTree(tree);
	int i = 0;
	for(;tree[i].next;i++);
	for(;i>1;free(tree[i--].next));
	free(tree);
}
