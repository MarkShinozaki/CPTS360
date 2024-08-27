# Exam Material 

## Exam 1 - Review Questions

### Question 1

Given a program with GLOBAL variable, LOCAL variables, which variables are in `a.out` file?  
___________ **WHY?** ____________


### Question 2

```c
Given main(int argc, char *argv[]) { int a=1, b=2; A(a,b); }
      int A(int x, int y) { int u, v; HERE: }
```
- #### Draw a diagram of stack contents from HERE: to argv of main()
- #### Identify the STACK FRAME of A() function.

### Question 3

```
myprintf(char *fmt,...)
{ char *cp; int *ip; ....}
```

### Question 4

```
printu(u32 x);
```
- Write code for `printd(int x)`.
- Write code to let `cp` point at the fmt string.
- Write code to let `ip` point at the first item to be printed on stack.

### Question 5

#### Partition Table: 
- ##### Write code for printd(int x) 
- ##### Write code to let cp point at the fmt string _____________
- ##### Write code to let ip point at the first item to be printed on stack ___________

### Question 6

#### Partition Table
##### 1. How did you get MBR of a virtual disk into char buf[512]?
##### 2. Write code to print the TYPE of partition 3 in MBR.
##### 3. What’s an extended partition?

### Question 7


- ##### Write C statments to define a node strcuture containing:
   - ##### name : array of 64 chars
   - ##### key  : int
   - ##### next : pointer to the next node;

- ##### Given a link of list of NODEs as defined above,
   - ##### Write C code to print the names of the list elements.
   - ##### Write code to INSERT a new node to the END of a list
   - ##### Write code to delete a node with a given key value 


### Question 8

- ##### What's a process? _______________
- ##### what does ksleep() dp ? ___________
- ##### What does kwakeup() do? ___________
- ##### How does a process terminate/ ___________

### Question 9 

#### Processes form a BINARY tree by child, sibiling pointers.

#### Assume: PROC *p kfork() a child *q.

- ##### (1) Write code to insert q into CHILD list of p: ___________
- ##### (2) Write code to print child list of p: __________

### Question 9 

- ##### For any Linux command, how does Linux sh find the command? _________
- ##### Write code to tokenize PATH into dir[0], dir[1], ___________

### Question 10

#### ASSUME: Your sh gets a command line "cmd one two three"

- ##### Write C code to tokenize the line into char *myargv[ ].
- ##### Write C code to execute cmd, passing as parameters myargv and env.

```
while(1) {
    get a command line;
    tokenize the command line;
    fork a child (EXACT code);
    if (parent) wait for child to die;
    repeat while loop;
    else (CHILD) sh do the command line;
}
```

### Question 11

```
CHILD sh: for a command line = CMD a b c d  > outfile 
   Write code to do > outfile: _________________
               MAY be <      :
   Create a pathname = dir[i]/cmd:    CODE: _______________
   Create myargv[  ]:   Show myargv[  ]   EXACTLY: _____________

            int r = execve(pathname, myargv, env);

   What if the execve() call succeeds? ____________
   What if the execve() call fails? __________

   Assume command line = cat filename | grep printf
   Write C code to do the pipe _____________
```

### Question 12

- ##### Threads vs. Process: What are threads?
- ##### How to create Threads?
- ##### What does thread_join() do? _________
- ##### What's the difference between thread_barrier() and thread_join()?
  
- ##### What’s a Critical Region?
- ##### How to protect Critical Regions?

### Question 13 

- ##### What’s partial pivoting? __________
- ##### Why partial pivoting? ___________

##### Given linear equations A*X = B
- ##### What’s LU decomposition? __________________
- ##### After converting A = L*U, how to solve for X? ________________________

### Question 13

```
                                      mkdir, rmdir, cd, pwd, chmod.
Write down syscall function for each: _____  _____  __  ___  _____
```

### Question 14 

- ##### How to tell a file is REGular?
- ##### How to tell a file is a DIRectory?
- ##### How to tell a file is a LINKfile?

### Question 15 


#### u32 st_ctime = 12345678:

- ##### What does the value mean? ________________

- ##### How to convert st_ctime into CALENDAR form? ___________

#### Write C code to print all the file names under Current Directory.


### Question 16 

```
Write C code to open filename for READ: _________________
                Then read 10 bytes:     _________________
               fopen filename for READ: _________________
                Then read 10 bytes:     _________________

Relation between fopen and open? ____________________________
                 fread and read? ____________________________

When to use syscall? When to use Lib I/O functions?

```

