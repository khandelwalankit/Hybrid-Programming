#ifndef _Matrix_h
#define _Matrix_h

#include "debug.h"
#include "atomic.h"
#include <armadillo>
#include <stdint.h>
#include "MatrixBase.h"
#include "Data.h"
#include <initializer_list>


namespace Tasks
{
  using namespace arma;
  using namespace std;


  template <class T>
  class Matrix:public MatrixBase
  //class Matrix
  {
    public:
      typedef Data<T> matData;
      typedef Mat<T> ArmadilloMatrix;
      
      Matrix():data(new matData(1,1)), arma_mat(data->data,1,1,false, false){
        //arma_mat  = new ArmadilloMatrix(data->data, 1,1,false, false);
      }
      
      /*Matrix(T* _data, size_t nrows, size_t ncols):
        data(new matData(nrows,ncols)),
        arma_mat(data->data, nrows, ncols, true){
          cout<<"Deep-Copied the data..."<<data->data<<"()"<<arma_mat.memptr()<<endl;
          cout<<"ARMA_MAT = \n"<<arma_mat<<endl;
          //arma_mat  = new ArmadilloMatrix(data->data, nrows, ncols, flag);
      }*/

      Matrix(T* _data, size_t nrows, size_t ncols):
        data(new matData(_data, nrows, ncols)),
        arma_mat(data->data,nrows,ncols){}
      
      Matrix(int _rows, int _cols):data(new matData(_rows, _cols)), arma_mat(data->data, _rows, _cols, false, false){}
      /*Overload the copy-constructor, but do not do a deep-copy the data member*/
      
      Matrix(matData* _data): data(_data),
        arma_mat(_data->data, _data->rows, _data->cols, false, false){
        /*arma_mat = new ArmadilloMatrix(_data->data,_data->rows,_data->cols,false,false);*/
      }
      ArmadilloMatrix& operator()(){
        //return *arma_mat;
        return arma_mat;
      }
      ~Matrix(){
        //cout<<"Matrix Destructor...."<<this<<endl;
        //delete arma_mat;
        data->decRefCount();
      }
      void deleteArmaWrapper(){
        //printf("(%x):Deleting arma-wrapper(%d)\n",data->data,data->getRefCount());
        //delete arma_mat;
      }
      matData* data;
      ArmadilloMatrix arma_mat;
  };
} // namespace RTasks

#endif // #ifndef _Matrix_h
