Y790
====

This repository contains details related to Hybrid CPU/GPU TaskParallelization.

Wiki Pages: 
1) Added a Page on Summary of the Research report on StarPU in wiki.
2) Added summary of Phalanx programming model (Rough).

Software: STARPu - http://runtime.bordeaux.inria.fr/StarPU/starpu.html

StarPU dependencies:hwloc and tetex-live.

Modify makefile of tetex-live with appropriate path(manual modification) in version 3.0.

Environment variables:
C_INCLUDE_PATH, CPLUS_INCLUDE_PATH, LD_LIBRARY_PATH, PKG_CONFIG_PACKAGE, PATH.

Installation procedure:
1) ./configure --prefix=/destdir
2) make DESTDIR=/destdir
3) make install

