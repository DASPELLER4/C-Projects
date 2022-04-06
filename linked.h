#ifndef LINKED_H
#define LINKED_H
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	struct node* prev; // The prev pointer is only here so you don't need to keep track of the head only, but can also use any element
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

node_t* findHead(node_t* curr){
	node_t* current = curr;
	while(current->prev != NULL){
	        current = current->prev;
	}
	return current;
}

node_t* accessIndex(int index, node_t* head){
	node_t *curr = findHead(head);
	for(int i = 0; i < index; i++){
		curr=curr->next;
		if (curr == NULL){
			return NULL;
		}
	}
	return curr;

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
		deleteNode(accessIndex(listLen(head)-1,findHead(head)));
	}
}

void insertNode(int index, int val, node_t* head){
        node_t* prev = accessIndex(index-1,head);
        node_t* next = accessIndex(index,head);
        prev->next = makeNode(val);
        prev->next->prev = prev;
        prev->next->next = next;
        next->prev = prev->next;
}

// To create a node, like to initialize a list, you can use makeNode
// To append values to the list, select a node in the list and use appendNode
// To get the pointer to an index of a list, select a node, the index and use accessIndex
// To delete a node, provide it's pointer to deleteNode (this is like the pop function)
// To "push" a node, provide it's index, value, and the head of the list to insertNode
// To reverse a list, provide a pointer that is within the list to reverseList
// To clear a list and free all it's pointers, provide a pointer that is within th list to deleteList

#endif
