#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;
int num_webpages=20000;

vector<vector<int>> outgoing_links(100000);
// vector<double> sumlist;

void mapper(MPI_Comm mpi_comm, int numproc, int taskid, vector<double> pageranks){
  int segsize = num_webpages/(numproc);
  vector<double> pageranks_im(num_webpages, 0.0f);
  for(int j=0; j<pageranks.size(); j++){
  	int sizes = outgoing_links[(taskid*segsize) + j].size();
  	if(sizes!=0){
  		for(int i =0; i<sizes; i++){
  			pageranks_im[outgoing_links[(taskid*segsize) + j][i]] += (double)(pageranks[j]/sizes);
  		}
  	}
  }
  for (int d = 0; d < numproc; d++) {
    if(d!=taskid){
      cout<<"near"<<taskid<<endl;
      MPI_Isend(&pageranks_im[d*segsize],segsize, MPI_DOUBLE, d, 1, mpi_comm);
      cout<<"near"<<taskid<<endl;
    }
  }
  cout<<"neareeee"<<taskid<<endl;
  return;
}

void reducer(int numprocs, MPI_Comm mpi_comm, vector<double> &pagerank){
  int segsize = num_webpages/(numprocs);
  vector<double> pg(segsize,0.0f);
  vector<double> pg_acc(segsize,0.0f);
  for (int d = 0; d < numprocs; d++) {
    MPI_Recv(&pg[0],segsize, MPI_DOUBLE, d, 1, mpi_comm, MPI_STATUS_IGNORE);
    for(int i =0; i<segsize; i++){
      pg_acc[i] += pg[i];
    }
  }
  pagerank =  pg_acc;
  return;
}

int main(int argc, char **argv) {
  int taskid, numprocs;
  // vector<double> pageranks(100000, 0.0f);
  double alpha = 0.85;
  double conv = 0.00001;
  int a,b,i;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

  ifstream fopen;
  fopen.open("barabasi-20000.txt");
  while(!fopen.eof()){
    fopen>>a>>b;
    num_webpages = max(num_webpages,max(a,b));
  }
  fopen.close();
  num_webpages++;

  fopen.open("barabasi-20000.txt");
  while(!fopen.eof()){
    fopen>>a>>b;
    outgoing_links[a].push_back(b);
  }
  fopen.close();
  int seg_size = num_webpages/(numprocs);
  vector<double> pageranks(seg_size, 0.0f);
  vector<double> pageranks_up(seg_size, 0.0f);

  while(true){
    mapper(MPI_COMM_WORLD, numprocs, taskid, pageranks);
    cout<<"here"<<taskid<<endl;
    MPI_Barrier(MPI_COMM_WORLD);
    reducer(numprocs, MPI_COMM_WORLD, pageranks_up);
    bool converging = true;
    for(i=0; i<num_webpages; i++){
      if(pageranks[i]-pageranks_up[i]>conv)
      converging=false;
      pageranks[i] = pageranks_up[i];
    }
    for(int i=0; i<num_webpages; i++){
      cout<<i<<" = "<<pageranks[i]<<endl;
    }
    break;
    if(converging)
    break;
  }

  MPI_Finalize();
  return 0;
}

/*

compile:
mpicc -g -Wall -o asgn2 asgn2.c

run:
mpiexec -n 4 ./asgn2

*/
