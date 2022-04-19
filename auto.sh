export LD_LIBRARY_PATH=$HOME/local/lib:$LD_LIBRARY_PATH
export CFLAGS='-gdwarf-2 -gstrict-dwarf'
make tests
make check
make clean
