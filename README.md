# C-Projects
Small C projects

# Docs
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

# ISSUES
<h3>prime factorisation</h3>
IOT error, might fix later
 
<h3>Minesweeper</h3>
Have to edit code to give new values
