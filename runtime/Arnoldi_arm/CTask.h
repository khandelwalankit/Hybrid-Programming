#ifndef _CTASK_H_
#define _CTASK_H_
#include "Task.h"
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
using namespace Tasks;
extern std::vector<DoubleData*> final_outputs;
extern std::vector<double> final_scalar_outputs;
extern int n;
extern int k;
extern void execute0(__attribute__((unused))void *descr[], void *arg);
extern void execute1(__attribute__((unused))void *descr[], void *arg);
extern void execute2(__attribute__((unused))void *descr[], void *arg);
extern void execute3(__attribute__((unused))void *descr[], void *arg);
extern void execute4(__attribute__((unused))void *descr[], void *arg);
extern void execute5(__attribute__((unused))void *descr[], void *arg);
extern void execute6(__attribute__((unused))void *descr[], void *arg);
extern void execute7(__attribute__((unused))void *descr[], void *arg);
extern void execute8(__attribute__((unused))void *descr[], void *arg);
extern void execute_END(__attribute__((unused))void *descr[], void *arg);

class CTask_task0:public Task{
  public:
    int k$0;
    int j$1;
    double PARAM_tmp4$1;
    DoubleData* A$0_data;
    DoubleData* v$0_data;
    iteration_vector task_version;
    static double cumulative_task_time;
    //static tbb::tick_count start;
    //static tbb::tick_count end;
    static double start, end;
    void static start_time();
    void static end_time();
    //struct starpu_codelet *cl_ar;

