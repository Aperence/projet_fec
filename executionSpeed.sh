export CFLAGS='-gdwarf-2 -gstrict-dwarf'
make
N=$1
TIMESTART=$(date +%s.%N)    # Time at start of execution
for ((i = 0 ; i <= $N ; i++));
    do ./fec ./binary_exemple -f here.txt -n $2   #run N times the program
done
TIMEEND=$(date +%s.%N)
TIME=$(echo "($TIMEEND-$TIMESTART)/$N * 1000.0" | bc -l)   # get the average time taken
echo Execution time :  $TIME ms
make clean