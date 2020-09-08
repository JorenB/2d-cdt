#!/bin/bash

for i in $(seq $1 $2)
do
	sbatch ./run.sh $i
done



