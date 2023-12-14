#!/bin/sh

rm -f bm.txt

TIMEFORMAT=%R
for test_path in tests/t*; do
    t=$( { time ./task < "$test_path" > /dev/null; } 2>&1 )
    printf "%s\n" "$t" >> bm.txt
done

sort bm.txt -o bm.txt
awk '{s+=$1}END{print "avg:",s/NR}' RS=" " bm.txt >> bm.txt

