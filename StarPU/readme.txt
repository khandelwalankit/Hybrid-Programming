To compile any file without using tasking feature of StarPU
gcc <file_name>.c -o <Output File>

To compile any file using gcc plugins and tasking concepts of StarPu
gcc `pkg-config starpu-1.1 --cflags` <file_name>.c -fplugin=`pkg-config starpu-1.1 --variable=gccplugin` `pkg-config starpu-1.1 --libs` -o <output_file_name>

