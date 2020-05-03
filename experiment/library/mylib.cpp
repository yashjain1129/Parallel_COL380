#include "mylib.h"
#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

void mprdc::trial(){
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
