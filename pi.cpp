#include <iostream>
#include <stdio.h>
#include <unistd.h>

#ifdef USE_MPI
#include <mpi.h>
#endif

#include <omp.h>
using namespace std;

double arc(double x);
double computePi(long int start, long int stop,long int num_steps);
int main()
{
  double globalTime = omp_get_wtime();

#ifdef USE_MPI
  MPI_Init(NULL, NULL);

  int numRanks;
  MPI_Comm_size(MPI_COMM_WORLD, &numRanks);

  int myRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

  if(myRank == 0)
    printf("%d MPI processes\n",numRanks);
  MPI_Barrier(MPI_COMM_WORLD);
  usleep(20);
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

#ifdef USE_OPENMP
#ifdef USE_MPI
  if(myRank == 0) printf("%d openMP threads\n",numThreads);
#else
  printf("%d openMP threads\n",numThreads);
#endif
#endif

#ifdef USE_MPI
  MPI_Barrier(MPI_COMM_WORLD);
  usleep(20);
#endif

  const long int num_steps=8000000000;
  double time = omp_get_wtime();
#ifdef USE_MPI
  long double workPerRank=num_steps/static_cast<long int>(numRanks);
  long int myStart = myRank*workPerRank+1;
  long int myEnd   = myRank*workPerRank+workPerRank;
  if(myRank==numRanks-1)
    myEnd = myEnd + (num_steps - workPerRank*numRanks);
  double pi = computePi(myStart,myEnd,num_steps);
  double globalPi;
  MPI_Reduce(&pi,&globalPi,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
#else
  double pi = computePi(1L,num_steps,num_steps);
#endif
  time = omp_get_wtime() - time;

  cout.precision(10);
  cout << "local value is " 
    << fixed << pi
    << " on core " << myCPU << " of " << hostname
    << " ( " << time << " s)"
    << endl;
#ifdef USE_MPI
  MPI_Barrier(MPI_COMM_WORLD);
  usleep(20);
  if(myRank==0)
    printf("\nGlobal Pi is %14.10f\n",globalPi);
#endif

  globalTime = omp_get_wtime() - globalTime;
#ifdef USE_MPI
  if(myRank==0)
    printf("%f seconds\n",globalTime);
#else
  printf("\n%f seconds\n",globalTime);
#endif


  return 0;

}


double computePi(long int start, long int stop,long int num_steps)
{
  double pi;
  double sum=0.0;
  double step = 1.0/static_cast<double>(num_steps);
  double x;

#ifdef USE_OPENMP
#pragma omp parallel for reduction(+:sum)
#endif
  for (long int i=start;i<=stop;i++)
  {
    x = (i+0.5)*step;
    sum += 4.0/(1.0+x*x);
  }

  pi = step*sum;

  return pi;
}
