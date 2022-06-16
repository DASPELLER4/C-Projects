//C CODE TO GENERATE AND RENDER A PRIME FACTOR TREE, THE RENDERING SUCKS BUT IS WORKABLE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct tree_node{
	int left;
	int right;
	int length;
} node_t;

int isPrime(int num){
	for(int i = 2; i < num-1; i++){
		if (i != num){
			if(num%i == 0)
				return 0;
		}
	}
	return 1;
}

node_t primeGen(int num){
	for(int i = 2; i < (num-1/2); i++){
		if(isPrime(i)){
			if(num%i == 0){
				return (node_t) {i,num/i}; // Where the earlier node is, the DEFINITIVE prime, the divisor, next node from right branch
			}
		}
	}
}

int getStepCount(int num){
	int steps = 0;
	while(!isPrime(num))
		for(int i = 2; i < (num-1/2); i++)
	        	if(isPrime(i))
        	        	if(num%i == 0){
					num /= i;
					steps++;
				}
	return steps;
}

node_t* treeGen(int num){
	if(isPrime(num)){
		node_t* out = calloc(1,sizeof(node_t));
		out[0].left = num;
		out[0].right= 1;
		out[0].length = 0;
		return out;
	}
	else{
		int steps = getStepCount(num);
		node_t* tree = calloc(steps,sizeof(node_t));
		int last = 1;
		node_t startGen = primeGen(num);
		int lastRight = startGen.right;
		tree[0].left = startGen.left;
		tree[0].right = startGen.right;
		tree[0].length = steps;
		while(!isPrime(lastRight)){
			last++;
			node_t currGen = primeGen(lastRight);
			tree[last-1].left = currGen.left;
			tree[last-1].right = currGen.right;
			tree[last-1].length = steps;
			lastRight = tree[last-1].right;
		}
		return tree;
	}
}

void printTree(node_t* tree){
	int treeLen = tree[0].length-1;
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

int main(){
	int input;
	scanf("%d", &input);
	node_t* tree = treeGen(input);
	printTree(tree);
	free(tree);
}
