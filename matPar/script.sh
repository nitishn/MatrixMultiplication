#!/bin/bash

#$ -q BrainLab.owner
#$ -cwd
#$ -N pa3
#$ -pe orte 20
#$ -M nitishn@gmail.com
#$ -m s
#$ -o pa3.o
#$ -e pa3.e

export PATH=/usr/lib64/openmpi-1.5.3/bin:$PATH
export LD_LIBRARY_PATH=/usr/lib64/openmpi-1.5.3/lib:$LD_LIBRARY_PATH

for j in $(seq 5 1 20)
   do
	for i in $(seq 251 250 2001)
  	 do
		   mpirun -np $j ./pa3 $i >> dump/pa$j
	done
done
				
