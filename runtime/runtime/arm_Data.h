#ifndef _Data_h
#define _Data_h

#include "atomic.h"
#include "debug.h"
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
  const size_t alignment = 16;
  Data():data(new T){}
  Data(T* _data): data(_data), refCount(0){
  }
  Data(const size_t size): data(NULL), refCount(0){
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
    refCount(0){
#if defined(HLLC_USE_TBB_SA_ALLOC)
    Debug(0).dbg_printf("HLLC_USE_TBB_SA_ALLOC \n");
    data  = (T*)scalable_malloc(rows * cols * sizeof(T));
    //tbb::scalable_allocator<T>().allocate(rows*cols);
#elif defined(HLLC_USE_TBB_CA_ALLOC)
    Debug(0).dbg_printf("HLLC_USE_TBB_CA_ALLOC \n");
    data  = (T*)scalable_aligned_malloc(rows * cols * sizeof(T), alignment);
    //tbb::cache_aligned_allocator<T>.allocate(rows*cols);
#else
    Debug(0).dbg_printf("HLLC_USE_TBB_STD_ALLOC \n");
    data  = new T[rows * cols];
#endif
  }
  Data(const size_t _rows, const size_t _cols, const size_t _slices): 
    data(NULL), 
    rows(_rows),
    cols(_cols),
    slices(_slices),
    refCount(0){
#if defined(HLLC_USE_TBB_SA_ALLOC)
    data  = scalable_malloc(rows * cols * slices * sizeof(T));
#elif defined(HLLC_USE_TBB_CA_ALLOC)
    data  = scalable_aligned_malloc(rows * cols * slices * sizeof(T),alignment);
#else
    data  = new T[rows * cols * slices];
#endif
  }
  void incRefCount()
  {
    Debug(0).dbg_printf("(%x) count before incRefCount: %i\n",data, refCount.read());
    refCount.inc();
    /*int count = refCount.inc();*/
    Debug(0).dbg_printf("(%x) count after incRefCount: %i\n",data, refCount.read());
  }

  void decRefCount()
  {
    //int count = refCount.dec();
    Debug(0).dbg_printf("(%x) decrementing refCount = %d\n",data, refCount.read());
    //if (count == 0) {
    if(refCount.dec()==0){
#if defined(HLLC_USE_TBB_SA_ALLOC)
      Debug(0).dbg_printf("(%x) deleting data(HLLC_USE_TBB_SA_ALLOC)\n",data);
      scalable_free((void*)data);
#elif defined(HLLC_USE_TBB_CA_ALLOC)
      Debug(0).dbg_printf("(%x) deleting data(HLLC_USE_TBB_CA_ALLOC)\n",data);
      scalable_aligned_free((void*)data);
#else
      Debug(0).dbg_printf("(%x) deleting data(std::delete)\n",data);
      delete [] data;
#endif
      delete this;
    }
  }

  T*  operator()(){
    return data;
  }

  void printRefCount()
  {
    printf("refcount = %i\n", refCount.read());
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
