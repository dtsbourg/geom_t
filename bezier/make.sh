#!/bin/bash

echo "Compiling bezier.c"
gcc -o bezier bezier.c
echo "Running ./bezier"
chmod +x bezier
./bezier