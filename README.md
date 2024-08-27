# Exam 2 

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

