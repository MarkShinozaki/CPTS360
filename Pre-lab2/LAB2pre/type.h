/********* type.h ***********/
#define NPROC     9
#define SSIZE  1024

#define FREE      0        // proc status
#define READY     1 
#define SLEEP     2
#define BLOCK     3
#define ZOMBIE    4

typedef struct proc{
    struct proc *next;      // next proc pointer
    int  *saved_sp;         // at offset 4: do NOT alter
 
    int   pid;              // pid = 0 to NPROC-1
    int   ppid;             // parent pid 
    int   status;           // PROC status: FREE|READY|etc 
    int   priority;         // scheduling priority 

    int   event;            // event to sleep on 
    int   exitCode;         // exit code value

    struct proc *child;     // first child PROC pointer
    struct proc *sibling;   // sibling PROC pointer
    struct proc *parent;    // parent PROC pointer

    int   kstack[SSIZE];    // processs stack                 
}PROC;

