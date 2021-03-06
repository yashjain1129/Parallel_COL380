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
  fopen.open(argv[1]);
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
  int tt = 0;

  mprdc mr;

  while (true) {
    double dp =0.0f;
    for(i=0; i<num_webpages; i++){
      if(outgoing_links[i].size()==0){
        dp += pageranks[i];  // sumlist.push_back(value);
      }
    }
    dp = double(dp/num_webpages);

    MPI_Barrier(MPI_COMM_WORLD);
    mr.initialize(num_webpages);

    mr.map(outgoing_links, pageranks, num_webpages);

    mr.reduce(num_webpages);

    mr.broadcast(num_webpages);

    int ttt = 0;
    for (auto i = mr.keyvalue.begin(); i != mr.keyvalue.end(); ++i){
      pageranks_up[ttt] = *i;
      // cout << ttt << " = " << pageranks_up[ttt] << endl;
      ttt++;
    }
    for(i=0; i<num_webpages; i++){
      pageranks_up[i] = (double)alpha*pageranks_up[i] + alpha*dp + (double)(1-alpha)/num_webpages;
    }
    bool converging = true;
    for(i=0; i<num_webpages; i++){
      if(pageranks[i]-pageranks_up[i]>conv){
        converging=false;
        pageranks[i] = pageranks_up[i];
      }
    }

    /*
    double sumer = 0.0;
    for(int i=0; i<num_webpages; i++){
      // cout<<i<<" = "<<pageranks[i]<<endl;
      sumer += pageranks[i];
    }
    cout<<"sum "<<sumer << endl;
    */

    // if(me == 0){
    //   std::cout << "pageranks[0] = " << dp << '\n';
    // }
    // cout << "Loop -> " << tt << " is " << converging << endl;
    if(converging)
      break;

    tt++;


  }

/*
  if (me == 0) {
    std::cout << "Number of loop -> " << tt << '\n';
  }

  double sumer = 0.0;
  for(int i=0; i<num_webpages; i++){
    if(me == 0){
      // cout << i << " = " << pageranks[i] << endl;
    }
    sumer += pageranks[i];
  }
  */

  if(me == 0){
    ofstream fopen1;
    fopen1.open(argv[3]);
    double sumer = 0.0;
    for(int i=0; i<num_webpages; i++){
        fopen1<<i<<" = "<<pageranks[i]<<endl;
        sumer += pageranks[i];
    }
    // cout<<"number of iterations are "<<count<<endl;
    fopen1<<"sum "<<sumer;
    fopen1.close();
  }

  MPI_Finalize();
  return 0;
  /* code */
  return 0;
}

/*
for windows subsystem warning:
echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope

compile:
make -f Makefile.part2
mpic++ -g -Wall -o mr-pr-mpi.o test.cpp mylib.cpp mylib.h

run:
mpiexec -n 4 ./mr-pr-mpi.o barabasi-20000.txt -o barabasi-20000-pr-mpi.txt
*/
