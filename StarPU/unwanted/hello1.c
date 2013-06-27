/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2012  INRIA
 *
 * StarPU is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * StarPU is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Lesser General Public License in COPYING.LGPL for more details.
 */

#include <stdio.h>

/* Task declaration. */
static void my_task (int x) __attribute__ ((task));
static void my_task1 (int x) __attribute__ ((task));
static void my_task2 (int x,int y) __attribute__ ((task));
static void my_task3 (int x,int y) __attribute__ ((task));
/* Definition of the CPU implementation of ‘my task’. */
static void my_task (int x)
{
	sleep(10);	
	printf ("Hello, world! With x = %d\n", x);
	
}

static void my_task1(int x)
{
	//sleep(20);
	printf("Again, Hello with x = %d\n", x);
}
static void my_task2(int x,int y)
{
	//sleep(10);
	int z;
	z=x*y;
	printf("Again, value of x*y = %d\n", z);
	
}
static void my_task3(int x,int y)
{
	int z;
	z=x+y;
	printf("Again, value of x+y = %d\n", z);
	//sleep(20);
}

int main ()
{
/* Initialize StarPU. */
#pragma starpu initialize

/* Do an asynchronous call to ‘my task’. */
	my_task (42);
	my_task1 (20);
	my_task2 (20,20);
	my_task3 (20,30);

/* Wait for the call to complete. */
#pragma starpu wait

/* Terminate. */
#pragma starpu shutdown
	return 0;
}

