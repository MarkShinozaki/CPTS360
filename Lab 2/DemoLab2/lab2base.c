#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include <stdlib.h>

typedef struct node
{
  char name[64];
  char type;
  struct node *child, *sibling, *parent;
} NODE;

NODE *root = NULL, *cwd;
char line[128];
char command[64], pathname[64];
char dir_name[64], base_name[64];
char *name[100];
int n;
FILE *fp;

int initialize(void);
int findCmd(char *command);
int mkdir(char *pathname);
int rmdir(char *pathname);
int ls(char *pathname);
int cd(char *pathname);
int pwd(char *pathname);
int creat(char *pathname);
int rm(char *pathname);
int reload(char *pathname);
int save(char *pathname);
int menu(char *pathname);
int quit(char *pathname);
char *cmd[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm", "reload", "save", "menu", "quit", NULL};

int main()
{
  int (*fptr[])(char *) = {(int (*)())mkdir, rmdir, ls, cd, pwd, creat, rm, reload, save, menu, quit};

  initialize(); //initialize tree
  while (1)
  {
    strcpy(pathname, " ");
    printf("Enter command: ");
    fgets(line, 128, stdin); // get at most 128 chars from stdin

    line[strlen(line) - 1] = 0;               //next line
    sscanf(line, "%s %s", command, pathname); //sscan command and pathname
    int command2 = findCmd(command);          //find command

    if (command2 < 0 || command2 > 10)
    {
      printf("Command DNE!!\n"); //to handle seg fault
    }
    else
    {
      int result = fptr[command2](pathname); //pass function as parameter
    }
    if (command2 == 10)
    {
      break;
    }
  }
}

int initialize(void)
{
  root = (NODE *)malloc(sizeof(NODE)); //allocate space for root
  root->child = NULL;                  //initialize child to NULL
  root->parent = root;                 //initialize parent to root
  root->sibling = NULL;                //initialize sibling to NULL
  root->name[0] = '/';                 //initialize start directory in pwd
  root->type = 'D';
  cwd = root; //initialize cwd to root
}

int delete_func(NODE *p)
{
  if (!p)
  {
    return 1;
  }
  NODE *childp = p->child;
  NODE *siblingp = p->sibling;

  free(p); //remove node

  delete_func(childp);   //recursion to delete p->child
  delete_func(siblingp); //recursion to delete p->sibling
}

int deleteTree(void)
{
  delete_func(root); //delete tree
}

int tokenize(char *pathname)
{
  int token = 0;
  char *n;
  n = strtok(pathname, "/"); // first call to strtok()

  if (!n)
  {
    return 0;
  }
  while (n)
  {
    name[token] = n;
    token++;            //increment tok
    n = strtok(0, "/"); // call strtok() until NULL
  }
  return token;
}

NODE *makeNode(char *name, char type)
{
  NODE *newNode = (NODE *)malloc(sizeof(NODE)); //allocate space for new node
  newNode->child = NULL;                        //initialiE child, sibling, parents to NULL for node
  newNode->sibling = NULL;                      //            -
  newNode->parent = NULL;                       //            -
  strcpy(newNode->name, name);                  //strcpy string to node
  newNode->type = type;                         //define type of dir node is
}

NODE *search_child(NODE *parent, char *name)
{
  NODE *p = parent->child;
  while (p)
  {
    if (strcmp(p->name, name) == 0)
    {
      return p;
    }

    p = p->sibling;
    if (!p)
    {
      break;
    }
  }
  return 0;
}

NODE *findNode(char *pathname)
{
  NODE *start = NULL;
  int n = 0;

  if (pathname[0] == '.')
  {
    return cwd;
  }
  if (pathname[0] == '/')
  {
    start = root;
  }
  else
  {
    start = cwd;
  }

  n = tokenize(pathname); //token pathname
  NODE *p = start;

  for (int i = 0; i < n; i++)
  {
    p = search_child(p, name[i]);
    if (!p)
    {
      printf("path DNE\n");
      return 0;
    }
  }
  return p;
}

int findCmd(char *command)
{
  int i = 0;
  while (cmd[i])
  {
    if (strcmp(command, cmd[i]) == 0)
    {
      return i;
    }
    i++;
  }
  return -1;
}

char dir_name[64], base_name[64]; //dir_name and base_name

int dir_base_name(char *pathname)
{
  char temp[128];
  strcpy(temp, pathname);
  strcpy(dir_name, dirname(temp));
  strcpy(temp, pathname);
  strcpy(base_name, basename(temp));
}

int insertNode(NODE *p, char *pathname, char type)
{
  NODE *newNode = makeNode(base_name, type);
  newNode->parent = p;

  if (!p->child)
  {
    p->child = newNode;
    return 1;
  }
  else
  {
    p = p->child;
    while (p->sibling != NULL)
    {
      p = p->sibling;
    }
    p->sibling = newNode;
    return 1;
  }
  return 0;
}

int removeNode(NODE *p)
{
  NODE *temp = p->parent->child;

  if (temp == p)
  {
    p->parent->child = p->sibling;
  }
  else
  {
    while (temp->sibling != p)
    {
      temp = temp->sibling;
    }
    temp->sibling = p->sibling;
  }
  free(p); // free node
}

int mkdir(char *pathname)
{
  dir_base_name(pathname);
  NODE *p = NULL;

  p = findNode(dir_name);

  if (!p || p->type == 'F')
  {
    printf("ERROR! Could not find directory %s\n", dir_name);
    return 0;
  }
  else
  {
    // MAKE DISTINCTION BETWEEN INSERTING RELATIVE INSERTs

    ///not found, ie insert here
    if (search_child(p, base_name) == 0)
    {
      insertNode(p, base_name, 'D');
    }
  }
  return 0;
}

int rmdir(char *pathname)
{
  NODE *p = NULL;
  p = findNode(pathname);

  if (!p || p->type == 'F')
  {
    printf("File DNE!! %s\n", dir_name);
    return 0;
  }
  else
  {
    if (p->child) //check if tree has child
    {
      printf("Directory not empty! Cannot remove directory \n");
      return 0;
    }
    else
    {
      removeNode(p); //remove node
      return 1;
    }
  }

  //printf("Exiting rmdir!\n");
  return 0;
}

int ls(char *pathname)
{
  NODE *temp = cwd->child;

  while (temp)
  {
    printf("[TYPE: %c   NAME: %s] \n", temp->type, temp->name);
    temp = temp->sibling;
  }
  printf("\n");
  return 0;
}

int cd(char *pathname)
{
  NODE *p = NULL;

  //find node to directory we wanna insert in
  p = findNode(pathname);

  if (strcmp(pathname, "..") == 0)
  {
    cwd = p->parent;
    return 1;
  }
  else if (strcmp(pathname, " ") == 0)
  {
    cwd = root;
    return 1;
  }

  if (!p || p->type == 'F')
  {
    printf("Directory DNE!! %s\n", dir_name);
    return 0;
  }
  cwd = p;
  return 1;
}

int pwd_func(NODE *p)
{
  if (p == root)
  {
    printf("/"); //print "/"
    return 0;
  }
  pwd_func(p->parent);    //recurse to each directory
  printf("%s/", p->name); //print directory
  return 0;
}

int pwd(char *pathname)
{
  NODE *p = cwd;
  pwd_func(p); //recursion to print each directory
  printf("\n");
  return 0;
}

int creat(char *pathname)
{
  dir_base_name(pathname);
  NODE *p = NULL;

  p = findNode(dir_name);

  if (!p || p->type == 'F')
  {
    printf("File DNE %s\n", dir_name);
    return 0;
  }
  else
  {
    if (search_child(p, base_name) == 0)
    {
      insertNode(p, base_name, 'F');
    }
  }
  return 0;
}

int rm(char *pathname)
{
  NODE *p = NULL;
  p = findNode(pathname);

  if (!p || p->type == 'D')
  {
    printf("File DNE or wrong file type %s\n", dir_name); //if p DNE or wrong file type
    return 0;
  }
  else
  {
    removeNode(p); //remove F-type file
    return 1;
  }
  return 0;
}

int reload(char *pathname)
{
  FILE *infile = fopen("myFile.txt", "r");

  if (!infile)
  {
    return 0; //check if file exists
  }

  char line[100];
  char type[1];
  char pname[100];
  char name[100];
  fgets(line, 100, infile); //get root node

  deleteTree(); //deletes current tree
  initialize(); //reinitializes tree
  strcpy(line, " ");

  while (fgets(line, 100, infile))
  {
    sscanf(line, "%s %s %s", type, pname, name); //read strings

    if (type[0] == 'D')
    {
      mkdir(strcat(pname, name)); //create D-type directory
    }
    else
    {
      creat(strcat(pname, name)); //create F-type directory
    }
    strcpy(line, " ");
  }

  fclose(infile); //close file for reading
  return 1;
}

int saveFile(NODE *p, FILE *outfile, char *pathname)
{
  if (!p)
  {
    return 1; //check if p exists
  }

  fprintf(outfile, "%c %s %s\n", p->type, pathname, p->name);
  char nextPath[100];
  strcpy(nextPath, pathname);
  strcat(nextPath, p->name);

  if (strcmp(p->name, "/") != 0)
  {
    strcat(nextPath, "/");
  }
  saveFile(p->child, outfile, nextPath);
  saveFile(p->sibling, outfile, pathname);
}

int save(char *pathname)
{
  FILE *outfile = fopen("myFile.txt", "w"); //open myFile.txt
  saveFile(root, outfile, "");              //save contents in myFile
  fclose(outfile);
  return 0;
}

int menu(char *pathname)
{
  printf("\n|-------------------------------- MENU --------------------------------|\n");
  printf("Commands: mkdir, rmdir, ls, cd, pwd, creat, rm, reload, save, menu, quit\n");
  return 0;
}

int quit(char *pathname)
{
  deleteTree(); //free space - delete tree
  return 0;
}