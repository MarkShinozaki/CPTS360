/************** lab5base.c file ******************/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ext2fs/ext2_fs.h>
#include <string.h>

typedef struct ext2_super_block SUPER;
typedef struct ext2_group_desc  GD;
typedef struct ext2_inode       INODE;
typedef struct ext2_dir_entry_2 DIR;

SUPER *sp;
GD    *gp;
INODE *ip;
DIR   *dp;   

#define BLKSIZE 1024

int fd;             // opened vdisk for READ
int inodes_block;   // inodes start block number

char gpath[128];    // token strings
char *name[32];
int n;
char indirect_blk_buf[1024], double_indirect_blk_buf[1024];

char *disk = "diskimage";
int dev = 0; // set this when opening disk

int get_block(int dev, int blk, char *buf)
{   
   lseek(dev, blk*BLKSIZE, SEEK_SET);
   return read(dev, buf, BLKSIZE);
}

int search(INODE *ip, char *name)
{
   // Chapter 11.4.4  int i; 
   // Exercise 6
   char buf2[BLKSIZE], temp[256];
   char * cp;
   int ino;


   printf("i = 0  i_block[0] = %d\n", ip->i_block[0]);
   printf("   i_number rec_len name_len    name\n");
  
   get_block(fd, ip->i_block[0], buf2);
   dp = (DIR *)buf2;
   cp = buf2;
   printf("      %d      %d       %d        %s\n",dp->inode, dp->rec_len, dp->name_len, temp);
   while (cp < buf2 + ip->i_block[0])
   {
      strncpy(temp, dp->name, dp->name_len);
      temp[dp->name_len]=0;
      printf("      %d      %d       %d        %s\n",dp->inode, dp->rec_len, dp->name_len, temp);
      if (strcmp(name, temp)== 0)
         ino = dp->inode;
      cp += dp->rec_len;
      dp = (DIR *)cp;
   }
   return ino;
}

int show_dir(INODE *ip)
{
   char sbuf[BLKSIZE], temp[256];
   DIR *dp;
   char *cp;
 
   // ASSUME only one data block i_block[0]
   // YOU SHOULD print i_block[0] number here
   get_block(dev, ip->i_block[0], sbuf);

   dp = (DIR *)sbuf;
   cp = sbuf;
 
   while(cp < sbuf + BLKSIZE){
       strncpy(temp, dp->name, dp->name_len);
       temp[dp->name_len] = 0;
      
       printf("%4d %4d %4d %s\n", dp->inode, dp->rec_len, dp->name_len, temp);

       cp += dp->rec_len;
       dp = (DIR *)cp;
   }
}

int mount_root()
{

}

// FIX UP 
INODE *path2inode(INODE *ip, char pathname[], int inodes_start)
{
   int i = 0;
   char *tokenizedPath[64];
   int numOfComponents = 0;
   
   if(pathname != 0) //* If a pathname was passed
   {
      //* Tokenize path
      numOfComponents = tokenize(pathname, tokenizedPath);
      int j=0;
        
      //TODO:For debugging only
      printf("Tokenized path:> ");
      while(j < numOfComponents)
      {
         printf("/%s", tokenizedPath[j++]);
         fflush(stdout);
      }
      puts("");

      int ino, blk, offset;
      int n = numOfComponents; 
    
      for (i=0; i < n; i++)
      {
         ino = search(ip, tokenizedPath[i]);
        
         if (ino==0)
         {
            printf("can't find %s\n", tokenizedPath[i]); 
            exit(1);
         }
         // Mailman's algorithm: Convert (dev, ino) to INODE pointer
         blk    = (ino - 1) / 8 + inodes_start; 
         offset = (ino - 1) % 8;        
         get_block(dev, blk, buf);
         ip = (INODE *)buf + offset;   // ip -> new INODE
      }
      //*Print information out of current ip
      printIPinfo(tokenizedPath[i-1], ip);
      return ip;
    }
   else
   {
      show_dir(ip);
   }
}

