 #include<stdio.h>
 #include<unistd.h>
 #include<stdlib.h>

 typedef int bool;

 typedef struct node{
  int id;
  int time_left;
  struct node *next;
 }node;

node* newNode(int pid,int exec_time) {

  node *newnode=(node*)malloc(sizeof(node));

  newnode->id=pid;

  newnode->time_left=exec_time;

  newnode->next=NULL;

  return newnode;

 }

bool isEmpty(node *end){

  return (end==NULL);

}

void printList(node *end){

  if(isEmpty(end))return;

    node *ptr=end->next;

    while(ptr!=end){

      printf("Process %d\t %d seconds left\n",ptr->id,ptr->time_left);

      ptr=ptr->next;

    }

printf("Process %d\t %d seconds left\n",ptr->id,ptr->time_left);

printf("\n");

}

void add(int pid,int exec_time,node **end){

node *newnode=newNode(pid,exec_time);

if((*end)==NULL){

newnode->next=newnode;

(*end)=newnode;

}else{

newnode->next=(*end)->next;

(*end)->next=newnode;

(*end)=newnode;

}

}

node* createList(){

node *end=NULL;

  while(1){

    int id,t;

    if(scanf("%d%d",&id,&t)==EOF)break;

      add(id,t,&end);

   }

return end;

}

node* removeCurrent(node *curr,node **end){

  if(curr==NULL)return NULL;

  if(curr->time_left!=0)
    return curr->next;

  printf("Process %d has executed.\n",curr->id);

  if(curr->next==curr){

    //only process

    free(curr);

    return NULL;

  }

node *temp=curr;

node *parent_temp=temp->next;

while(parent_temp->next!=temp)parent_temp=parent_temp->next;

  parent_temp->next=temp->next;

  if(curr==(*end)) {

    (*end)=curr->next;

  }

  curr=curr->next;

  free(temp);

  return curr;

 }

node* scanList(node *end){

  if(end==NULL)
    return NULL;

  printList(end);

  node *curr=end->next;

  while(curr!=NULL){

    sleep(1);//sleep for one second

    (curr->time_left) = (curr->time_left)-1;

    curr = removeCurrent(curr,&end);

    if(curr==NULL)
       end=NULL;

    printList(end);

  }

return NULL;

}

int main() {

  node *end=createList();

  printf("\n");

  printList(end);

  printf("\n");

  end=scanList(end);

  return 0;

}

