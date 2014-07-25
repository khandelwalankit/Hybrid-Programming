#CXX					=	g++
CXX					=	icc
#CXX_FLAGS		=	-std=c++11 -march=native -O3 -D_DEBUG -D_DEBUG_LEVEL=0
#CXX_FLAGS		=	-std=c++11 -march=native -O3 
#CXX_FLAGS		=	-std=c++11 -march=native -O3  -ansi-alias -g -shared-intel -debug inline-debug-info 
CXX_FLAGS		=	-std=c++11 -march=native -O3  -ansi-alias -shared-intel 
ARMA_INCLUDE   =  /opt/armadillo/usr/include/
TBB_INCLUDE    =  $(HOME)/Downloads/tbb40_20120613oss/include
TBB_INCLUDE   =	/opt/intel/tbb/include 
TBB_LIBDIR		=	/opt/intel/tbb/lib/intel64/gcc4.4

LAPACK_LIBDIR  =  /usr/lib64
TBB_LIBS        =  -ltbb -ltbbmalloc

all: libruntime.so

libruntime.so: Task.cpp
	$(CXX)	$(CXX_FLAGS) `pkg-config starpu-1.1 --cflags` `pkg-config starpu-1.1 --libs` -I. -I$(ARMA_INCLUDE) -I$(TBB_INCLUDE) -Wall -fPIC -c Task.cpp -o Task.o
	$(CXX) `pkg-config starpu-1.1 --libs` -L$(TBB_LIBDIR) $(TBB_LIBS) -shared -Wl,-soname,libruntime.so -o libruntime.so Task.o

clean:
	rm *.o *.so
