#include <iostream>
//#include <starpu.h>
#include "file1.cpp"
using namespace std;
class Temp;
extern void execute();
class Temp : public check_inheritence
{
    protected:
        int m;
    public:
        check_inheritence ci;
        void set_a(int a)
        {
            ci.a = a;
        }
        void set_b (int b)
        {
            ci.b = b;
        }
      
        friend void execute();
        void print_ab()
        {
            ci.printa();
            ci.printb();
            ci.printexecute(&execute);
        }

}; 
 

