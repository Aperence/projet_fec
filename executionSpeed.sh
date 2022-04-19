export CFLAGS='-gdwarf-2 -gstrict-dwarf'
make
N=50
TIMESTART=$(date +%s.%N)
for ((i = 0 ; i <= $N ; i++));
    do ./fec ./binary_exemple -f here.txt
done
TIMEEND=$(date +%s.%N)
TIME=$(echo "($TIMEEND-$TIMESTART)/$N * 1000.0" | bc -l)
echo Execution time :  $TIME ms
make clean