#!/bin/bash
export  PATH="$PATH:../../gcc_toolchains/MinGW/bin/"

gcc simulator.c -o ../example/simulator.exe

cp ../example/simulator.exe ../../generators/C_to_all/res/simulator.exe