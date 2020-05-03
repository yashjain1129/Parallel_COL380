#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

struct mprdc{
public:
  vector<double> keyvalue;
  void initialize(int num);
  void map(vector<vector<int>> outgoing_links,vector<double> pageranks,int num);
  void reduce(int num_webpages);
  void broadcast(int num_webpages);
};
