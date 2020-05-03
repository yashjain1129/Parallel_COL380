#include "mylib.h"
#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

void mprdc::initialize(int num){
  this->keyvalue.clear();
  this->keyvalue.resize(num, 0.0);
}

void mprdc::map(vector<vector<int>> outgoing_links,vector<double> pageranks,int num_webpages){
  int world_rank, num_procs;
  MPI_Comm_size(MPI_COMM_WORLD,&num_procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  for (int i = ((world_rank*num_webpages)/num_procs); i < (((world_rank+1)*num_webpages)/num_procs); i++) {
    // cout << i << endl;
    if(outgoing_links[i].size() == 0){
      continue;
    }
    for(int j =0; j<outgoing_links[i].size(); j++){
      // cout << j << " of " << outgoing_links[i].size() << " of webpage " << i << " of " << (world_rank+1)*(20000/num_procs) << " (" << world_rank << ") " << endl;
      double pg = (double)(pageranks[i]/outgoing_links[i].size());
      // std::cout << "OK " << pg << '\n';
      this->keyvalue[outgoing_links[i][j]] += pg;
		}
  }
}

// void mprdc::trial(){
//   int taskid, numtasks;
//   MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
//   MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
//
//   if(taskid == 0){
//     int number = -1;
//     MPI_Send(&number,1, MPI_INT, 1, 0, MPI_COMM_WORLD);
//     cout << "sent " << number << endl;
//   }
//   if(taskid == 1){
//     int numberr;
//     MPI_Recv(&numberr,1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//     cout << "recieved " << numberr << endl;
//   }
// }
