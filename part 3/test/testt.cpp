/* ----------------------------------------------------------------------
   MR-MPI = MapReduce-MPI library
   http://www.cs.sandia.gov/~sjplimp/mapreduce.html
   Steve Plimpton, sjplimp@sandia.gov, Sandia National Laboratories

   Copyright (2009) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the modified Berkeley Software Distribution (BSD) License.

   See the README file in the top-level MapReduce directory.
------------------------------------------------------------------------- */

// MapReduce word frequency example in C++
// Syntax: wordfreq file1 dir1 file2 dir2 ...
// (1) read all files and files in dirs
// (2) parse into words separated by whitespace
// (3) count occurrence of each word in all files
// (4) print top 10 words

#include "mpi.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/stat.h"
#include "mapreduce.h"
#include "keyvalue.h"
#include <bits/stdc++.h>

using namespace MAPREDUCE_NS;
using namespace std;

int num_edges=0;
int num_webpages=20000;
// int a,b,i;
int test;


vector<vector<double>> intermediate_pg(100000);
vector<vector<int>> outgoing_links(100000);
vector<double> pageranks(100000, 0.0f);
vector<double> pageranks_up(100000, 0.0f);
vector<double> sumlist;

void fileread(int itask, KeyValue *kv, void *ptr);
// void sum(char *, int, char *, int, int *, KeyValue *, void *);
// int ncompare(char *, int, char *, int);
// void output(uint64_t, char *, int, char *, int, KeyValue *, void *);

/* ---------------------------------------------------------------------- */

int main(int narg, char **args)
{
  MPI_Init(&narg,&args);

  int me,nprocs;
  double alpha = 0.85;
	double conv = 0.00001;
	int a,b,i;

  ifstream fopen;
	fopen.close();
	fopen.open("barabasi-20000.txt");
	while(!fopen.eof()){
		fopen>>a>>b;
		outgoing_links[a].push_back(b);
	}
	fopen.close();

  // std::cout << "I nproc = " << nprocs << std::endl;
  MPI_Comm_rank(MPI_COMM_WORLD,&me);
  MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
  // std::cout << "II nproc = " << nprocs << std::endl;

  // int world_rank;
  // MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  // cout << outgoing_links[world_rank].size() << " -> " << world_rank << endl;


  if (narg <= 1) {
    if (me == 0) printf("Syntax: wordfreq file\n");
    MPI_Abort(MPI_COMM_WORLD,1);
  }

  MapReduce *mr = new MapReduce(MPI_COMM_WORLD);
  mr->verbosity = 2;
  mr->timer = 1;
  //mr->memsize = 1;
  //mr->outofcore = 1;

  // MPI_Barrier(MPI_COMM_WORLD);
  double tstart = MPI_Wtime();
  // std::cout << "trstart = " << tstart << std::endl;
  // test = 0;

  for (int i = 0; i < 5; i++) {
    /* code */
    test = i;
    int nwords = mr->map(nprocs,fileread,NULL);
    // cout << "Rank 0 :  " << test << "\n";
    int nfiles = mr->mapfilecount;

    MPI_Barrier(MPI_COMM_WORLD);

    if (me == 0) {
      /* code */
      std::cout << "loop i = " << i << " collate" << '\n';
    }

    mr->collate(NULL);

    std::cout << "******************************************" << '\n';
    mr->print(0, 1, 5, 5);


    // if (me == 0) {
      /* code */
    // }


    // MPI_Barrier(MPI_COMM_WORLD);
    // cout << "______________________" << endl;
  }
  std::cout << "*************** loop " << i << " *****************" << '\n';
  mr->print(0, 1, 5, 5);

  std::cout << "OK" << '\n';
  if(me == 0){
    mr->gather(0);
  }


  // delete mr;

  // if (me == 0) {
  //   printf("%d total words, %d unique words\n",nwords,nunique);
  //   printf("Time to process %d files on %d procs = %g (secs)\n",
	//    nfiles,nprocs,tstop-tstart);
  // }

  MPI_Finalize();
}

/* ----------------------------------------------------------------------
   read a file
   for each word in file, emit key = word, value = NULL
------------------------------------------------------------------------- */

void fileread(int itask, KeyValue *kv, void *ptr)
{
  // filesize = # of bytes in file

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  char* numberstring = new char[(((sizeof test) * CHAR_BIT) + 2)/3 + 2];
  char* numberstring_ = new char[(((sizeof test) * CHAR_BIT) + 2)/3 + 2];
  sprintf(numberstring, "%d", test);
  sprintf(numberstring_, "%d", world_rank);
  cout << "Inside rank " << world_rank << " -> " << test << endl;
  kv->add(numberstring,2,numberstring_,2);
  std::cout << "________________________________" << '\n';

  // test = world_rank;


}


/*
Before compilation
 echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope

Compile
 make -f Makefile.test
 // mpic++ wordfreq.o ../src/libmrmpi_mpicc.a -o wordfreq

Run
mpirun -np 8 testt barabasi-20000.txt

*/
