#ifndef _MATRIX_BASE_H_
#define _MATRIX_BASE_H_
namespace Tasks{
  using namespace arma;
  using namespace std;

  class MatrixBase{
    public:
      virtual ~MatrixBase(){}
      //virtual MatrixBase& operator()()=0;
  };
}

#endif //_MATRIX_BASE_H_
