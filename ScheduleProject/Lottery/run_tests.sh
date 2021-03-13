#!/bin/bash

make

echo "Start test 1"
##################### Test Case 1 #####################
# This case tests that new processes are proplerly created
# and their ticket values and parent processes properly
# assigned. Expect at the end: 
#pid: 1, parent: 0 state: RUNNING tickets: 1
#pid: 2, parent: 1 state: RUNNABLE tickets: 1
#pid: 3, parent: 2 state: RUNNABLE tickets: 1
#pid: 4, parent: 1 state: RUNNABLE tickets: 5
#pid: 5, parent: 2 state: RUNNABLE tickets: 78

./procprog < test_case_1_input.txt | grep "tickets:"

echo
echo "Start test 2:"
##################### Test Case 2 #####################
# This tests that the lottery scheduler assgins processes
# randomly when the wieghts are all even. It should output
# ~200 five times.

./procprog < test_case_2_input.txt | grep "selected pid: 1" | wc -l
./procprog < test_case_2_input.txt | grep "selected pid: 2" | wc -l
./procprog < test_case_2_input.txt | grep "selected pid: 3" | wc -l
./procprog < test_case_2_input.txt | grep "selected pid: 4" | wc -l
./procprog < test_case_2_input.txt | grep "selected pid: 5" | wc -l

echo
echo "Start test 3:"
##################### Test Case 3 #####################
# This tests that the lottery scheduler assgins processes
# according to their wieghts. The first four should ~100
# and the last should be ~600.

./procprog < test_case_3_input.txt | grep "selected pid: 1" | wc -l
./procprog < test_case_3_input.txt | grep "selected pid: 2" | wc -l
./procprog < test_case_3_input.txt | grep "selected pid: 3" | wc -l
./procprog < test_case_3_input.txt | grep "selected pid: 4" | wc -l
./procprog < test_case_3_input.txt | grep "selected pid: 5" | wc -l