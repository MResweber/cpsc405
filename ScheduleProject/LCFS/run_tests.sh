#!/bin/bash

make

echo "Start test 1:"
##################### Test Case 1 #####################
# This case tests that new processes are proplerly created
# and their nice values and parent processes properly
# assigned. Expect at the end: 
# pid: 1, parent: 0 state: RUNNING 
# pid: 2, parent: 1 state: RUNNABLE 
# pid: 3, parent: 2 state: RUNNABLE 
# pid: 4, parent: 1 state: RUNNABLE 
# pid: 5, parent: 2 state: RUNNABLE

./procprog < test_case_1_input.txt | grep "niceness:"

echo
echo "Start test 2:"
##################### Test Case 2 #####################
# This case tests that new processes are proplerly created
# and their nice values and parent processes properly
# assigned. Expect at the end: 
# pid: 1 time slice: 24ms weight: 1024
# pid: 2 time slice: 10ms weight: 36
# pid: 3 time slice: 75ms weight: 3121

./procprog < test_case_2_input.txt | grep "time slice:"