    void initialize_codelet()
    {
        /*starpu_malloc((void**)&cl_ar, sizeof(struct starpu_codelet));
        cl_ar->cpu_funcs[0]=execute0;
        cl_ar->cpu_funcs[1]=NULL;
        cl_ar->where = STARPU_CPU;
        cl_ar->nbuffers = 0;*/
        default_codelet_init(execute0);
    }
    void Add_k$0(int arg){
      k$0  =  arg ;
      AddScalarInput();
    }
    void Add_A$0(DoubleMat& arg){
      A$0_data  =  std::move(arg.data) ;
      AddInput<DoubleMat>(arg);
    }
    void Add_v$0(DoubleMat& arg){
      v$0_data  =  std::move(arg.data) ;
      AddInput<DoubleMat>(arg);
    }
    CTask_task0(iteration_vector iter):Task(2,1,4,2,0,iter){
    }
    friend void execute0(__attribute__((unused))void *descr[], void *arg);
};
/*
n$1 = length(v$0);
H$1 = zeros(k$0, k$0);
V$1 = zeros(n$1, k$0);
PARAM_tmp1$1 = norm(v$0);
j$1 = 2;
PARAM_tmp4$1 = j$1 <= k$0;
*/
class CTask_task1:public Task{
  public:
    //Added Static keyword till cumulative_task_time
    int k$0;
    int n$1;
    double PARAM_tmp1$1;
    int j$1;
    double PARAM_tmp4$1;
    DoubleData* v$0_data;
    DoubleData* H$1_data;
    DoubleData* V$1_data;
    iteration_vector task_version;
    static double cumulative_task_time;
    //static tbb::tick_count start;
    //static tbb::tick_count end;
    static double start, end;
    void static start_time();
    void static end_time();
    //struct starpu_codelet *cl_ar;
    void initialize_codelet()
    {
        /*starpu_malloc((void**)&cl_ar, sizeof(struct starpu_codelet));
        cl_ar->cpu_funcs[0]=execute1;
        cl_ar->cpu_funcs[1]=NULL;
        cl_ar->where = STARPU_CPU;
        cl_ar->nbuffers = 0;*/
        default_codelet_init(execute1);
    }
    void Add_k$0(int arg){
      k$0  =  arg ;
      AddChildScalarInput();
    }
    void Add_v$0(DoubleMat& arg){
      v$0_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    CTask_task1(iteration_vector iter):Task(1,1,4,4,1,iter){
    }
    friend void execute1(__attribute__((unused))void *descr[], void *arg);
};
/*
v$1 = v$0 / PARAM_tmp1$1;
w$1 = A$0 * v$1;
j$1 = 2;
PARAM_tmp4$1 = j$1 <= k$0;
*/
class CTask_task2:public Task{
  public:
    double PARAM_tmp1$1;
    int k$0;
    int j$1;
    double PARAM_tmp4$1;
    DoubleData* v$0_data;
    DoubleData* A$0_data;
    DoubleData* v$1_data;
    DoubleData* w$1_data;
    iteration_vector task_version;
    static double cumulative_task_time;
    //static tbb::tick_count start;
    //static tbb::tick_count end;
    static double start, end;
    void static start_time();
    void static end_time();
    //struct starpu_codelet *cl_ar;
    void initialize_codelet()
    {
        /*starpu_malloc((void**)&cl_ar, sizeof(struct starpu_codelet));
        cl_ar->cpu_funcs[0]=execute2;
        cl_ar->cpu_funcs[1]=NULL;
        cl_ar->where = STARPU_CPU;
        cl_ar->nbuffers = 0;*/
        default_codelet_init(execute2);
    }
    void Add_PARAM_tmp1$1(double arg){
      PARAM_tmp1$1  =  arg ;
      AddChildScalarInput();
    }
    void Add_k$0(int arg){
      k$0  =  arg ;
      AddChildScalarInput();
    }
    void Add_v$0(DoubleMat& arg){
      v$0_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    void Add_A$0(DoubleMat& arg){
      A$0_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    CTask_task2(iteration_vector iter):Task(2,2,4,2,2,iter){
    }
    friend void execute2(__attribute__((unused))void *descr[], void *arg);
};
/*
PARAM_tmp2$1 = v$1';
alpha$1 = PARAM_tmp2$1 * w$1;
j$1 = 2;
PARAM_tmp4$1 = j$1 <= k$0;
*/
class CTask_task3:public Task{
  public:
    int k$0;
    double alpha$1;
    int j$1;
    double PARAM_tmp4$1;
    DoubleData* v$1_data;
    DoubleData* w$1_data;
    DoubleData* PARAM_tmp2$1_data;
    iteration_vector task_version;
    static double cumulative_task_time;
    //static tbb::tick_count start;
    //static tbb::tick_count end;
    static double start, end;
    void static start_time();
    void static end_time();
    //struct starpu_codelet *cl_ar;
    void initialize_codelet()
    {
        /*starpu_malloc((void**)&cl_ar, sizeof(struct starpu_codelet));
        cl_ar->cpu_funcs[0]=execute3;
        cl_ar->cpu_funcs[1]=NULL;
        cl_ar->where = STARPU_CPU;
        cl_ar->nbuffers = 0;*/
        default_codelet_init(execute3);
    }
    void Add_k$0(int arg){
      k$0  =  arg ;
      AddChildScalarInput();
    }
    void Add_v$1(DoubleMat& arg){
      v$1_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    void Add_w$1(DoubleMat& arg){
      w$1_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    CTask_task3(iteration_vector iter):Task(2,1,2,3,3,iter){
    }
    friend void execute3(__attribute__((unused))void *descr[], void *arg);
};
/*
V$1(:, 1) = v$1;
H$1(1, 1) = alpha$1;
j$1 = 2;
PARAM_tmp4$1 = j$1 <= k$0;
*/
class CTask_task4:public Task{
  public:
    double alpha$1;
    int k$0;
    int j$1;
    double PARAM_tmp4$1;
    DoubleData* V$1_data;
    DoubleData* v$1_data;
    DoubleData* H$1_data;
    iteration_vector task_version;
    static double cumulative_task_time;
    //static tbb::tick_count start;
    //static tbb::tick_count end;
    static double start, end;
    void static start_time();
    void static end_time();
    //struct starpu_codelet *cl_ar;
    void initialize_codelet()
    {
        /*starpu_malloc((void**)&cl_ar, sizeof(struct starpu_codelet));
        cl_ar->cpu_funcs[0]=execute4;
        cl_ar->cpu_funcs[1]=NULL;
        cl_ar->where = STARPU_CPU;
        cl_ar->nbuffers = 0;*/
        default_codelet_init(execute4);
    }
    void Add_alpha$1(double arg){
      alpha$1  =  arg ;
      AddChildScalarInput();
    }
    void Add_k$0(int arg){
      k$0  =  arg ;
      AddChildScalarInput();
    }
    void Add_V$1(DoubleMat& arg){
      V$1_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    void Add_v$1(DoubleMat& arg){
      v$1_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    void Add_H$1(DoubleMat& arg){
      H$1_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    CTask_task4(iteration_vector iter):Task(3,2,2,2,4,iter){
    }
    friend void execute4(__attribute__((unused))void *descr[], void *arg);
};
/*
PARAM_tmp3$1 = v$1 * alpha$1;
f$1 = w$1 - PARAM_tmp3$1;
j$1 = 2;
PARAM_tmp4$1 = j$1 <= k$0;
*/
class CTask_task5:public Task{
  public:
    double alpha$1;
    int k$0;
    int j$1;
    double PARAM_tmp4$1;
    DoubleData* v$1_data;
    DoubleData* w$1_data;
    DoubleData* f$1_data;
    DoubleData* PARAM_tmp3$1_data;
    iteration_vector task_version;
    static double cumulative_task_time;
    //static tbb::tick_count start;
    //static tbb::tick_count end;
    static double start, end;
    void static start_time();
    void static end_time();
    //struct starpu_codelet *cl_ar;
    void initialize_codelet()
    {
        /*starpu_malloc((void**)&cl_ar, sizeof(struct starpu_codelet));
        cl_ar->cpu_funcs[0]=execute5;
        cl_ar->cpu_funcs[1]=NULL;
        cl_ar->where = STARPU_CPU;
        cl_ar->nbuffers = 0;*/
        default_codelet_init(execute5);
    }
    void Add_alpha$1(double arg){
      alpha$1  =  arg ;
      AddChildScalarInput();
    }
    void Add_k$0(int arg){
      k$0  =  arg ;
      AddChildScalarInput();
    }
    void Add_v$1(DoubleMat& arg){
      v$1_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    void Add_w$1(DoubleMat& arg){
      w$1_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    CTask_task5(iteration_vector iter):Task(2,2,2,2,5,iter){
    }
    friend void execute5(__attribute__((unused))void *descr[], void *arg);
};
/*
beta$1 = norm(f$2);
v$3 = f$2 / beta$1;
PARAM_tmp5$1 = j$2 - 1;
H$1(j$2, PARAM_tmp5$1) = beta$1;
j$3 = j$2 + 1;
PARAM_tmp4$3 = j$3 <= k$0;
*/
class CTask_task6:public Task{
  public:
    int j$2;
    int k$0;
    double beta$1;
    int PARAM_tmp5$1;
    int j$3;
    bool PARAM_tmp4$3;
    DoubleData* f$2_data;
    DoubleData* H$1_data;
    DoubleData* v$3_data;
    iteration_vector task_version;
    static double cumulative_task_time;
    //static tbb::tick_count start;
    //static tbb::tick_count end;
    static double start, end;
    void static start_time();
    void static end_time();
    //struct starpu_codelet *cl_ar;
    void initialize_codelet()
    {
        /*starpu_malloc((void**)&cl_ar, sizeof(struct starpu_codelet));
        cl_ar->cpu_funcs[0]=execute6;
        cl_ar->cpu_funcs[1]=NULL;
        cl_ar->where = STARPU_CPU;
        cl_ar->nbuffers = 0;*/
        default_codelet_init(execute6);
    }
    void Add_j$2(int arg){
      j$2  =  arg ;
      AddChildScalarInput();
    }
    void Add_k$0(int arg){
      k$0  =  arg ;
      AddChildScalarInput();
    }
    void Add_f$2(DoubleMat& arg){
      f$2_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    void Add_H$1(DoubleMat& arg){
      H$1_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    CTask_task6(iteration_vector iter):Task(2,2,5,4,6,iter){
    }
    friend void execute6(__attribute__((unused))void *descr[], void *arg);
};
/*
V$1(:, j$2) = v$3;
w$2 = A$0 * v$3;
PARAM_tmp6$1 = V$1(:, 1:1:j$2)';
h$1 = PARAM_tmp6$1 * w$2;
j$3 = j$2 + 1;
PARAM_tmp4$3 = j$3 <= k$0;
*/
class CTask_task7:public Task{
  public:
    int j$2;
    int k$0;
    int j$3;
    bool PARAM_tmp4$3;
    DoubleData* V$1_data;
    DoubleData* v$3_data;
    DoubleData* A$0_data;
    DoubleData* w$2_data;
    DoubleData* h$1_data;
    DoubleData* PARAM_tmp6$1_data;
    iteration_vector task_version;
    static double cumulative_task_time;
    //static tbb::tick_count start;
    //static tbb::tick_count end;
    static double start, end;
    void static start_time();
    void static end_time();
    //struct starpu_codelet *cl_ar;
    void initialize_codelet()
    {
        /*starpu_malloc((void**)&cl_ar, sizeof(struct starpu_codelet));
        cl_ar->cpu_funcs[0]=execute7;
        cl_ar->cpu_funcs[1]=NULL;
        cl_ar->where = STARPU_CPU;
        cl_ar->nbuffers = 0;*/
        default_codelet_init(execute7);
    }
    void Add_j$2(int arg){
      j$2  =  arg ;
      AddChildScalarInput();
    }
    void Add_k$0(int arg){
      k$0  =  arg ;
      AddChildScalarInput();
    }
    void Add_V$1(DoubleMat& arg){
      V$1_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    void Add_v$3(DoubleMat& arg){
      v$3_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    void Add_A$0(DoubleMat& arg){
      A$0_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    CTask_task7(iteration_vector iter):Task(3,2,7,2,7,iter){
    }
    friend void execute7(__attribute__((unused))void *descr[], void *arg);
};
/*
PARAM_tmp7$1 = V$1(:, 1:1:j$2) * h$1;
f$3 = w$2 - PARAM_tmp7$1;
H$1(1:1:j$2, j$2) = h$1;
j$3 = j$2 + 1;
PARAM_tmp4$3 = j$3 <= k$0;
*/
class CTask_task8:public Task{
  public:
    int j$2;
    int k$0;
    int j$3;
    bool PARAM_tmp4$3;
    DoubleData* V$1_data;
    DoubleData* h$1_data;
    DoubleData* w$2_data;
    DoubleData* H$1_data;
    DoubleData* f$3_data;
    DoubleData* PARAM_tmp7$1_data;
    iteration_vector task_version;
    static double cumulative_task_time;
    //static tbb::tick_count start;
    //static tbb::tick_count end;
    static double start, end;
    void static start_time();
    void static end_time();
    //struct starpu_codelet *cl_ar;
    void initialize_codelet()
    {
        /*starpu_malloc((void**)&cl_ar, sizeof(struct starpu_codelet));
        cl_ar->cpu_funcs[0]=execute8;
        cl_ar->cpu_funcs[1]=NULL;
        cl_ar->where = STARPU_CPU;
        cl_ar->nbuffers = 0;*/
        default_codelet_init(execute8);
    }
    void Add_j$2(int arg){
      j$2  =  arg ;
      AddChildScalarInput();
    }
    void Add_k$0(int arg){
      k$0  =  arg ;
      AddChildScalarInput();
    }
    void Add_V$1(DoubleMat& arg){
      V$1_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    void Add_h$1(DoubleMat& arg){
      h$1_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    void Add_w$2(DoubleMat& arg){
      w$2_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    void Add_H$1(DoubleMat& arg){
      H$1_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    CTask_task8(iteration_vector iter):Task(4,2,5,2,8,iter){
    }
    friend void execute8(__attribute__((unused))void *descr[], void *arg);
};
/*
*/
class CTask_END:public Task{
  public:
    DoubleData* V$1_data;
    DoubleData* H$1_data;
    DoubleData* f$2_data;
    static double cumulative_task_time;
    //static tbb::tick_count start;
    //static tbb::tick_count end;
    static double start, end;
    void static start_time();
    void static end_time();
    //struct starpu_codelet *cl_ar;
    void initialize_codelet()
    {
        /*starpu_malloc((void**)&cl_ar, sizeof(struct starpu_codelet));
        cl_ar->cpu_funcs[0]=execute_END;
        cl_ar->cpu_funcs[1]=NULL;
        cl_ar->where = STARPU_CPU;
        cl_ar->nbuffers = 0;*/
        default_codelet_init(execute_END);
    }
    void Add_V$1(DoubleMat& arg){
      V$1_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    void Add_H$1(DoubleMat& arg){
      H$1_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    void Add_f$2(DoubleMat& arg){
      f$2_data  =  std::move(arg.data) ;
      AddChildInput<DoubleMat>(arg);
    }
    CTask_END(iteration_vector iter):Task(3,0,0,0,0,iter){
    }
    friend void execute_END(__attribute__((unused))void *descr[], void *arg);
};
#endif
