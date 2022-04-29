#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct branch {
    int val;
    struct branch* l;
    struct branch* r;
} branch_t;

branch_t* makeBranch(int val){
    branch_t* new = calloc(1,sizeof(branch_t));
    new->val = val;
    return new;
}

void addBranchVal(int val, char lor, branch_t* parent){ // lor: 0 = right, lor: 1 = left; parent = the branch where the new branch is being added
    if(lor == 0){
        parent->r = makeBranch(val);
    } else {
        parent->l = makeBranch(val);
    }
}

void addBranchPtr(branch_t* val, char lor, branch_t* parent){ // lor: 0 = right, lor: 1 = left; parent = the branch where the new branch is being added
    if(lor == 0){
        parent->r = val;
    } else {
        parent->l = val;
    }
}

branch_t* accessBranch(char* accString, branch_t* root){ // accString: "01010" = right, left, right, left, right; Must be NULL terminated
    branch_t* curr = root;
    for (int i = 0; i<strlen(accString); i++){
        if(accString[i]-'0' == 0){ // right
            curr = curr->r;
        } else if(accString[i]-'0' == 1) {
            curr = curr->l;
        }
    }
    return curr;
}

void freeTree(branch_t* root){ // recursive function to clear a branch and passes the next branches
    branch_t* left = root->l;
    branch_t* right = root->r;
    free(root);
    if(left){
        freeTree(left);
    }
    if(right){
        freeTree(right);
    }
}

void invertTree(branch_t* root){ // recursive function to swap l <-> r
    branch_t* l = root->l;
    root->l = root->r;
    root->r = l;
    if(root->r){
        invertTree(root->r);
    }
    if(root->l){
        invertTree(root->l);
    }
}

#endif
