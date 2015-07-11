SHELL=/bin/bash -l

all: pi pi-mpi pi-omp pi-serial

CC = icc

mpiModule = spack load openmpi%intel@15.0.3
MPICC     = spack env openmpi%intel@15.0.3 mpicc

source = pi.cpp


##-openmp is requred even in serial becuase I use omp_get_wtime()
pi: $(source)
	$(mpiModule) && $(MPICC) $< -DUSE_OPENMP -DUSE_MPI -openmp -o $@

pi-omp: $(source)
	$(CC) $< -DUSE_OPENMP -openmp -o $@

pi-mpi: $(source)
	$(mpiModule) && $(MPICC) $< -DUSE_MPI -openmp -o $@

pi-serial: $(source)
	$(CC) $< -openmp -o $@

clean:
	rm -f pi pi-omp pi-mpi pi-serial
