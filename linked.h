#ifndef LINKED_H
#define LINKED_H
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	struct node* prev;
	int val;
	struct node* next;
} node_t;

node_t* makeNode(int value){
	node_t* ret = malloc(sizeof(node_t));
	ret->val = value;
	ret->prev = NULL;
	ret->next = NULL;
	return ret;
}

int listLen(node_t* head){
	node_t* curr = head;
	int len = 1;
	while(curr->next != NULL){
		curr = curr->next;
		len++;
	}
	return len;
}

node_t* accessIndex(int index, node_t* head){
	node_t *curr = head;
	for(int i = 0; i < index; i++){
		curr=curr->next;
		if (curr == NULL){
			return NULL;
		}
	}
	return curr;

}

node_t* findHead(node_t* curr){
	node_t* current = curr;
	while(current->prev != NULL){
	        current = current->prev;
	}
	return current;
}

void appendNode(int value, node_t* head){
	node_t* oldLast = accessIndex(listLen(head)-1,head);
	oldLast->next = makeNode(value);
	oldLast->next->prev = oldLast;
}

node_t* deleteNode(node_t* curr){ // return the head of the list
	node_t* ret = curr;
	if(curr->prev != NULL){
		if(curr->next != NULL){
			curr->prev->next = curr->next;
			curr->next->prev = curr->prev;
			ret = findHead(curr);
		} else {
			curr->prev->next = NULL;
			ret = findHead(curr);
		}
		free(curr);
	} else {
		if(curr->next != NULL){
			ret = curr->next;
			free(curr);
		} else {
			free(curr);
			ret = NULL;
		}
	}
	return ret;
}

void printList(node_t* head){
	head = findHead(head);
	node_t* curr = head;
	putchar('[');
	while(curr != NULL){
		printf("%d", curr->val);
		if(curr->next){
			putchar(',');
		}
		curr = curr->next;
	}
	putchar(']');
	putchar('\n');
}

void reverseList(node_t* head){
	head = findHead(head);
	int index = 0;
	for(index = 0; index<listLen(head)-1; index++){
		int f = accessIndex(index,head)->val;
		accessIndex(index,head)->val = accessIndex(listLen(head)-1-index,head)->val;
		accessIndex(listLen(head)-1-index,head)->val = f;
	}
}

void deleteList(node_t* head){
	int len = listLen(head);
	for(int i = 0; i < len; i++){
		deleteNode(accessIndex(listLen(head)-1,head));
	}
}

int main(){
	node_t* head = makeNode(8);
	appendNode(7,head);
	appendNode(6,head);
	printf("Made list of length: %d, Containing:\n",listLen(head));
	printList(head);
	deleteNode(accessIndex(listLen(head)-2,head));
	printf("Deleted the second element\nList has now length: %d, Containing:\n",listLen(head));
	printList(head);
	reverseList(head);
	printf("Reversed List\nList has length: %d, Containing:\n",listLen(head));
	printList(head);
	deleteList(head);
	return 0;
}
#endif
