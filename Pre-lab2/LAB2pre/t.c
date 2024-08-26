/*********** A Multitasking System ************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "type.h"
#include "queue.c"  // comment in to use YOUR queue.c file
#include "wait.c"

struct node
{
  int key;
  struct node *left, *right;
};

// global variables
PROC proc[NPROC];   // 9 PROCs
PROC *freeList;     // FREE proc list
PROC *readyQueue;   // priority queue of READY procs
PROC *sleepList;    // list of SLEEP procs
PROC *running;      // pointer to current RUNNING proc

int body()
{
  char command[64];
  while(1){
    printf("***************************************\n");

    printf("P%d running: Parent=%d\n", running->pid, running->ppid);
    
    printFreeList(freeList);
    printReadyQueue(readyQueue);
    
    printf("input a command: [ps|fork|switch] | [sleep|wakeup|exit|wait] : ");

    fgets(command, 64, stdin);
    command[strlen(command)-1] = 0; // kill \n at end
     
    if (!strcmp(command, "ps"))
      ps();
    else if (!strcmp(command, "fork"))
      kfork(body, 1);
    else if (!strcmp(command, "switch"))
      tswitch();
    else
      printf("invalid command\n");
  }
}










int kfork(int (*func), int priority)
{
  struct node *root = NULL;
  PROC *p;
  int  i;
  /*** get a proc from freeList for child proc: ***/
  p = dequeue(&freeList);
  if (!p){
     printf("no more proc\n");
     return -1;
  }

  /* initialize the new proc and its stack */
  p->status = READY;
  p->priority = priority;     // for ALL PROCs except P0
  p->ppid = running->pid;
  p->parent = running;
  p->child = 0; p->sibling = 0;
  
  //                    -1   -2  -3  -4  -5  -6  -7  -8   -9
  // kstack contains: |retPC|eax|ebx|ecx|edx|ebp|esi|edi|eflag|
  //modify BST
 for (i=1; i<10; i++)
    p->kstack[SSIZE - i] = 0;
    p->kstack[SSIZE-1] = (int)func;
    p->saved_sp = &(p->kstack[SSIZE - 9]);

  enqueue(&readyQueue, p);
  //printChildList(p); // dont know if this is the correct implementation for part 4
  
  return p->pid;
}



void printChildList(PROC *p) {
  printf("childList = [");
  while (p) {
    printf("pid %d status %d", p->pid, p->status);
    p = p->sibling;
    if (p) {
      printf(" -> ");
    }
  }
  printf("]\n");
}


int init()
{
  int i;
  PROC *p;
  
  // 1. all PROCs in freeList
  for (i = 0; i < NPROC; i++){
    p = &proc[i];
    p->pid = i; 
    p->status = FREE;
    p->priority = 0;
    p->next = p+1;
  }
  proc[NPROC-1].next = 0;
 
  freeList = &proc[0];        
  readyQueue = 0;

  // 2. create P0 as the initial running process
  running = dequeue(&freeList);
  running->status = READY;
  running->priority = 0;     // P0 has lowest prioriy 0
  running->parent = running;
  //running->child = 0; running->sibling = 0;
  
  printFreeList(freeList);
  printf("init complete: P0 running\n"); 
}


/*************** main() ***************/
int main()
{
   printf("Welcome to 360 Multitasking System\n");
   init();
   printf("P0 fork P1\n");
   kfork(body, 1);  

   while(1){
     if (readyQueue){
        printf("P0: switch task\n");
        tswitch();
     }
   }
}

int do_exit()
{
  int value;
  PROC *p;
  if (running->pid==1){
      printf("P1 never dies\n");
      return 0;
  }
  printf("proc %d in do_exit(), enter an exit value : ", running->pid);
  scanf("%d", &value);
  kexit(value);
} 

//Create new node in BST
struct node *newNode(int item)//Creates new BST node
{
  struct node *temp = (struct node *)malloc(sizeof(struct node));
  temp->key = item;
  temp->left = temp->right = NULL;
  return temp;
}
struct node* insert(struct node* node, int key)//insert node in BST
{
  if(node == NULL)//check if BST is empty
    return newNode(key);//create a new node if NULL and return

  if(key < node->key)//check if key is less than node
    node->left = insert(node->left, key);//insert key left of node

  else if(key > node->key)//check if key is greater than node
    node->right = insert(node->right, key);//insert key right of node
  return node;
}
struct node* inorder(struct node *root)
{
  if(root != NULL)
  {
    inorder(root->left);
    printf("test: ");
    printf("%d \n", root->key);
    inorder(root->right);
  }
}

/*********** scheduler *************/
int scheduler()
{ 
  printf("proc %d in scheduler()\n", running->pid);
  if (running->status == READY)
      enqueue(&readyQueue, running);
  printReadyQueue(readyQueue);
  running = dequeue(&readyQueue);
  printf("next running = %d\n", running->pid);  
}


char *pstatus[]={"FREE   ","READY  ","SLEEP  ","BLOCK  ","ZOMBIE", "RUNNING"};

int ps()
{
  int i; PROC *p;
  printf("pid   ppid    status\n");
  printf("--------------------\n");
  for (i=0; i<NPROC; i++){
     p = &proc[i];
     printf(" %d      %d     ", p->pid, p->ppid);
     if (p==running)
       printf("%s\n", pstatus[5]);
     else
       printf("%s\n", pstatus[p->status]);
  }
}

struct node *newNode(int item)//Creates new BST node
{
  struct node *temp = (struct node *)malloc(sizeof(struct node));
  temp->key = item;
  temp->left = temp->right = NULL;
  return temp;
}
struct node* insert(struct node* node, int key)//insert node in BST
{
  if(node == NULL)//check if BST is empty
    return newNode(key);//create a new node if NULL and return

  if(key < node->key)//check if key is less than node
    node->left = insert(node->left, key);//insert key left of node

  else if(key > node->key)//check if key is greater than node
    node->right = insert(node->right, key);//insert key right of node
  return node;
}
struct node* inorder(struct node *root)
{
  if(root != NULL)
  {
    inorder(root->left);
    printf("test: ");
    printf("%d \n", root->key);
    inorder(root->right);
  }
}