CC              = g++
OPTS            = -O3 -std=c++0x -march=native -fPIC 
ARMA_INCLUDE    = /opt/armadillo/usr/include/
TBB_INCLUDE     = $(HOME)/include
RUNTIME_INCLUDE = $(HOME)/work/Y790/runtime/runtime

OBJS = CTask.o CTask_task0.o CTask_task1.o CTask_task2.o CTask_task3.o CTask_task4.o CTask_task5.o CTask_task6.o CTask_task7.o CTask_task8.o CTask_END.o

all:	Arnoldi_arm

Arnoldi_arm:	$(OBJS)
	 mex -v -f ./mexopts.sh  `pkg-config starpu-1.1 --cflags` `pkg-config starpu-1.1 --libs` -I$(RUNTIME_INCLUDE) -I$(TBB_INCLUDE) -I$(ARMA_INCLUDE) Arnoldi_arm.cpp $(OBJS)

CTask.o:	CTask.cpp
	$(CC) -c $(OPTS) -I. `pkg-config starpu-1.1 --cflags` `pkg-config starpu-1.1 --libs` -I$(RUNTIME_INCLUDE) -I$(ARMA_INCLUDE) -I$(TBB_INCLUDE) CTask.cpp -o CTask.o

CTask_task0.o:	CTask_task0.cpp
	$(CC) -c $(OPTS) -I. `pkg-config starpu-1.1 --cflags` `pkg-config starpu-1.1 --libs` -I$(RUNTIME_INCLUDE) -I$(ARMA_INCLUDE) -I$(TBB_INCLUDE) CTask_task0.cpp -o CTask_task0.o

CTask_task1.o:	CTask_task1.cpp
	$(CC) -c $(OPTS) -I. `pkg-config starpu-1.1 --cflags` `pkg-config starpu-1.1 --libs` -I$(RUNTIME_INCLUDE) -I$(ARMA_INCLUDE) -I$(TBB_INCLUDE) CTask_task1.cpp -o CTask_task1.o

CTask_task2.o:	CTask_task2.cpp
	$(CC) -c $(OPTS) -I. `pkg-config starpu-1.1 --cflags` `pkg-config starpu-1.1 --libs` -I$(RUNTIME_INCLUDE) -I$(ARMA_INCLUDE) -I$(TBB_INCLUDE) CTask_task2.cpp -o CTask_task2.o

CTask_task3.o:	CTask_task3.cpp
	$(CC) -c $(OPTS) -I. `pkg-config starpu-1.1 --cflags` `pkg-config starpu-1.1 --libs` -I$(RUNTIME_INCLUDE) -I$(ARMA_INCLUDE) -I$(TBB_INCLUDE) CTask_task3.cpp -o CTask_task3.o

CTask_task4.o:	CTask_task4.cpp
	$(CC) -c $(OPTS) -I. `pkg-config starpu-1.1 --cflags` `pkg-config starpu-1.1 --libs` -I$(RUNTIME_INCLUDE) -I$(ARMA_INCLUDE) -I$(TBB_INCLUDE) CTask_task4.cpp -o CTask_task4.o

CTask_task5.o:	CTask_task5.cpp
	$(CC) -c $(OPTS) -I. `pkg-config starpu-1.1 --cflags` `pkg-config starpu-1.1 --libs` -I$(RUNTIME_INCLUDE) -I$(ARMA_INCLUDE) -I$(TBB_INCLUDE) CTask_task5.cpp -o CTask_task5.o

CTask_task6.o:	CTask_task6.cpp
	$(CC) -c $(OPTS) -I. `pkg-config starpu-1.1 --cflags` `pkg-config starpu-1.1 --libs` -I$(RUNTIME_INCLUDE) -I$(ARMA_INCLUDE) -I$(TBB_INCLUDE) CTask_task6.cpp -o CTask_task6.o

CTask_task7.o:	CTask_task7.cpp
	$(CC) -c $(OPTS) -I. `pkg-config starpu-1.1 --cflags` `pkg-config starpu-1.1 --libs` -I$(RUNTIME_INCLUDE) -I$(ARMA_INCLUDE) -I$(TBB_INCLUDE) CTask_task7.cpp -o CTask_task7.o

CTask_task8.o:	CTask_task8.cpp
	$(CC) -c $(OPTS) -I. `pkg-config starpu-1.1 --cflags` `pkg-config starpu-1.1 --libs` -I$(RUNTIME_INCLUDE) -I$(ARMA_INCLUDE) -I$(TBB_INCLUDE) CTask_task8.cpp -o CTask_task8.o

CTask_END.o:	CTask_END.cpp
	$(CC) -c $(OPTS) -I. `pkg-config starpu-1.1 --cflags` `pkg-config starpu-1.1 --libs` -I$(RUNTIME_INCLUDE) -I$(ARMA_INCLUDE) -I$(TBB_INCLUDE) CTask_END.cpp -o CTask_END.o

clean:
	rm -rf *.o Arnoldi_arm.mexa64
