#include <bits/stdc++.h>
// #include <iostream>
using namespace std;

struct webpage{
	int num_outgoing =0;
	float pagerank = 1;
	vector<int> outlist;
};

int num_edges=1;
int num_webpages=1;

vector<vector<float>> intermediate_pg(100000);
vector<vector<int>> outgoing_links(100000);
vector<float> pageranks(100000, 0.0f);
vector<float> pageranks_up(100000, 0.0f);

void emit(int key, float value){
	intermediate_pg[key].push_back(value);
}

void mapper(int key, float pg){
	emit(key, 0.0f);
	int sizes = outgoing_links[key].size();
	if(sizes!=0){
		for(int i =0; i<sizes; i++){
			emit(outgoing_links[key][i],(float)(pg/sizes));
		}
	}
}

void reducer(int key, vector<float> pg){
	float up_pg = 0.0f;
	for(int i =0; i<pg.size(); i++){
		up_pg += (float)pg[i];
	}
	pageranks_up[key] = up_pg;
}

int main(int argc, char const *argv[])
{

	float alpha = 0.85;
	float conv = 0.0001;

	cin >> num_webpages;
	cin >> num_edges;

	// struct webpage w1;
	float pgr = float(1.0f/num_webpages);
	for(int i=0; i<num_webpages; i++){
		pageranks[i] = pgr;
	}
	// vector<webpage> graph_webpage(num_webpages, w1);

	int a,b,i;

	for(i=0; i<num_edges; i++){
		cin>>a>>b;
		outgoing_links[a].push_back(b);
	}

	while(true){
		for(i=0; i<num_webpages; i++){
			mapper(i,pageranks[i]);
		}
		for(i=0; i<num_webpages; i++){
			reducer(i,intermediate_pg[i]);
			for(int j=0; j<intermediate_pg[i].size(); j++){
				cout<<intermediate_pg[i][j]<<" ";
			}
			intermediate_pg[i].clear();
		}	
		bool converging = true;
		for(i=0; i<num_webpages; i++){
			if(pageranks[i]-pageranks_up[i]>conv)
				converging=false;
			pageranks[i] = pageranks_up[i];
		}
		for(int i=0; i<num_webpages; i++){
			cout<<pageranks[i]<<" ";
		}

		if(converging)
			break;
	}

	cout<<"--------here is the final output------------"<<endl;

	for(int i=0; i<num_webpages; i++){
		cout<<pageranks[i]<<" ";
	}

	/* code */
	return 0;
}
