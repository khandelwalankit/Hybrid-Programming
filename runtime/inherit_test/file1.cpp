#include <stdio.h>
struct check_inheritence 
{
    int a;
    int b;
    void printa()
    {
        printf("The value of A is %d",this->a);
    }
    void printb()
    {
        printf("The value of B is %d",this->b);
        //&execute;
    }
    void printexecute(void(*)fp)
    {
      fp;
    }
};
