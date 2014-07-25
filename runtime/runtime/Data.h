#ifndef _Data_h
#define _Data_h

#include "debug.h"
#include "atomic.h"
#include <armadillo>
#include <stdint.h>
#include <tbb/scalable_allocator.h>
#include <tbb/cache_aligned_allocator.h>
#include <initializer_list>

#ifndef ALIGNMENT
#define ALIGNMENT 16
#endif
namespace Tasks
{
using namespace arma;
using namespace std;

template <class T>
class Data 
{
 public:
  typedef Data<T> matData;
  const size_t alignment = 16;
  Data():data(new T){}
  Data(T* _data): data(_data), refCount(1){
  }
  Data(matData* data_ptr):
    data(data_ptr->data),
    rows(data_ptr->rows),
    cols(data_ptr->cols),
    slices(data_ptr->slices),
    refCount(data_ptr->refCount){}

  matData& operator=(matData&& other){
    data      = std::move(other.data);
    rows      = std::move(other.rows);
    slices    = std::move(other.slices);
    refCount  = std::move(other.refCount);
    return *this;
  }
  Data(const size_t size): data(NULL), refCount(1){
    //data  = new T[size];
#if defined(HLLC_USE_TBB_SA_ALLOC)
    data  = scalable_malloc(size * sizeof(T));
#elif defined(HLLC_USE_TBB_CA_ALLOC)
    data  = scalable_aligned_malloc(size * sizeof(T),alignment);
#else
    data  = new T[size];
#endif
  }
  Data(const size_t _rows, const size_t _cols): 
    data(NULL), 
    rows(_rows),
    cols(_cols),
    refCount(1){
#if defined(HLLC_USE_TBB_SA_ALLOC)
    // Debug(1).dbg_printf("HLLC_USE_TBB_SA_ALLOC \n");
    data  = (T*)scalable_malloc(rows * cols * sizeof(T));
    //tbb::scalable_allocator<T>().allocate(rows*cols);
#elif defined(HLLC_USE_TBB_CA_ALLOC)
    //Debug(1).dbg_printf("HLLC_USE_TBB_CA_ALLOC \n");
    data  = (T*)scalable_aligned_malloc(rows * cols * sizeof(T), alignment);
    //tbb::cache_aligned_allocator<T>.allocate(rows*cols);
#else
    data  = new T[rows * cols];
    Debug(2).dbg_printf("(%x):HLLC_USE_TBB_STD_ALLOC\n",data);
#endif
  }
  /*Data(T* _data, const size_t _rows, const size_t _cols): 
    data(_data), 
    rows(_rows),
    cols(_cols),
    refCount(0){
  }*/
  Data(T* _data, const size_t _rows, const size_t _cols):
    rows(_rows),
    cols(_cols),
    refCount(1){
    #if defined(HLLC_USE_TBB_SA_ALLOC)
      //Debug(1).dbg_printf("HLLC_USE_TBB_SA_ALLOC \n");
      data  = (T*)scalable_malloc(rows * cols * sizeof(T));
      //tbb::scalable_allocator<T>().allocate(rows*cols);
    #elif defined(HLLC_USE_TBB_CA_ALLOC)
      //Debug(1).dbg_printf("HLLC_USE_TBB_CA_ALLOC \n");
      data  = (T*)scalable_aligned_malloc(rows * cols * sizeof(T), alignment);
      //tbb::cache_aligned_allocator<T>.allocate(rows*cols);
    #else
      data  = new T[rows * cols];
      memcpy(data, _data, rows*cols*sizeof(T));
      Debug(2).dbg_printf("(%x):HLLC_USE_TBB_STD_ALLOC\n",data);
    #endif
    }
  
  Data(const size_t _rows, const size_t _cols, const size_t _slices): 
    data(NULL), 
    rows(_rows),
    cols(_cols),
    slices(_slices),
    refCount(1){
#if defined(HLLC_USE_TBB_SA_ALLOC)
    data  = scalable_malloc(rows * cols * slices * sizeof(T));
    Debug(2).dbg_printf("HLLC_USE_TBB_SA_ALLOC \n");
#elif defined(HLLC_USE_TBB_CA_ALLOC)
    data  = scalable_aligned_malloc(rows * cols * slices * sizeof(T),alignment);
    Debug(2).dbg_printf("HLLC_USE_TBB_CA_ALLOC \n");
#else
    data  = new T[rows * cols * slices];
    Debug(2).dbg_printf("(%x):HLLC_USE_TBB_STD_ALLOC\n",data);
#endif
  }
  void incRefCount(){
    Debug(2).dbg_printf("(%x) count before incRefCount: %i\n",data, refCount.read());
    refCount.inc();
    /*int count = refCount.inc();*/
    Debug(2).dbg_printf("(%x) count after incRefCount: %i\n",data, refCount.read());
  }

 void decRefCount(){
    //int count = refCount.dec();
    Debug(2).dbg_printf("(%x) decrementing refCount = %d\n",data, refCount.read());
    //printf("(%x) decrement refCount = %d\n", data, refCount.read());
    //if (count == 0) {
    if (refCount.dec()  ==  0) {
#if defined(HLLC_USE_TBB_SA_ALLOC)
      Debug(2).dbg_printf("(%x) deleting data(HLLC_USE_TBB_SA_ALLOC)\n",data);
      scalable_free((void*)data);
#elif defined(HLLC_USE_TBB_CA_ALLOC)
      Debug(2).dbg_printf("(%x) deleting data(HLLC_USE_TBB_CA_ALLOC)\n",data);
      scalable_aligned_free((void*)data);
#else
      Debug(2).dbg_printf("(%x) deleting data(std::delete)\n",data);
      //printf("(%x) deleting data(std::delete)\n",data);
      delete [] data;
      data  = NULL;
#endif
      delete this;
    }
  }
  
  T*  operator()(){
    return data;
  }

  void printRefCount(){
    //printf("refcount = %i\n", refCount.read());
  }
  int getRefCount(){
    return refCount.read();
  }

  T* data;
  unsigned int rows;
  unsigned int cols;
  unsigned int slices;
 private:
  atomic_t<size_t> refCount;
};

} // namespace RTasks

#endif // #ifndef _Matrix_h