/*************************************************************************/
int tokenize(char *pathname)
{
  
   strcpy(gpath, pathname);   // copy into global gpath[]
   char *s;
   s = strtok(gpath, "/");    
   n = 0;
   while(s)
   {
        name[n++] = s;           // token string pointers   
        s = strtok(0, "/");
   }
   name[n] = 0; 
   return 1;
} 

// the start of the "showblock" program
// usage: "./a.out /a/b/c/d"
// that means you will have to parse argv for the path
// see the example 'lab5.bin' program
// example usage: ./lab5.bin lost+found
int main(int argc, char *argv[])
{
   // follow the steps here: https://eecs.wsu.edu/~cs360/LAB5.html
   char buf[BLKSIZE];  // use more bufs if you need them
   int ino = 0;
   char *c;
   size_t bytes_read;

   // 1
   int fd = open("vdisk", O_RDONLY);
   printf("1. open vdisk for READ: fd = %d \n", fd);

   // 2
   lseek(fd, 1024, SEEK_SET);
   bytes_read = read(fd, buf, 1024);

   sp = (SUPER*)buf;
      

   //print s_magic in Hex
   printf("2. read SUPER block #1 to verify EXT2 fs : s_magic = %x ", sp->s_magic);
   //Verify EXT2
   if(sp->s_magic != 0xEF53)
   {
     
      printf("This is not a EXT2 fs!\n");
      exit(1);
   }
   else
      printf("ok\n");
   


   // 3 - Move up to Block#2 and read into global buf 
   get_block(fd, 2, buf); //Cast the buf as a group descriptor 
   gp = (GD*)buf;

   puts("\n3.-----------(( Group Descriptor info: ))---------------");
   puts("map | imap | iblock | # free blocks | # free inodes | dir count");
   printf(" %d      %d      %d          %d             %d           %d\n",
      gp->bg_block_bitmap,
      gp->bg_inode_bitmap,
      gp->bg_inode_table,
      gp->bg_free_blocks_count,
      gp->bg_free_inodes_count,
      gp->bg_used_dirs_count);

   int bmap = gp->bg_block_bitmap;
   int imap = gp->bg_inode_bitmap;
   int iblock = gp->bg_inode_table;
   printf("\tSummary: bmap = %d | imap = %d | iblock = %d\n\n", bmap, imap, iblock);
   int inodes_start=iblock; 

   inodes_block = gp->bg_inode_table;


   // 4. Let INODE *ip

   get_block(dev,iblock,buf);
   ip = (INODE *)buf;
   ip++;

   ip = path2inode(ip, pathname, inodes_start);

   int j = 0;
   if(ip->i_block[12] > 0)
   {
      //TODO: search indirect blocks
      puts("\n-=-=-=-=-=-==-=-=-=-( Indirect Blocks )=-=-=-=-=-=-=-=-=");
      get_block(dev, ip->i_block[12], indirect_blk_buf);
      int * ind_blk_buf_ptr = (int *)indirect_blk_buf;
      for(i=0; i < 256; i++)
      {
         if(ind_blk_buf_ptr[i] == 0)
         {
            break;
         }
         printf("%d ", ind_blk_buf_ptr[i]);
         fflush(stdout);
      }
      puts("\n=-=--=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-==-=-= ");
    }
    
   if(ip->i_block[13] > 0)
   {
      
      get_block(dev, ip->i_block[13], indirect_blk_buf);
      int * ind_blk_buf_ptr = (int *)indirect_blk_buf;
      puts("\n-=-=-=-=-=-=-=-=-=( Double Indirect Blocks )=-=-=-=-=-=-=-=-=-=- ");
      for(i=0; i < 256; i++)
      {
         if(ind_blk_buf_ptr[i] == 0)
         {
            break;
         }
         get_block(dev,ind_blk_buf_ptr[i], double_indirect_blk_buf);
         int * doub_ind_blk_buf_ptr = (int *)double_indirect_blk_buf;
         for(j=0; j < 256; j++)
         {
            if(doub_ind_blk_buf_ptr[j] == 0)
            {
               break;
            }
            printf("%d ", doub_ind_blk_buf_ptr[j]);
            fflush(stdout);
            }
        }
      puts("\n=-=--=-=-==-=-=-=-=-=-=-=-=-=-=-=-=-==-=-= ");
    }


}
 









