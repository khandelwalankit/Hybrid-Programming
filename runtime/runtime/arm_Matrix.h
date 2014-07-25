#ifndef _Matrix_h
#define _Matrix_h

#include "atomic.h"
#include <armadillo>
#include <stdint.h>
#include "Data.h"
#include <initializer_list>

namespace Tasks
{
using namespace arma;
using namespace std;

template <class T>
class Matrix
{
 public:
   typedef Data<T> matData;
   typedef Mat<T> ArmadilloMatrix;
  //Matrix(std::vector<long long int>& args):mat(args[0](),args[1],args[2],false,false){}
  //Matrix():data(new matData()),mat(data->data,1,1,false,false){
  
    Matrix():data(new matData(1,1)),mat(data->data, 1,1,false, false){}
    Matrix(int _rows, int _cols):data(new matData(_rows, _cols)), mat(data->data, _rows, _cols, false, false){}
  
    Matrix(matData* _data):
    data(_data),
    arma_mat(_data->data,_data->rows,_data->cols,false,false){}

    ArmadilloMatrix& operator()(){
      return arma_mat;
    }
    
    void deleteData(){
      delete []data->data;
      delete data;
    }
    //Data<T>* data;
    matData* data;
    ArmadilloMatrix arma_mat;
};

} // namespace RTasks

#endif // #ifndef _Matrix_h
