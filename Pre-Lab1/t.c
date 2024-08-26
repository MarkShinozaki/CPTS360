#include <stdio.h>
#include <stdlib.h>
typedef unsigned int u32;
char *ctable = "0123456789ABCDEF";
int  BASE = 10; 
int BASE16 = 16;
int BASE8 = 8;
int *FP;


int main(int argc, char *argv[ ], char *env[ ])
{
  int a,b,c;
  printf("enter main\n");
  
  printf("&argc=%x argv=%x env=%x\n", &argc, argv, env);
  printf("&a=%8x &b=%8x &c=%8x\n", &a, &b, &c);

//(1). Write C code to print values of argc and argv[] entries
  //print("value of *argc and *argv is: &*argc, )
  a=1; b=2; c=3;
  A(a,b);
  printf("exit main\n");
}
	      
int A(int x, int y)
{ 
  int d,e,f; 
  printf("enter A\n");
  // write C code to PRINT ADDRESS OF d, e, f
  printf("&d=%8x",&d);
  printf(" &e=%8x",&e);
  printf(" &f=%8x",&f);
  d=4; e=5; f=6;
  B(d,e);
  printf("exit A\n");
}

int B(int x, int y)
{
  int g,h,i;
  printf("\nenter B\n");
  // write C code to PRINT ADDRESS OF g,h,i
  printf("&g=%8x",&g);
  printf(" &h=%8x",&h);
  printf(" &i=%8x",&i);
  g=7; h=8; i=9;
  C(g,h);
  printf("exit B\n");
}

int C(int x, int y)
{
  int u, v, w, i, *p;

  printf("\nenter C\n");
  // write C code to PRINT ADDRESS OF u,v,w,i,p;
  printf("&u=%8x",&u);
  printf(" &v=%8x",&v);
  printf(" &w=%8x",&w);
  printf(" &i=%8x",&i);
  printf(" &p:%8x\n",&p);
  
  u=10; v=11; w=12; i=13;
		   

//======================================================================
 // FP = (int *)getebp();  // FP = stack frame pointer of the C() function

 FP = (int *) getebp(); 
 printf("\nFP = ebp = %8x\n", FP);
// FP is a 32-bit ADDRESS. Write C code to print its value in HEX  

//(2). Write C code to print the stack frame link list as
    printf("stack frame link list\n");
    while(FP)
    {
        printf("%p -> ", (void*)FP);
        FP = (int*)*FP;
       
        //*FP!=NULL;
    }
    printf("%p\n", (void*)FP);
    // FP -> previousFP -> previousFP -> .... -> NULL      (all in Hex)
    
//(3). Let p = &u;
   //  Print the stack contents from p to the frame of main().
   //  YOU MAY JUST PRINT 128 entries of the stack contents.
    printf("128 stack contents \n");
    for (int i = 0; i < 128; i++)
    {
        printf("%p      %x  \n", (void*)p, *p);
        p++;
    }
    
   
} 

//(4). On a hard copy of YOUR print out, identify the stack contents
//     as LOCAL VARIABLES, PARAMETERS, stack frame pointer of each function.

/*
Given: putchar(char c) of Linux, which prints a char.

1-1. Write YOUR own prints(char *s) fucntion to print a string.
     If you don't know how to do it, you BETTER attend my class lecture !!!!

Given: The following printu() function print unsigned integer.

typedef unsigned int u32;

char *ctable = "0123456789ABCDEF";
int  BASE = 10; 

int rpu(u32 x)
{  
    char c;
    if (x){
       c = ctable[x % BASE];   
       rpu(x / BASE);
       putchar(c);           
    }
}

int printu(u32 x)
{
   (x==0)? putchar('0') : rpu(x);
   putchar(' ');
}

		          EXAMPLE:                             stack
      Assume u32 x = 123;                                      ------ High
      1st call to rpu(x): x=123; x%10 = 3 ===>  c = ctable[3] = '3';   
      2nd call to rpu(x): x=12;  x%10 = 2 ===>  c = ctable[2] = '2';  
      3rd call          : x=1;   x%10 = 1 ===>  c = ctable[1] = '1';  
      --------------------------------------------------------------- Low
      4th call          : x=0 => return  ====>      print       '1';
                                 retrun  ====>      print       '2'
                                 retrun  ====>      print       '3'

*/

// Part 1: myprintf Function:

//1. Show results of myprintf() function
int rpu(u32 x)
{  
    char c;
    if (x){
       c = ctable[x % BASE];
       rpu(x / BASE);
       putchar(c);
    }
}

int rpu8(u32 x)
{
    char c;
    if (x){
       c = ctable[x % BASE8];
       rpu8(x / BASE8);
       putchar(c);
    }
}

int rpu16(u32 x)
{
    char c;
    if (x){
       c = ctable[x % BASE16];
       rpu16(x / BASE16);
       putchar(c);
    }
}

int printu(u32 x)
{
   (x==0)? putchar('0') : rpu(x);
   putchar(' ');
}

int printd(int x)
{
    if (x < 0)
    {
        x *= -1;
        putchar('-');
        printu(x);
    }
    else
    {
        printu(x);
    }
}

int printo(u32 x)
{
    putchar('0');
    (x==0) ? putchar('0') :rpu8(x);
    putchar(' ');
}

int printx(u32 x)
{
    putchar('0');
    putchar('x');
    
    (x==0) ? putchar('0') :rpu16(x);
    putchar(' ');
}

int prints(char *string)
{
    while(*string)
    {
        putchar(*string);
        string++;
    }
}


void myprintf(char* fmt, ...)
{
    char *cp = fmt;
    int *ip = (int*) &fmt + 1;

    while(*cp)
    {
        if(*cp != '%')
        {
            putchar(*cp);
        }else{
            switch(*(cp+1))
            {
                case 'c':
                    putchar((char)*ip++);
                    break;
                case 's':
                    prints((char *)*ip++);
                    break;
                case 'd':
                    printd((int) *ip++);
                    break;
                case 'u':
                    printu((u32) *ip++);
                    break;
                case 'o':
                    printo((u32) *ip++);
                    break;
                case 'x':
                    printx((u32) *ip++);
                    break;
                default:
                    putchar(*fmt);
                    break;
            }
            cp++;
        }
        cp++;
    }
}











// Part 2: Partition Table

// 1. Show start_sector, nr_sectors, sys_type of 4 partitions in MBR


// 2. Show start_sector, nr_sectors of All extended partitions 


// --------- Quiz: Any Two Questions ---------- 








