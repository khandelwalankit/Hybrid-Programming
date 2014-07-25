#
# mexopts.sh	Shell script for configuring MEX-file creation script,
#               mex.  These options were tested with the specified compiler.
#
# usage:        Do not call this file directly; it is sourced by the
#               mex shell script.  Modify only if you don't like the
#               defaults after running mex.  No spaces are allowed
#               around the '=' in the variable assignment.
#
# Note: For the version of system compiler supported with this release,
#       refer to the Supported and Compatible Compiler List at:
#       http://www.mathworks.com/support/compilers/current_release/
#
#
# SELECTION_TAGs occur in template option files and are used by MATLAB
# tools, such as mex and mbuild, to determine the purpose of the contents
# of an option file. These tags are only interpreted when preceded by '#'
# and followed by ':'.
#
#SELECTION_TAG_MEX_OPT: Template Options file for building gcc MEX-files
#
# Copyright 1984-2011 The MathWorks, Inc.
# $Revision: 1.78.4.17 $  $Date: 2011/04/16 06:07:44 $
#----------------------------------------------------------------------------
#
    TMW_ROOT="$MATLAB"
    MFLAGS=''
    if [ "$ENTRYPOINT" = "mexLibrary" ]; then
        MLIBS="-L$TMW_ROOT/bin/$Arch -lmx -lmex -lmat -lmwservices -lut"
    else  
        MLIBS="-L$TMW_ROOT/bin/$Arch -lmx -lmex -lmat"
    fi
    case "$Arch" in
        Undetermined)
#----------------------------------------------------------------------------
# Change this line if you need to specify the location of the MATLAB
# root directory.  The script needs to know where to find utility
# routines so that it can determine the architecture; therefore, this
# assignment needs to be done while the architecture is still
# undetermined.
#----------------------------------------------------------------------------
            MATLAB="$MATLAB"
            ;;
        glnx86)
#----------------------------------------------------------------------------
            RPATH="-Wl,-rpath-link,$TMW_ROOT/bin/$Arch"
            # StorageVersion: 1.0
            # CkeyName: GNU C
            # CkeyManufacturer: GNU
            # CkeyLanguage: C
            # CkeyVersion:
            # CkeyLinkerName: GNU ld
            # CkeyLinkerVersion:
            CC='gcc'
            CFLAGS='-ansi -D_GNU_SOURCE'
            CFLAGS="$CFLAGS -fPIC -pthread -m32"
            CFLAGS="$CFLAGS  -fexceptions"
            CFLAGS="$CFLAGS -D_FILE_OFFSET_BITS=64" 
            CLIBS="$RPATH $MLIBS -lm"
            COPTIMFLAGS='-O2 -DNDEBUG'
            CDEBUGFLAGS='-g'
            CLIBS="$CLIBS -lstdc++"
#           
            # C++keyName: GNU C++
            # C++keyManufacturer: GNU
            # C++keyLanguage: C++
            # C++keyVersion: 
            # C++keyLinkerName: GNU ld
            # C++keyLinkerVersion: 
            CXX='g++'
            CXXFLAGS='-ansi -D_GNU_SOURCE -std=c++11'
            CXXFLAGS="$CXXFLAGS -D_FILE_OFFSET_BITS=64" 
            CXXFLAGS="$CXXFLAGS -fPIC -pthread"
            CXXLIBS="$RPATH $MLIBS -ltbb -ltbbmalloc -lm -lruntime"
            CXXOPTIMFLAGS='-O2 -DNDEBUG'
            CXXDEBUGFLAGS='-g'
#
#
            # FortrankeyName: gfortran
            # FortrankeyManufacturer: GNU
            # FortrankeyLanguage: Fortran
            # FortrankeyVersion:
            # FortrankeyLinkerName: GNU ld
            # FortrankeyLinkerVersion: 
            FC='gfortran'
            FFLAGS='-fexceptions -fbackslash'
            FFLAGS="$FFLAGS -fPIC"
            FLIBS="$RPATH $MLIBS -lm"
            FOPTIMFLAGS='-O3'
            FDEBUGFLAGS='-g'
#
            TBB_LIB_DIR='/home/pratnali/Downloads/tbb40_20120613oss/build/linux_intel64_gcc_cc4.6.3_libc2.15_kernel3.6.11_release'
            RUNTIME_LIBDIR='/home/amkhande/work/Y790/runtime/runtime'
            LD="$COMPILER"
            LDEXTENSION='.mexglx'
            LDFLAGS="-pthread -shared -m32 -Wl,--version-script,$TMW_ROOT/extern/lib/$Arch/$MAPFILE -Wl,--no-undefined $TBB_LIB_DIR"
            LDOPTIMFLAGS='-O2'
            LDDEBUGFLAGS='-g'
#
            POSTLINK_CMDS=':'
#----------------------------------------------------------------------------
            ;;
        glnxa64)
