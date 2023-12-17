#!/bin/sh

TESTS=10000

function run_tests {
    for ((i=0;i<TESTS;i++)); do
        ti=`python gentest.py`
        ans1=`printf "%s" "$ti" | ./bf`
        ans2=`printf "%s" "$ti" | ./task`
        ans3=`printf "%s" "$ti" | ./bf2`
        if [ "$ans1" != "$ans2" || "$ans1" != "$ans3" ]; then
            echo "$i WA"
            printf "input:\n%s\n" "$ti"
            printf "bruteforce 1 output:\n%s\n" "$ans1"
            printf "bruteforce 2 output:\n%s\n" "$ans3"
            printf "dp output:\n%s\n" "$ans2"
            exit 1
        else
            echo "$i ok"
        fi
    done
}

THREADS=12
for i in `seq $THREADS`; do
    run_tests &
done
wait

