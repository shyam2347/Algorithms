#!/bin/bash

echo "TEST SUITE"
gcc sudoku.c
if [ $? -eq 0 ]
then
    ./a.out test1.txt
    ./test out.txt
    echo ""
fi
