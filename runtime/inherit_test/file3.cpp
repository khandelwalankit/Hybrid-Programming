#include<iostream>
#include"file2.cpp"
void execute()
{
  Temp* test_fp = new Temp();
  test_fp->m=10;
  printf("\n The value of m is %d\n",test_fp->m);
  delete test_fp;
}

int main(int argc, char** argv)
{
  Temp* root = new Temp();
  root->set_a(10);
  root->set_b(5);
  root->print_ab();
  delete root;
  return 1;
}
