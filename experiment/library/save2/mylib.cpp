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
      double pg = (double)(pageranks[i]/outgoing_links[i].size());
      // std::cout << "OK " << pg << '\n';
      this->keyvalue[outgoing_links[i][j]] += pg;
		}
  }
}

void mprdc::reduce(int num_webpages){
  int world_rank, num_procs;
  MPI_Comm_size(MPI_COMM_WORLD,&num_procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  if(world_rank == 0){
    vector<double> recieve;
    recieve.resize(num_webpages);
    for (int i = 1; i < num_procs; i++) {
      MPI_Recv(&recieve[0], num_webpages, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      std::transform(this->keyvalue.begin(),this->keyvalue.end(),recieve.begin(),this->keyvalue.begin(),std::plus<double>( ));
    }
  }else{
     MPI_Send( &this->keyvalue[0], num_webpages, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
     this->keyvalue.clear();
  }
}

void mprdc::broadcast(int num_webpages){
  int world_rank, num_procs;
  MPI_Comm_size(MPI_COMM_WORLD,&num_procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  if(world_rank != 0){
    this->keyvalue.resize(num_webpages);
    MPI_Recv(&this->keyvalue[0], num_webpages, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }else{
    for (int i = 1; i < num_procs; i++) {
      MPI_Send( &this->keyvalue[0], num_webpages, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
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
