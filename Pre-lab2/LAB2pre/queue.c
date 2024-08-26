// Mark Shinozaki
// 11672355

int enqueue(PROC **queue, PROC *p)  // NOTE **queue
{
  // enter p into queue by priority; FIFO if same priority
  PROC *q = *queue;
  if (q == 0 || p->priority > q->priority)
  {
    *queue = p;
    p->next = q;
  }
  else {
    while(q->next && p->priority <= q->next->priority)
    {
        q = q -> next;
    }
    p->next = q->next;
    q->next = p;
  }

}

// remove and return first PROC from queue 
PROC *dequeue(PROC **queue)         // NOTE **queue 
{
  // remove and return FIRST element in queue
  PROC *p = *queue;
  if (p)
  {
    *queue = (*queue) ->next;
  }
  return p;
}

int printList(char *name, PROC *p)
{
    printf("%s = ", name);
    while(p)
    {
        printf("[%d %d] ->",p->pid,p->priority);
        p = p->next;
    }
    printf("NULL\n");
}

/*
name = "listname"
what = 0: print [p->pid p->priority],
what = 1: print [p->pid p->event],
what = 2: print [p->pid p->status as STRING]
*/