To compile any file without using tasking feature of StarPU
gcc <file_name>.c -o <Output File>

To compile any file with StarPu
gcc `pkg-config starpu-1.1 --cflags` -O3 <file_name>.c  `pkg-config starpu-1.1 --libs` -o <output_file_name>

To compile any file with StarPu and OpenMP
gcc `pkg-config starpu-1.1 --cflags` -O3 -fopenmp <file_name>.c  `pkg-config starpu-1.1 --libs` -o <output_file_name>


