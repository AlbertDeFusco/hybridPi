#!/bin/bash

#SBATCH --nodes=4
#SBATCH --tasks-per-node=1
#SBATCH --cpus-per-task=8
#SBATCH --time=5:00

export OMP_NUM_THREADS=8

srun /home/sam/training/mpi/hybridPi/pi
