#ifndef _Task_h_
#define _Task_h_

#pragma once

#include <iostream>
#include <assert.h>
#include <starpu.h>
#define END 32000


namespace S-Tasks{
static struct starpu_codelet *cl_ar;
extern void execute(__attribute__((unused))void *descr[], __attribute__((unused))void *arg);
struct starpu_task *root;
inline void launch_task() 
{
    this->root=starpu_task_create();
    this->root->cl=cl_ar;
}
inline void submit_task()
{
    int ret=starpu_task_submit(this->root);
}
template<class T>
inline void initialize_codelet()
{
  starpu_malloc((void**)&cl_ar, sizeof(struct starpu_codelet));
  cl_ar->cpu_funcs[0] = T::execute; 
  cl_ar->cpu_funcs[1] = NULL;
  cl_ar->where = STARPU_CPU;
  cl_ar->nbuffers = 0;
}


} // namepace Tasks

#endif // ifndef _Task_h
