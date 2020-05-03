#include <bits/stdc++.h>
#include <mpi.h>
// #include "mylib.h"
using namespace std;

// void trial(MPI_Comm mpi_comm){
void trial(){
  int taskid, numtasks;
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

  if(taskid == 0){
    int number = -1;
    MPI_Send(&number,1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    cout << "sent " << number << endl;
  }
  if(taskid == 1){
    int numberr;
    MPI_Recv(&numberr,1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    cout << "recieved " << numberr << endl;

  }


}

int main(int argc,  char **argv) {
  int taskid, numtasks;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
  MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

  // if(taskid == 0){
  //   printf("hello\n");
  // }
  // else{
  //   printf("hello %d\n", taskid);
  // }

  trial();

  MPI_Finalize();
  return 0;
  /* code */
  return 0;
}

/*
for windows subsystem warning:
echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope

compile:
mpic++ -g -Wall -o test test.cpp

run:
mpiexec -n 4 ./test
*/