#----------------------------------------------------------------------------
            RPATH="-Wl,-rpath-link,$TMW_ROOT/bin/$Arch"
            # StorageVersion: 1.0
            # CkeyName: GNU C
            # CkeyManufacturer: GNU
            # CkeyLanguage: C
            # CkeyVersion:
            # CkeyLinkerName: GNU ld
            # CkeyLinkerVersion:
            CC='gcc'
            CFLAGS='-ansi -D_GNU_SOURCE'
            CFLAGS="$CFLAGS  -fexceptions"
            CFLAGS="$CFLAGS -fPIC -fno-omit-frame-pointer -pthread"
            CLIBS="$RPATH $MLIBS -lm"
            COPTIMFLAGS='-O2 -DNDEBUG'
            CDEBUGFLAGS='-g'
            CLIBS="$CLIBS -lstdc++"
#
            # C++keyName: GNU C++
            # C++keyManufacturer: GNU
            # C++keyLanguage: C++
            # C++keyVersion:
            # C++keyLinkerName: GNU ld
            # C++keyLinkerVersion:  
            CXX='g++'
            CXXINCLUDE='-I/home/pratnali/Downloads/tbb40_20120613oss/include'
            TBBLIBDIR='-L/home/pratnali/Downloads/tbb40_20120613oss/build/linux_intel64_gcc_cc4.6.3_libc2.15_kernel3.6.11_release'
            RUNTIME_LIBDIR='-L/home/amkhande/work/Y790/runtime/runtime'
            BLASLIBDIR='-L/lib64'
            MKL_LIBDIR="-L$MKL_HOME"
            OMP_LIBDIR="-L$OMP_HOME"
            ARMA_LIBDIR='-L/opt/armadillo/usr/lib64'
            LAPACKLIBDIR='-L/usr/lib64'
            CXXFLAGS='-ansi -D_GNU_SOURCE -std=c++0x'
            CXXFLAGS="$CXXFLAGS -fPIC -fno-omit-frame-pointer -pthread $CXXINCLUDE"
            #CXXLIBS="$RPATH $MLIBS -lm -lrt $TBBLIBDIR -ltbb -ltbbmalloc $BLASLIBDIR -lopenblas $LAPACKLIBDIR -llapack"
            #CXXLIBS="$TBBLIBDIR -ltbb -ltbbmalloc $RPATH $MLIBS -lm -lrt "
            #CXXLIBS="$RPATH $MLIBS $TBBLIBDIR -ltbb -ltbbmalloc -lm -lrt  $MKL_LIBDIR -lmkl_intel_ilp64 -lmkl_gnu_thread -lmkl_core -ldl -lgomp -lpthread"
            #CXXLIBS="$MLIBS $TBBLIBDIR -ltbb -ltbbmalloc -lm -lrt  -ldl libmkl_rt.so -DMKL_INTERFACE_ILP64"
            #CXXLIBS="$MLIBS $TBBLIBDIR -ltbb -ltbbmalloc -lm -lrt  -ldl libmkl_rt.so"
            CXXLIBS="$RPATH $MLIBS $TBBLIBDIR $MKL_LIBDIR $OMP_LIBDIR $RUNTIME_LIBDIR"
            CXXOPTIMFLAGS='-O2 -DNDEBUG'
            CXXDEBUGFLAGS='-g'

#
            # FortrankeyName: gfortran
            # FortrankeyManufacturer: GNU
            # FortrankeyLanguage: Fortran
            # FortrankeyVersion:
            # FortrankeyLinkerName: GNU ld
            # FortrankeyLinkerVersion:  
#
            FC='gfortran'
            FFLAGS='-fexceptions -fbackslash'
            FFLAGS="$FFLAGS -fPIC -fno-omit-frame-pointer"
            FLIBS="$RPATH $MLIBS -lm"
            FOPTIMFLAGS='-O'
            FDEBUGFLAGS='-g'
#
            LD="$COMPILER"
            LDEXTENSION='.mexa64'
            #LDFLAGS="-pthread -shared -Wl,--version-script,$TMW_ROOT/extern/lib/$Arch/$MAPFILE -Wl,--no-undefined -ltbb -ltbbmalloc -lm -lrt -ldl -larmadillo -lmkl_intel_ilp64 -lmkl_intel_thread -lmkl_core -liomp5"
            LDFLAGS="-pthread -shared -Wl,--version-script,$TMW_ROOT/extern/lib/$Arch/$MAPFILE -Wl,--no-undefined -ltbb -ltbbmalloc -lm -lrt -ldl -lmkl_intel_lp64 -lmkl_intel_thread -lmkl_core -liomp5 -lruntime"
            #LDFLAGS="-pthread -shared -Wl,--version-script,$TMW_ROOT/extern/lib/$Arch/$MAPFILE -Wl,--no-undefined -ltbb -ltbbmalloc -lm -lrt -ldl -lmkl_rt"
            LDOPTIMFLAGS='-O'
            LDDEBUGFLAGS='-g'
