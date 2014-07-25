#ifndef _Task_h
#define _Task_h

#include <vector>
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
#include "Data.h"
#include "debug.h"
#include <iostream>


//extern std::ofstream op_file;
tbb::spin_mutex printMutex;
tbb::spin_mutex mapSearchMutex;

using namespace arma;
 
namespace Tasks{
typedef std::vector<unsigned int>   i_vector;
typedef std::pair<int,i_vector>     TaskRef; // <static task ID, iteration vector(or 0)>
typedef Data<double>                DoubleData;
typedef Matrix<double>              DoubleMat;
typedef cube                        DoubleCube;
typedef std::vector<long long>      matrec;
tbb::task* root;

class Task : public tbb::task{
  protected:
    std::vector<unsigned int> iter_count;
    unsigned int static_id;
    unsigned int num_inputs;
    unsigned int num_scalar_inputs;
    std::vector<DoubleData*>  inputs;
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
    void AddInput(unsigned int num, DoubleMat& mat){
      inputs[num] = mat.data;
      mat.data->incRefCount();
    }
    
    void AddScalarInput(unsigned int num, double val){
      assert(num < num_scalar_inputs);
      scalar_inputs[num] = val;
    }

    // For all tasks besides the root task
    void AddChildInput(unsigned int num, DoubleMat& mat){
      /*if(static_id == 8){
        cout<<"INPUT-MAT  = \n"<<mat()<<endl;
      }*/
      AddInput(num, mat);
      ready_inputs.inc();
      SpawnIfReady();
    }

    // For all tasks besides the root task
    void AddChildScalarInput(unsigned int num, double val){
      AddScalarInput(num, val);
      ready_scalar_inputs.inc();
      SpawnIfReady();
    }

    bool HasAllInputs(){
      if((ready_inputs.read()  ==  num_inputs) &&  (ready_scalar_inputs.read()  ==  num_scalar_inputs)){
        if(executing.inc()  ==  1)
          return true;
        else
          return false;
      }else{
        return false;
      }
    }

    void SpawnIfReady(){
      if (HasAllInputs()) {
        Tasks::root->increment_ref_count();
        /*Debug(0)<<" R-Ref-Cnt = "
          <<root->ref_count()
          <<"  Spawning = "
          <<this
          <<" St = "
          <<this->state()
          //<<" Ref-Cnt = "
          //<<this->ref_count()
          <<endl;*/
        /*printMutex.unlock();*/
        spawn(*this);
      }
    }

    void ReleaseInputs(){
      for(unsigned int i = 0; i < inputs.size(); i++) {
        inputs[i]->decRefCount();
        /*matrec tmp = inputs[i];
        DoubleMat* mat_ref  = (DoubleMat*)tmp[3];
        mat_ref->decRefCount();*/

      }
    }

    /*Print Task Metadata*/
    void PrintTaskData(){
      std::cout<<"{"<<static_id<<"} {";
      /*for(auto i : iter_count){
        std::cout<<i<<" ";
      }*/
      for(int i = 0; i < iter_count.size(); ++i){
        std::cout<<iter_count[i]<<" ";
      }
      std::cout<<"} "<<std::endl;
    }
};

class hash_func {
  public:
    static size_t hash( const TaskRef& s){
      return s.first  + s.second.size();
    }

    static bool equal( const TaskRef& t1, const TaskRef& t2){
      return ((t1.first == t2.first) &&(std::equal(t1.second.begin(),t1.second.end(),t2.second.begin())));
    }
};
typedef tbb::concurrent_hash_map<TaskRef , Task*,hash_func> TaskMap;
TaskMap task_map;

void AddTask(TaskRef taskRef, Task* task){
  task_map.insert(std::make_pair(taskRef, task));
}

template <class T>
void EnsureTask(TaskRef taskRef){
  TaskMap::accessor task_acc;
  task_map.find(task_acc, taskRef);
  if (task_acc.empty()) {
    Task* task;
    task = new(tbb::task::allocate_root()) T(taskRef.second);
    AddTask(taskRef, task);
  }
}

Task* GetTask(TaskRef taskRef)
{
  TaskMap::accessor task_acc;
  task_map.find(task_acc, taskRef);
  if (task_acc.empty()) {
    return NULL;
  } else {
    return task_acc->second;
  }
}

} // namepace Tasks

#endif // ifndef _Task_h