--- 

## Exam 2 - Review Questions 

### Question 1

#### Mailman’s Algorithm

Write code for the following function:
```c
tst_bit(int ibuf[256], int bit)
```

#### In a Disk Image:

- `BLKSIZE = 1024`
- `inode_size (s_inode_size) = 256`

**Answer the following questions:**

##### 1. **How many INODES are in a disk block?**
##### 2. **Inodes start block = 20:** Where do you get this value?
##### 3. **Given:** `ino = 16`
   - Which disk block contains this INODE? `blk = __________`
   - Which INODE in blk is `ino=16`? `offset = __________`
##### 4. Write code to load this INODE into `MINODE *mip`, which points to a minode.

### Question 2

#### `MINODE *iget(dev, ino)` Returns Pointer to a Minode=[dev, ino]

- ##### How do you try to find the needed minode in `cacheList`?
- ##### What if it’s not in `cacheList`?
- ##### `cacheList` is a queue created by `enqueue(MINODE **queue, MINODE *p)`, but its elements may be out-of-order.

- ##### Write code to re-order `cacheList`.


### Question 3

#### `iput(MINODE *mip)` Dispose of a mip

- ##### How do you decide whether to write `mip->INODE` back to disk?
- ##### Write code to write `mip->INODE` back to disk


### Question 4

#### `MINODE *mip` Points at a Minode, Whose INODE is a Directory


- ##### `char *cp, buf[BLKSIZE];`
- ##### `DIR *dp;`

   - ##### Write code to print the inode number of all files in this INODE.




### Question 5 

#### `ls dirname`

- ##### `pip = path2inode(dirname);`
- ##### Write code to get `[ino, name]` in `pip->INODE’s i_block[0]` (similar to 4 above).
  
```c
  For each `[ino, name]`,
    get its `MINODE` pointer `mip`;
    call `ls_file(MINODE *mip, char *name)`.
```

### Question 6

#### `mkdir pathname`

- ##### Check conditions: `_________________`
- ##### Allocate `ino`, `bno`.
- ##### Create `INODE` and write `INODE` to disk.
- ##### Create data block with `..`; write to `bno` on disk.
   - ##### What are the inode numbers of `.` and `..`?**

```
`enter_child()`: What’s `NEED_LEN`?
                        `IDEAL_LEN` of each entry? 
                 Where to enter the child entry?
```


### Question 7

#### `rmdir pathname`

- ##### How to check if the DIR is empty?
  - ##### `rm_child()`: How to remove the child entry in the parent’s data block?

### Question 8

####  `link oldfile newfile`

- ##### What does it do?


### Question 9

#### `unlink pathname`

- ##### What does it do?

### Question 10


#### `symlink old new`

- #### How to create a new symlink to old?


### Question 11 

- #### Draw a diagram to show what does opne filename 0(READ) do 


---

## Exam 2 - Questions 

### Question 1

- `int ibuf[256];`
- `int BIT` (a bit number)


#### Write code for `set_bit(int ibuf[ ], int BIT)` which sets `BIT` in `ibuf[ ]` to 1.


### Question 2

#### In a Disk Image:
- #### BLKSIZE = 1024
- #### inode_size (s_inode_size) = 256
- #### sizeof(INODE) = 128
- #### inodes_start_block = 20

#### Given: ino = 12, whose INODE is INODE12;

- ##### (1) How many INODES in a disk block? __________

- ##### (2) Which disk block contains INODE12?
   - ##### BLK = __________ (formula: __________)
     
- ##### (3) Which INODE in BLK is INODE12?
   - ##### OFFSET = __________ (formula: __________)

- ##### (4) Write code to get BLK into char buf[BLKSIZE] : ___________ 
   - ##### Then let INODE pointer ip point at INODE12 in buf[ ] : _________ 

### Question 3

#### Given:
```
   Given: enqueue(MINODE **queue, MINODE *p): enter p into queue.
          MINODE *dequeue(MINODE **queue): remove/return first queue element.
   cacheList is a queue of minodes, but its elements may be out-of-order.

```
#### Write code to re-order cacheList.

### Question 4


#### Assume:
char buf[BLKSIZE] contains the data block of a directory.

#### Given:

`DIR *dp, char *cp;`

#### Write code to print the [inode, rec_len] of all directory entries in buf [ ].
