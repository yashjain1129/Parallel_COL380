#include <bits/stdc++.h>
#include <mpi.h>
#include "mylib.h"
using namespace std;

int num_webpages = -1;
vector<vector<int>> outgoing_links(100000);
vector<double> pageranks(100000, 0.0f);
vector<double> pageranks_up(100000, 0.0f);

int main(int argc,  char **argv) {
  MPI_Init(&argc, &argv);

  int me,nprocs;
  MPI_Comm_rank(MPI_COMM_WORLD,&me);
  MPI_Comm_size(MPI_COMM_WORLD,&nprocs);

  double alpha = 0.85;
	double conv = 0.00001;
	int a,b,i;
	ifstream fopen;
	fopen.close();
	fopen.open("barabasi-20000.txt");
	while(!fopen.eof()){
		fopen>>a>>b;
		num_webpages = max(num_webpages,max(a,b));
    outgoing_links[a].push_back(b);
	}
	fopen.close();
	num_webpages++;
  // num_webpages = 20000;

  double pgr = double(1.0f/num_webpages);
	for(int i=0; i<num_webpages; i++){
		pageranks[i] = pgr;
	}

  mprdc mr;

  //while loop
  double dp =0.0f;
  for(i=0; i<num_webpages; i++){
    if(outgoing_links[i].size()==0){
      dp += pageranks[i];	// sumlist.push_back(value);
    }
  }
  dp = double(dp/num_webpages);

  MPI_Barrier(MPI_COMM_WORLD);
  mr.initialize(num_webpages);

  mr.map(outgoing_links, pageranks, num_webpages);
  // for (auto i = mr.keyvalue.begin(); i != mr.keyvalue.end(); ++i)
  //       cout << *i << " ";

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
