#include <bits/stdc++.h>
// #include <iostream>
using namespace std;

struct webpage{
	int num_outgoing =0;
	float pagerank = 1;
	vector<int> outlist;
};

int main(int argc, char const *argv[])
{

	int num_webpages=1;
	int num_edges=1;
	cin >> num_webpages;
	cin >> num_edges;

	struct webpage w1;
	w1.pagerank = float(1.0f/num_webpages);
	vector<webpage> graph_webpage(num_webpages, w1);

	int a,b,i;

	for(i=0; i<num_edges; i++){
		cin>>a>>b;
		graph_webpage[a].outlist.push_back(b);
		graph_webpage[a].num_outgoing++;
	}

	/* code */
	return 0;
}

