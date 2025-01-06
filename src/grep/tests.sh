#!/bin/bash
SUCCESS_COUNT=0
FAIL_COUNT=0
DIFF_RES=""

declare -a tests=(
"hello test1.txt VAR"
"VAR hello test1.txt"
"hello VAR test1.txt test.txt"
"VAR -e sed -e hello test1.txt test.txt"
"VAR -f regexes.txt test1.txt"
"-fregexes.txt VAR test1.txt test.txt"
)

declare -a plus_tests=(
"hello -ivclnho test.txt test1.txt"
"-lc hello test1.txt test.txt"
"-e 'sd\|e' test1.txt"
"test.txt -ehello -fregexes.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_grep $t > test_s21_grep.txt
    grep $t > test_grep.txt
    DIFF_RES="$(diff -s test_s21_grep.txt test_grep.txt)"

    if [ "$DIFF_RES" == "Files test_s21_grep.txt and test_grep.txt are identical" ]
    then
      (( SUCCESS_COUNT++ ))
        echo "\033[32mOK\033[0m s21_grep $t"
    else
      (( FAIL_COUNT++ ))
        echo "\033[31mFAILED\033[0m s21_grep $t"
    fi
    rm test_s21_grep.txt test_grep.txt
}

for var1 in i v c l n h o
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

for i in "${plus_tests[@]}"
do
    var="-"
    testing $i
done

echo "-------------------------"
echo "succes: $SUCCESS_COUNT"
echo "fail: $FAIL_COUNT"
echo "-------------------------"
if [ "$FAIL_COUNT" == 0 ]
then 
    echo "result: \033[32mOK\033[0m"
else 
    echo "result: \033[31mFAILED\033[0m"
fi
