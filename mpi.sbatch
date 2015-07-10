#!/bin/bash

#SBATCH -J mpi+openmp
#SBATCH -o mpi.out
#SBATCH --nodes=4
#SBATCH --ntasks-per-node=2
#SBATCH -c 1
#SBATCH --qos=short
#
#


##serial
echo "%%%%%%%%%%%%%%%% serial %%%%%%%%%%%%%%%%"
#export OMP_NUM_THREADS=1
#./pi
echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"

echo
echo
echo

##single MPI rank per node
echo "%%%%%%%%%%%%%%%% single %%%%%%%%%%%%%%%%"
#export OMP_NUM_THREADS=1
#srun --tasks-per-node=1 ./pi
echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"

echo
echo
echo

##
echo "%%%%%%%%%%%%%%%% 4 threads per node? %%%%%%%%%%%%%%%%"
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
#srun --hint=compute_bound ./pi
srun ./pi
echo "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%"

