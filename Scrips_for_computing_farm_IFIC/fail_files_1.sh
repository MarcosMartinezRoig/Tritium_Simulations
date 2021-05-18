#!/bin/bash

Time=600
Activity=450

RunNumber1=$1
RunNumber2=$2
RunNumber3=$3
RunNumber4=$4
RunNumber5=$5
RunNumber6=$6
RunNumber7=$7
RunNumber8=$8
RunNumber9=$9


echo "RunNumber: " $RunNumber1

./MultipleFibbers run1.mac $RunNumber1 0 $Time $Activity $RANDOM

root -l .x Analise.C\(\"MultipleFibbers_"$Activity"BqL_Run_"$RunNumber1".root\"\)

echo "RunNumber: " $RunNumber2

./MultipleFibbers run1.mac $RunNumber2 0 $Time $Activity $RANDOM

root -l .x Analise.C\(\"MultipleFibbers_"$Activity"BqL_Run_"$RunNumber2".root\"\)

echo "RunNumber: " $RunNumber3

./MultipleFibbers run1.mac $RunNumber3 0 $Time $Activity $RANDOM

root -l .x Analise.C\(\"MultipleFibbers_"$Activity"BqL_Run_"$RunNumber3".root\"\)

echo "RunNumber: " $RunNumber4

./MultipleFibbers run1.mac $RunNumber4 0 $Time $Activity $RANDOM

root -l .x Analise.C\(\"MultipleFibbers_"$Activity"BqL_Run_"$RunNumber4".root\"\)

echo "RunNumber: " $RunNumber5

./MultipleFibbers run1.mac $RunNumber5 0 $Time $Activity $RANDOM

root -l .x Analise.C\(\"MultipleFibbers_"$Activity"BqL_Run_"$RunNumber5".root\"\)

echo "RunNumber: " $RunNumber6

./MultipleFibbers run1.mac $RunNumber6 0 $Time $Activity $RANDOM

root -l .x Analise.C\(\"MultipleFibbers_"$Activity"BqL_Run_"$RunNumber6".root\"\)

echo "RunNumber: " $RunNumber7

./MultipleFibbers run1.mac $RunNumber7 0 $Time $Activity $RANDOM

root -l .x Analise.C\(\"MultipleFibbers_"$Activity"BqL_Run_"$RunNumber7".root\"\)

echo "RunNumber: " $RunNumber8

./MultipleFibbers run1.mac $RunNumber8 0 $Time $Activity $RANDOM

root -l .x Analise.C\(\"MultipleFibbers_"$Activity"BqL_Run_"$RunNumber8".root\"\)

echo "RunNumber: " $RunNumber9

./MultipleFibbers run1.mac $RunNumber9 0 $Time $Activity $RANDOM

root -l .x Analise.C\(\"MultipleFibbers_"$Activity"BqL_Run_"$RunNumber9".root\"\)





