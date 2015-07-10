#include <iostream>
#include <stdio.h>

#ifdef USE_MPI
#include <mpi.h>
#endif

#include <omp.h>
using namespace std;

double arc(double x);
double computePi(long int start, long int stop);
int main()
{

#ifdef USE_MPI
  MPI_Init(NULL, NULL);

  int numRanks;
  MPI_Comm_size(MPI_COMM_WORLD, &numRanks);

  int myRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

  if(myRank == 0)
    printf("%d MPI processes\n",numRanks);
#endif

  char hostname[128];
  gethostname(hostname,128);

  int myCPU, numThreads;
#ifdef USE_OPENMP
#pragma omp parallel
#endif
  {
    myCPU = sched_getcpu();
    numThreads = omp_get_num_threads();
  }

#ifdef USE_MPI
  if(myRank == 0)
#endif
    printf("%d openMP threads\n",numThreads);

  const long int num_steps=8000000000;
  double time = omp_get_wtime();
  double pi = computePi(0L,num_steps);
  time = omp_get_wtime() - time;

  cout.precision(10);
  cout << "pi is probably " 
    << fixed << pi
    << " on core " << myCPU << " of " << hostname
    << " ( " << time << " s)"
    << endl;

  return 0;

}


double computePi(long int start, long int stop)
{
  double pi, sum=0.0;
  double step=1.0/(double) (stop - start + 1);

#ifdef USE_OPENMP
#pragma omp parallel for reduction(+:sum)
#endif
  for (long int i=start;i<=stop;i++)
  {
    double x=(i+0.5)*step;
    sum+= arc(x);
  }
  pi = sum*step;


  return pi;
}


double arc(double x)
{
  double y = 4.0/(1+x*x);
  return y;
}
