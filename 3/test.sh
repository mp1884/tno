#!/bin/sh

for test_path in tests/t*; do
    test_name=`echo "$test_path" | rev | cut -d"/" -f1 | rev`
    ans_path="tests/ans_${test_name}"
    output=`./task < "$test_path"`
    if ! diff -u --color "$ans_path" <(printf "%s\n" "$output"); then
        printf "%s WA\n" "$test_path"
        exit 1
    fi
    printf "%s ok\n" "$test_path"
done

