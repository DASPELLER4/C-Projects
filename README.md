# C-Projects
Small C projects

# Docs
<h3>2d Renderer</h3>
Once you compile it, you run it with the argument being the filename of the shape and the next being a single character, a shape is encoded as such

v<sub>1</sub> v<sub>2</sub> v<sub>3</sub> c<br>
v<sub>1</sub> v<sub>2</sub> v<sub>3</sub> c<br>...


Where v is the vertecies of each 3 sided polygon and c is the color (an alphabetical or symbolic(!@#$%^&*...) character) which is what the shape will be rendered as

<h3>Linked Library</h3>
To first create a node to act as a head use

    node_t* head = makeNode(VAL);
To append nodes to a list (head) use

    appendNode(NODE, head); // when you have the node pointer
    appendVal(VAL, head); // when you have the value
To access an index of a list (head) use

    node_t* the_node_you_want = accessIndex(INDEX, head);
To access the last node you need the length, which can be found with

    int length = listLen(head);
To insert a node and make it the index use

    head = insertNode(INDEX, VAL, head);
here we are assigning head to the return in case that the new insert is the new head of the list so the head is now truly the start THIS IS REQUIRED, NOT DOING SO MAY CAUSE A MEMORY LEAK

To delete a node at an index use

    head = deleteNode(INDEX, head);
Once again we're assigning the head incase the head was deleted

To reverse a list use

    reverseList(&head);
Ensure to use the memory address of the head in this function

To print a list use

    printList(head);

To create a list, from another list, seperate from that list use

    node_t* copy = copyList(head);

<h3>Binary Tree Library</h3>
To first create a branch to act as a root use

    branch_t* head = makeBranch(VAL);
To add branches to a parent branch use

    // left or right: l = 1, r = 0
    addBranchPtr(BRANCH, leftOrRight, parent); // when you have the branch pointer
    addBranchVal(VAL, leftOrRight, parent); // when you have the value
To access a branch of a tree use

    // NAV is a string where every char navigates you down so "01" is right then left
    branch_t* the_branch_you_want = accessBranch(NAV, root);
To invert a tree use

    invertTree(root);
To free a tree use

    freeTree(root);
