#include<stdio.h> //tells the compiler to insert the contents of stdio at that particular place, like standard input and output such as printf scanf.
#include<unistd.h> //standard header file that provides access to POSIX (portable operating system interface) API
#include<stdlib.h> // contains declaration of malloc() and free(). header file for standard library.

 typedef int bool;

 typedef struct node{ // typedef name doesn't become visible until the end of the declaration. If the structure contains a pointer to itself, you have use the struct version to refer to it.
	int processID;
  	int timeRemaining;
  	struct node *next;
 }node;

 bool isEmpty(node *end){ //returns true if list is empty, false otherwise
 	return (end == NULL);
 }

 void printList(node *end){ //prints contents of data stored in each node
	if(isEmpty(end))
		return;
	
	node *ptr = end->next;  

    while(ptr != end) { /* continue whilst there are nodes left */
      printf("Process %d: %d seconds left\n",ptr->processID,ptr->timeRemaining);
      ptr = ptr->next;   /* goto the next node in the list       */

    }

 printf("Process %d: %d seconds left\n",ptr->processID,ptr->timeRemaining);

 printf("\n");
 }

 node* newNode(int processID,int exeTime) { //adds new node 
	node *newnode = (node*)malloc(sizeof(node));
	
	newnode->processID = processID;
	newnode->timeRemaining = exeTime;
	newnode->next = NULL;

	return newnode;
 }

 void add(int processID,int exeTime,node **end) {

	node *newnode = newNode(processID,exeTime); //uses newNode to add new node
	if((*end) == NULL){ 
		newnode->next = newnode;
		(*end) = newnode;
	} else {
		newnode->next = (*end)->next;
		(*end)->next = newnode;
		(*end) = newnode;
	}
}

 /* creates linked list from data of test1.txt file,
    by executing like this: gcc ____.c -0 ____ and 
    ./_____<test1.txt */
 node* createList(){ 
	node *end=NULL;

  	while(1){
		int id,t;
		if(scanf("%d%d",&id,&t) == EOF)
			break;

		add(id,t,&end);
   }
   return end;
 }

 /* Using current and end pointer, remove process when 
    process execution time is done. */
 node* removeCurrent(node *curr, node **end){ 

  if(curr == NULL)
	return NULL;

  if(curr->timeRemaining != 0)
    return curr->next;

  printf("Process %d has executed.\n", curr->processID);

  if(curr->next == curr){
    free(curr);
    return NULL;
  }

 node *temp = curr;

 node *parent_temp = temp->next;

 while(parent_temp->next != temp)
	parent_temp = parent_temp->next;

  	parent_temp->next = temp->next;
 if(curr == (*end)) {
	 (*end) = curr->next;
  }

  curr = curr->next;
  free(temp);
  return curr;
 }

 /* scans linked list every 1 second, receives
    end pointer to start scanning */
 node* scanList(node *end){

  if(end==NULL)
    return NULL;

  printList(end);

  node *curr=end->next;

  while(curr!=NULL){
    sleep(1); //sleep for one second
    (curr->timeRemaining) = (curr->timeRemaining)-1;
    curr = removeCurrent(curr,&end);

    if(curr == NULL)
       end = NULL;

    printList(end);
  }

 return NULL;
 }

 int main() {
 	node *end = createList();
  	printf("\n");

    printList(end);
    printf("\n");

    end = scanList(end);
  	return 0;
}
