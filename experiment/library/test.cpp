#include <bits/stdc++.h>
#include <mpi.h>
#include "mylib.h"
using namespace std;

int main(int argc,  char **argv) {
  int taskid, numtasks;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

  mprdc a;
  a.trial();

  MPI_Finalize();
  return 0;
  /* code */
  return 0;
}

/*
for windows subsystem warning:
echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope

compile:
mpic++ -g -Wall -o test test.cpp mylib.cpp mylib.h

run:
mpiexec -n 4 ./test
*/
