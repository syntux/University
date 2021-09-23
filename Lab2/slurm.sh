#!/bin/bash

#SBATCH --job-name=vector
#SBATCH --ntasks=4
#SBATCH --mem=2gb
#SBATCH --time=00:05:00
#SBATCH --output=out/%n.log

module load mpi/mpich-3.2-x86_64

mpiexec valgrind /mnt/linuxlab/home/bsmith41/Documents/cosc420/Lab2/vector
