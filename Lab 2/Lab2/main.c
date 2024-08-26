// Mark Shinozaki
// 11672355
// mysh

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

char gpath[128];    // hold token strings  
char *name[64];     // token string pointers
int  n;             // number of token strings

char dpath[128];    // hold dir strings in PATH
char *dir[256];      // dir string pointers
int  ndir;          // number of dirs   

int tokenize(char *pathname) 
{
  char *s;
  strcpy(gpath, pathname);   // copy into global gpath[]
  s = strtok(gpath, " ");    
  n = 0;
  while(s){
    name[n++] = s;           // token string pointers   
    s = strtok(0, " ");
  }
  name[n] = 0;               // name[n] = NULL pointer
}

int token(char *pathname) 
{
  char *s;
  strcpy(gpath, pathname);   // copy into global gpath[]
  s = strtok(gpath, ":");    
  ndir = 0;
  while(s){
    dir[ndir++] = s;           // token string pointers   
    s = strtok(0, ":");
  }
  dir[ndir] = 0;               // name[n] = NULL pointer
}


int excc(char *line) 
{
  FILE *outfile = fopen("outfile.txt","w");
  FILE *infile = fopen("outfile", "");
  int fd;
  int i;
  char *str;
  FILE* ptr;
  name[i] = tokenize(line);
  while (line != NULL)
  {
    //FILE *stdin
    if ( name[i] == "<")
    {
      int fd = fopen("outfile.txt", "r");
      dup(fd);
      fclose(fd);

      //printf("%s", line);
      fd = 0;

    }


    //
    // FILE *stdout
    //fd = 1
    else if (name[i] == ">")
    {
      int fd = fopen("outfile.txt", "w");
      while(fgets(str, 100,ptr) != NULL)
      {
        printf("%s",str);
      }
      fd = 1;


    }
    //FILE *stderr >> 
    // fd = 2
    else 
    {
      int fd = fopen("outfile.txt", "w");
      while(fgets(str, 100,ptr) != NULL)
      {
        printf("%s",str);
      }
      fd = 1;
      fd = 2;

    }
  }
}




int main(int argc, char *argv[ ], char *env[ ])
{
  int i;
  int r;
  char *home;
  char *path;
  int pid, status;
  char *cmd;
  char line[128];
  //system("clear");
  //initalize(env);

  printf("\n********************Welcome to MKsh********************  \n");

  // YOU DO: Write code to print argc, argv and env
  printf("argc = %d\n", argc);
  for (i = 0; i < argc; i++)
  {
    printf("argv[%d] = %s\n", i, argv[i]);
    //strncmp home and path
  }

  i = 0;
  while (env[i]){
    
    printf("\nn");
    if(strncmp(env[i],"HOME=", 5)==0)
    {
      home = env[i];
    } 
    
    if(strncmp(env[i],"PATH=", 5) ==0)
    {
      path = env[i];
      
    }
    
    printf("env[%d] = %s\n", i, env[i++]);
  }
  printf("\n");
  printf("home = %s\n", home);
  printf("path = %s\n", path);
  token(path);

  i=0;
  while (dir[i] != NULL)
  {
    printf("dir[%d]= %s\n", i, dir[i]);
    i++;
  }


// string tok 



  printf("\n********************MKsh processing loop********************  \n");

  // 
  
  
  
  while(1){
    printf("mysh %d running\n", getpid());
    printf("enter a command line : "); // cat file1 file2
                                       // ls -l -a -f
                                       // cat file | grep print
                                       // ANY valid Linux command line
    fgets(line, 128, stdin);
    line[strlen(line) - 1] = 0;        // kill \n at end 

    if (line[0]==0) // if (strcmp(line, "")==0) // if line is NULL
      continue;
    
    tokenize(line);       // divide line into token strings

    for (i=0; i<n; i++){  // show token strings   
        printf("name[%d] = %s\n", i, name[i]);
    }

    cmd = name[0];         // line = name0 name1 name2 ....

    if (strcmp(cmd, "cd")==0){
      chdir(name[1]);
      continue;
    }
    if (strcmp(cmd, "exit")==0)
      exit(0); 


    
    pid = fork();
    
  



    if (pid){
       printf("mysh %d forked a child sh %d\n", getpid(), pid);
       printf("mysh %d wait for child sh %d to terminate\n", getpid(), pid);
       pid = wait(&status);
       printf("ZOMBIE child=%d exitStatus=%x\n", pid, status); 
       printf("mysh %d repeat loop\n", getpid());
    }

    else{
       printf("child sh %d running\n", getpid());
       
       // make a cmd line = ./cmd for execve()
       i=0;
       while(dir[i] != NULL)
       {
        strcpy(line, dir[i]);
        strcat(line, "/");
        strcat(line, cmd);  // line="./cmd"
        printf("line = %s\n", line);
        r = execve(line, name, env);
        i++;
       }
       excc(line);
       printf("execve failed r = %d\n", r);
       exit(1);
    }
    
    
    //excc(line);

  }















}





/********************* YOU DO ***********************
1. I/O redirections:

// cat < t.c 







while(name[i] != "i")
{
  strcpy(head,name[i]);
  strcat(head)
}

// 1. read the input line from the user 


// 2. Tokenize the input line to separate the command and arguments 
// handle sinple commands 




Example: line = arg0 arg1 ... > argn-1

  check each arg[i]:
  if arg[i] = ">" {
     arg[i] = 0; // null terminated arg[ ] array 
     // do output redirection to arg[i+1] as in Page 131 of BOOK
  }
  Then execve() to change image


2. Pipes:

Single pipe   : cmd1 | cmd2 :  Chapter 3.10.3, 3.11.2

Multiple pipes: Chapter 3.11.2
****************************************************/
