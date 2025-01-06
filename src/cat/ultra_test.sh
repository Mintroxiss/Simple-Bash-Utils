#!/bin/bash

SUCCESS=0
FAIL=0
make s21_cat
TEST_FILES=$(cd test_files && ls)

for file in $TEST_FILES
do
    for flag in -b -n -e -s -v -t
    do

        cat $flag test_files/"$file" > test_real.txt
        ./s21_cat $flag test_files/"$file" > test_to_check.txt
        RES=$(diff -s test_real.txt test_to_check.txt)
        if [ "$RES" == "Files test_real.txt and test_to_check.txt are identical" ]
        then 
        (( SUCCESS++ ))
        echo "$flag $file good"
        else
        (( FAIL++ ))
        echo "$flag $file shit"
        fi
        rm -rf test_real.txt test_to_check.txt
    done
done

for file in $TEST_FILES
do
    for flag in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
    do
        for flag2 in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
        do
                if [ $flag != $flag2 ]
                then
                cat $flag $flag2 test_files/"$file" > test_real.txt
                ./s21_cat $flag $flag2 test_files/"$file" > test_to_check.txt
                RES=$(diff -s test_real.txt test_to_check.txt)
                if [ "$RES" == "Files test_real.txt and test_to_check.txt are identical" ]
                then 
                (( SUCCESS++ ))
                echo "$flag $flag2 $file good"
                else
                (( FAIL++ ))
                echo "$flag $flag2 $file shit"
                fi
                rm -rf test_real.txt test_to_check.txt
                fi
        done
    done
done

for file in $TEST_FILES
do
for flag in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
do
  for flag2 in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
  do
      for flag3 in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
      do
        if [ $flag != $flag2 ] && [ $flag2 != $flag3 ] && [ $flag != $flag3 ]
        then
            cat $flag $flag2 $flag3 test_files/"$file" > test_real.txt
        ./s21_cat $flag $flag2 $flag3 test_files/"$file" > test_to_check.txt
        RES=$(diff -s test_real.txt test_to_check.txt)
        if [ "$RES" == "Files test_real.txt and test_to_check.txt are identical" ]
        then 
        (( SUCCESS++ ))
        echo "$flag $flag2 $flag3 $file good"
        else
        (( FAIL++ ))
        echo "$flag $flag2 $flag3 $file shit"
        fi
        rm -rf test_real.txt test_to_check.txt
        fi
      done
  done
done
done

for file in $TEST_FILES
do
for flag in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
do
  for flag2 in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
  do
      for flag3 in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
      do
          for flag4 in -b -e -n -s -t -v #-E -T --number-nonblank --number --squeeze-blank
          do
            if [ $flag != $flag2 ] && [ $flag2 != $flag3 ] && [ $flag != $flag3 ] && [ $flag != $flag4 ] && [ $flag2 != $flag4 ] && [ $flag3 != $flag4 ]
            then
                cat $flag $flag2 $flag3 $flag4 test_files/"$file" > test_real.txt
        ./s21_cat $flag $flag2 $flag3 $flag4 test_files/"$file" > test_to_check.txt
        RES=$(diff -s test_real.txt test_to_check.txt)
        if [ "$RES" == "Files test_real.txt and test_to_check.txt are identical" ]
        then 
        (( SUCCESS++ ))
        echo "$flag $flag2 $flag3 $flag4 $file good"
        else
        (( FAIL++ ))
        echo "$flag $flag2 $flag3 $flag4 $file shit"
        fi
        rm -rf test_real.txt test_to_check.txt
            fi
          done
      done
  done
done
done

make clean

echo "$SUCCESS <- ACCEPTED"
echo "$FAIL <- FAILED"
echo "`expr $SUCCESS + $FAIL` <- ALL"