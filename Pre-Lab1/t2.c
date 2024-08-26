#include <stdio.h>
#include <stdlib.h>

typedef unsigned int u32;

char *ctable = "0123456789ABCDEF";
int  BASE = 10; 
int BASE16 = 16;
int BASE8 = 8;

int main (void)
{
    char* testString = "this is a string";
    myprintf("%s %c %d %o %x %u %u %u\n hello \t this code \n is \"great\"", testString, 'Q', -100, 80, 160, 100, -100);
    getchar();
}


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

int printx (u32 x)
{
    putchar('0');
    putchar('x');
    
    (x==0) ? putchar('0') :rpu8(x);
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