#!/bin/bash

# Required parameter
EXE=$1

# Optional parameter
NUM_P=$2

if [[ -z $EXE ]];
then
	echo "error: missing argument: Provide the name of the executable"
	exit
fi

if [[ -z $NUM_P ]];
then
	NUM_P=2
fi

mpirun -np $NUM_P ./$EXE
