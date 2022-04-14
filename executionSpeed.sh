export CFLAGS='-gdwarf-2 -gstrict-dwarf'
make
N=200
TIMESTART=$EPOCHREALTIME
for ((i = 0 ; i <= $N ; i++));
    do ./fec ./binary_exemple -f here.txt
done
TIME=$(echo "($EPOCHREALTIME-$TIMESTART)/$N * 1000" | bc -l)
echo Execution time :  $TIME ms
make clean