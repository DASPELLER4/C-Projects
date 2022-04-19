#ifndef LINKED_H
#define LINKED_H
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int val;
	struct node* next;
} node_t;

node_t* makeNode(int value){
	node_t* ret = malloc(sizeof(node_t));
	ret->val = value;
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

void appendVal(int value, node_t* head){
	node_t* oldLast = accessIndex(listLen(head)-1,head);
	oldLast->next = makeNode(value);
}

void appendNode(node_t* new, node_t* head){
	node_t* oldLast = accessIndex(listLen(head)-1,head);
	oldLast->next = new;
}

node_t* deleteNode(int index, node_t* head){ // return the head of the list
	node_t* prev = accessIndex(index-1, head);
	node_t* curr = accessIndex(index, head);
	node_t* ret = curr;
	if(index != 0){
		if(curr->next != NULL){
			prev->next = curr->next;
			ret = head;
		} else {
			prev->next = NULL;
			ret = head;
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

void reverseList(node_t** head){
	node_t* curr = *head;
	node_t* prev = NULL;
	node_t* next = NULL;
	while(curr){
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	*head = prev;
}

void deleteList(node_t* head){
	int len = listLen(head);
	node_t* curr = head;
	while(curr != NULL){
		curr = deleteNode(0,curr);
	}
}

node_t* insertNode(int index, int val, node_t* head){ // incase the inserted node is now first
	if (index == 0){
		node_t* newHead = makeNode(val);
		newHead->next = head;
		return newHead;
	} else {
		node_t* prev = accessIndex(index-1,head);
		node_t* next = accessIndex(index,head);
		prev->next = makeNode(val);
		prev->next->next = next;
		return head;
	}
}

node_t* copyList(node_t* head){
	node_t* newHead = makeNode(head->val);
	node_t* prev = newHead;
	node_t* curr = head->next;
	while (curr){
		prev->next = makeNode(curr->val);
		prev = prev->next;
		curr = curr->next;
	}
	return newHead;
}

#endif
