
beginTime=$(date +%s%N)
    ./test1
    ./test2

endTime=$(date +%s%N)
    elapsed=`echo "($endTime - $beginTime) / 1000000" | bc`
    elapsedSec=`echo "scale=6;$elapsed / 1000" | bc | awk '{printf "%.6f", $1}'`
    echo TOTAL: $elapsedSec sec >> notime1.txt


