#!/bin/bash
export  PATH="$PATH:../../gcc_toolchains/MinGW/bin/"

gcc DataMemGenerator.c -o ../C_to_all/res/DataMemGenerator.exe
#$SHELL