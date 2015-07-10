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
  if(myRank == 0) printf("%d openMP threads\n",numThreads);
#else
#ifdef USE_OPENMP
  printf("%d openMP threads\n",numThreads);
#endif
#endif

  const long int num_steps=8000000000;
  double time = omp_get_wtime();
#ifdef USE_MPI
  long double workPerRank=num_steps/static_cast<long int>(numRanks);
  long int myStart = myRank*workPerRank+1;
  long int myEnd   = myRank*workPerRank+workPerRank;
  if(myRank==numRanks-1)
    myEnd = myEnd + (num_steps - workPerRank*numRanks);
  double pi = computePi(myStart,myEnd);
  double globalPi;
  MPI_Reduce(&pi,&globalPi,1,MPI_DOUBLE,MPI_PROD,0,MPI_COMM_WORLD);
  globalPi=globalPi/2.0;
#else
  double pi = computePi(1L,num_steps);
#endif
  time = omp_get_wtime() - time;

  cout.precision(10);
  cout << "pi is probably " 
    << fixed << pi
    << " on core " << myCPU << " of " << hostname
    << " ( " << time << " s)"
    << endl;
#ifdef USE_MPI
  if(myRank==0)
    printf("Global Pi is %14.10f\n",globalPi);
#endif

  return 0;

}


double computePi(long int start, long int stop)
{
  double pi;
  double prod=1.0;

#ifdef USE_OPENMP
#pragma omp parallel for reduction(*:prod)
#endif
  for (long int i=start;i<=stop;i++)
  {
    double temp = 4.0*i*i;
    prod*= temp/(temp - 1.0);
  }
  pi = prod*2.0;


  return pi;
}
