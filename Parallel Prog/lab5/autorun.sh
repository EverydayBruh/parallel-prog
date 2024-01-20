#!/bin/bash
for np in {1..16}
do
    mpirun -np $np --oversubscribe ./mpi
done
