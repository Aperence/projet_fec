export LD_LIBRARY_PATH=$HOME/local/lib:$LD_LIBRARY_PATH
export CFLAGS='-gdwarf-2 -gstrict-dwarf'    # comply to use older dwarf version
make check
make clean
