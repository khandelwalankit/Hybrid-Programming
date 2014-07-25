#ifndef _Task_h_
#define _Task_h_

#pragma once

#include <vector>
#include <numeric>
#include <iostream>
#include <assert.h>
#include <tbb/task.h>
#include <tbb/atomic.h>
#include <atomic>
#include <tbb/spin_mutex.h>
#include <tbb/queuing_rw_mutex.h>
#include <tbb/tick_count.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/concurrent_hash_map.h>
#include <armadillo>
#include "Matrix.h"
#include "MatrixBase.h"
#include "Data.h"
#include "debug.h"
#include <iostream>

#define END 32000


extern tbb::spin_mutex printMutex;
extern tbb::spin_mutex mapSearchMutex;

using namespace arma;
namespace Tasks{
typedef std::vector<unsigned int>   iteration_vector;
typedef std::pair<int,iteration_vector>     TaskRef; // <static task ID, iteration vector(or 0)>
typedef Data<double>                DoubleData;
typedef Matrix<double>              DoubleMat;
typedef cube                        DoubleCube;
typedef std::vector<long long>      matrec;

extern tbb::task* root;

class Task : public tbb::task{
  protected:
    std::vector<unsigned int> iter_count;
    unsigned int static_id;
    unsigned int num_inputs;
    unsigned int num_scalar_inputs;
    std::vector<MatrixBase*> inputs;
    std::vector<double> scalar_inputs;
    std::vector<DoubleData*> outputs;
    std::vector<double> scalar_outputs;
    atomic_t<unsigned int> ready_inputs;
    atomic_t<unsigned int> ready_scalar_inputs;
    atomic_t<unsigned int> executing;
    tbb::spin_mutex  sm_mutex;
    Task(unsigned int num_inputs, 
        unsigned int num_scalar_inputs, 
        unsigned int num_outputs, 
        unsigned int num_scalar_outputs, 
        unsigned int static_id, 
        std::vector<unsigned int> iter_count) : static_id(static_id), 
    num_inputs(num_inputs), 
    num_scalar_inputs(num_scalar_inputs), 
    inputs(num_inputs), 
    scalar_inputs(num_scalar_inputs), 
    outputs(num_outputs), 
    scalar_outputs(num_scalar_outputs), 
    ready_inputs(0), 
    ready_scalar_inputs(0), 
    iter_count(iter_count){executing = 0;}
  public:
    void AddScalarInput(){}
    
    template<class T>
    void AddInput(T* mat){
      mat->data->incRefCount();
    }
    
    template<class T>
    void AddInput(T& mat){
      mat.data->incRefCount();
    }
    
    template<class T>
    void AddChildInput(T* mat){
      /*Add the pointers to the inputs-vector anyways, so that we can release/deallocate the inputs at the end of execute method*/
      AddInput<T>(mat);
      ready_inputs.inc();
      SpawnIfReady();
    }
    
    template<class T>
    void AddChildInput(T& mat){
      /*Add the pointers to the inputs-vector anyways, so that we can release/deallocate the inputs at the end of execute method*/
      AddInput<T>(mat);
      ready_inputs.inc();
      SpawnIfReady();
    }
   
    inline void AddChildScalarInput(){
      AddScalarInput();
      //printf("(%x) -> (%d)\n",this,ready_scalar_inputs.read());
      ready_scalar_inputs.inc();
      SpawnIfReady();
    }
    
    inline bool HasAllInputs(){
      if((ready_inputs.read()  ==  num_inputs) &&  (ready_scalar_inputs.read()  ==  num_scalar_inputs)){
        if(executing.inc()  ==  1){
          return true;
        }else{
          return false;
        }
      }else{
        return false;
      }
    }

    void root_inc_ref_count(){
      root->increment_ref_count();
    }
    
    void root_dec_ref_count(){
      root->decrement_ref_count();
    }

    inline void SpawnIfReady(){
      if(HasAllInputs()) {
        root_inc_ref_count();
        spawn(*this);
      }
    }

    /*Print Task Metadata*/
    void PrintTaskData(){
      /*std::cout<<"{"<<static_id<<"} {";
      for(int i = 0; i < iter_count.size(); ++i){
        std::cout<<iter_count[i]<<" ";
      }
      std::cout<<"} "<<std::endl;*/
      Debug(0).dbg_printfv(static_id, iter_count);
    }
};


class hash_func{
  public:
    static size_t hash( const TaskRef& s){
      //return s.first  + s.second.size();
      //cout<<"Hash = "<<(s.first^(std::accumulate(s.second.begin(),s.second.end(),0) << 1))<<endl;
      //return s.first^(std::accumulate(s.second.begin(),s.second.end(),0) << 1);
      return (s.first + std::accumulate(s.second.begin(), s.second.end(),0));
    }
    static bool equal( const TaskRef& t1, const TaskRef& t2){
      return ((t1.first == t2.first) && (std::equal(t1.second.begin(),t1.second.end(),t2.second.begin())));
    }
};

typedef tbb::concurrent_hash_map<TaskRef , Task*, hash_func> TaskMap;
extern TaskMap task_map;

inline void AddTask(TaskRef taskRef, Task* task){
  task_map.insert(std::make_pair(taskRef, task));
}

template<class T>
void PrintTaskTotalTime(){
  cout<<"Task Cumulative time = "<<T::cumulative_task_time<<endl;
}

template<class T>
void EnsureTask(TaskRef taskRef){
  TaskMap::accessor task_acc;
  task_map.find(task_acc, taskRef);
  if(task_acc.empty()){
    /*Task* task;
    task  = new(tbb::task::allocate_root()) T(taskRef.second);
    AddTask(taskRef, task);*/
    AddTask(taskRef, new(tbb::task::allocate_root()) T(taskRef.second));
  }
}

template<class T>
T* GetTask(TaskRef taskRef){
  TaskMap::accessor task_acc;
  task_map.find(task_acc, taskRef);
  if (task_acc.empty()) {
    cout<<"GetTask()---No Task Found in Map"<<endl;
    return NULL;
  }else {
    return static_cast<T*>(task_acc->second);
  }
}

template<class T>
T* GetTask(int s_id, const iteration_vector& iter_id ){
  iteration_vector  d_id  = iter_id;
  TaskRef nextTask  = make_pair(s_id, iter_id);
  EnsureTask<T>(nextTask);
  TaskMap::const_accessor task_acc;
  task_map.find(task_acc, nextTask);
  if (task_acc.empty()) {
    return NULL;
  }else {
    return static_cast<T*>(task_acc->second);
  }
}

} // namepace Tasks

#endif // ifndef _Task_h
