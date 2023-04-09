#!/bin/bash

cmake --build ../build
../build/src/main ./text.txt ../build/output.ll
./compile_ll.sh ../build/output.ll
../build/output.ll.out