#
            POSTLINK_CMDS=':'
#----------------------------------------------------------------------------
            ;;
        sol64)
#----------------------------------------------------------------------------
echo "Error: Did not imbed 'options.sh' code"; exit 1 #imbed options.sh sol64 12
#----------------------------------------------------------------------------
            ;;
        mac)
#----------------------------------------------------------------------------
echo "Error: Did not imbed 'options.sh' code"; exit 1 #imbed options.sh mac 12
#----------------------------------------------------------------------------
            ;;
        maci)
#----------------------------------------------------------------------------
echo "Error: Did not imbed 'options.sh' code"; exit 1 #imbed options.sh maci 12
#----------------------------------------------------------------------------
            ;;
        maci64)
#----------------------------------------------------------------------------
            # StorageVersion: 1.0
            # CkeyName: GNU C
            # CkeyManufacturer: GNU
            # CkeyLanguage: C
            # CkeyVersion:
            # CkeyLinkerName:
            # CkeyLinkerVersion:
            CC='gcc-4.6.3'
            SDKROOT='/Developer/SDKs/MacOSX10.6.sdk'
            MACOSX_DEPLOYMENT_TARGET='10.5'
            ARCHS='x86_64'
            CFLAGS="-fno-common -no-cpp-precomp -arch $ARCHS -isysroot $SDKROOT -mmacosx-version-min=$MACOSX_DEPLOYMENT_TARGET"
            CFLAGS="$CFLAGS  -fexceptions"
            CLIBS="$MLIBS"
            COPTIMFLAGS='-O3 -DNDEBUG'
            CDEBUGFLAGS='-g'
#
            CLIBS="$CLIBS -lstdc++"
            # C++keyName: GNU C++
            # C++keyManufacturer: GNU
            # C++keyLanguage: C++
            # C++keyVersion:
            # C++keyLinkerName:
            # C++keyLinkerVersion:
            CXX=g++-4.2
            CXXFLAGS="-fno-common -no-cpp-precomp -fexceptions -arch $ARCHS -isysroot $SDKROOT -mmacosx-version-min=$MACOSX_DEPLOYMENT_TARGET"
            CXXLIBS="$MLIBS -lstdc++"
            CXXOPTIMFLAGS='-O2 -DNDEBUG'
            CXXDEBUGFLAGS='-g'
#
            # FortrankeyName: GNU Fortran
            # FortrankeyManufacturer: GNU
            # FortrankeyLanguage: Fortran
            # FortrankeyVersion: 
            # FortrankeyLinkerName: 
            # FortrankeyLinkerVersion:
            FC='gfortran'
            FFLAGS='-fexceptions -m64 -fbackslash'
            FC_LIBDIR=`$FC -print-file-name=libgfortran.dylib 2>&1 | sed -n '1s/\/*libgfortran\.dylib//p'`
            FC_LIBDIR2=`$FC -print-file-name=libgfortranbegin.a 2>&1 | sed -n '1s/\/*libgfortranbegin\.a//p'`
            FLIBS="$MLIBS -L$FC_LIBDIR -lgfortran -L$FC_LIBDIR2 -lgfortranbegin"
            FOPTIMFLAGS='-O'
            FDEBUGFLAGS='-g'
#
            LD="$CC"
            LDEXTENSION='.mexmaci64'
            LDFLAGS="-Wl,-twolevel_namespace -undefined error -arch $ARCHS -Wl,-syslibroot,$SDKROOT -mmacosx-version-min=$MACOSX_DEPLOYMENT_TARGET"
            LDFLAGS="$LDFLAGS -bundle -Wl,-exported_symbols_list,$TMW_ROOT/extern/lib/$Arch/$MAPFILE"
            LDOPTIMFLAGS='-O'
            LDDEBUGFLAGS='-g'
#
            POSTLINK_CMDS=':'
#----------------------------------------------------------------------------
            ;;
    esac
#############################################################################
#
# Architecture independent lines:
#
#     Set and uncomment any lines which will apply to all architectures.
#
#----------------------------------------------------------------------------
#           CC="$CC"
#           CFLAGS="$CFLAGS"
#           COPTIMFLAGS="$COPTIMFLAGS"
#           CDEBUGFLAGS="$CDEBUGFLAGS"
#           CLIBS="$CLIBS"
#
#           FC="$FC"
#           FFLAGS="$FFLAGS"
#           FOPTIMFLAGS="$FOPTIMFLAGS"
#           FDEBUGFLAGS="$FDEBUGFLAGS"
#           FLIBS="$FLIBS"
#
#           LD="$LD"
#           LDFLAGS="$LDFLAGS"
#           LDOPTIMFLAGS="$LDOPTIMFLAGS"
#           LDDEBUGFLAGS="$LDDEBUGFLAGS"
#----------------------------------------------------------------------------
#############################################################################
