#include <stdio.h>             // for I/O
#include <stdlib.h>            // for I/O
#include <string.h>

char gpath[256];
char name[16][64];           // OR   char *name[16];
int n;

int tokenize(char *pathname)
{
  int i;
  char *s;
  printf("pathname=%s\n", pathname);
  strcpy(gpath, pathname);
  n = 0;
  s = strtok(gpath, "/");

  while(s){
    strcpy(name[n++], s);    // OR  name[n++] = s;
    s = strtok(0, "/");
  }

  for (i=0; i < n; i++){
    printf("name[%d]=%s\n", i, name[i]);
  }
}

int main()
{
  tokenize("/a/b/c/d");
  tokenize("x/y/z");
}

/********************************  
NODE *search(NODE *parent, char *name)
{
  // same as before
}
NODE *path2node(char *pathname)
{
  NODE *start, *p;
  int i;
  1. tokenize(pathname);
  2. NODE *start = root OR cwd;
  3. for (i=0; i < n; i++){
        p = search(start, name[i]);
        if (p==0) 
           return 0;
        start = p;
     }
  4. return p;
}
*******************************/