  #include <mex.h>
  #include <iostream>
  #include <vector>
  #include <assert.h>
  #include <starpu.h>
  #include <tbb/concurrent_hash_map.h>
  #include "debug.h"
  #include "Task.h"
  #include <armadillo>
  #include "CTask.h"
  #include "atomic.h"
  #define END 32000
  using namespace std;
  using namespace tbb;
  using namespace Tasks;
  class CTask_task0;
  class CTask_task1;
  class CTask_task2;
  class CTask_task3;
  class CTask_task4;
  class CTask_task5;
  class CTask_task6;
  class CTask_task7;
  class CTask_task8;
  class CTask_END;
  void mexFunction(int nlhs,mxArray* plhs[],int nrhs,const mxArray* prhs[]){
    int ret;
    DoubleMat param_A$0(mxGetPr((mxArray*)prhs[0]),n,n);
    double param_k$0 = mxGetScalar(prhs[1]) ;
    DoubleMat param_v$0(mxGetPr((mxArray*)prhs[2]),n,1);
    double time_start,time_end,time_elapsed;
    ret=starpu_init(NULL);
    //if (ret == -ENODEV)
    //    return 77;
    STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");
    time_start  =  starpu_timing_now();
    /* This is TBB implementation
        CTask_task0& root  =  *new(tbb::task::allocate_root()) CTask_task0({0}) ;
        Tasks::root  =  &root ;
        root.Add_A$0(param_A$0);
        root.Add_v$0(param_v$0);
        root.Add_k$0(param_k$0);
    */
    CTask_task0 *task = new CTask_task0({0});
    
    task->initialize_codelet();
    task->launch_task<CTask_task0>(task);
    task->Add_A$0(param_A$0);
    task->Add_v$0(param_v$0);
    task->Add_k$0(param_k$0);
    task->submit_task();
    /* This is StarPU implementation
     * Do Something to launch StarPU_Tasks 
     */
    /* TBB spawn root and wait
     * tbb::task::spawn_root_and_wait(root);
     */
    starpu_task_wait_for_all();
    time_end  =  starpu_timing_now() ;
    starpu_shutdown();
    time_elapsed = (time_end-time_start);
    printf("%g secs\n",time_elapsed/1000000.0);
    plhs[0]  =  mxCreateDoubleMatrix(n,k,mxREAL) ;
    memcpy(mxGetPr(plhs[0]),final_outputs[0]->data,(n*k) * sizeof(double));
    plhs[1]  =  mxCreateDoubleMatrix(k,k,mxREAL) ;
    memcpy(mxGetPr(plhs[1]),final_outputs[1]->data,(k*k) * sizeof(double));
    plhs[2]  =  mxCreateDoubleMatrix(n,1,mxREAL) ;
    memcpy(mxGetPr(plhs[2]),final_outputs[2]->data,n * sizeof(double));
    return;
  }
