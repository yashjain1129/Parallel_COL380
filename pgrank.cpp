#include <bits/stdc++.h>
using namespace std;

int num_webpages;

vector<vector<double>> intermediate_pg(100000);
vector<vector<int>> outgoing_links(100000);
vector<double> pageranks(100000, 0.0f);
vector<double> pageranks_up(100000, 0.0f);
vector<double> sumlist;

void emit1(int key, double value){
	sumlist.push_back(value);
}

void mapper1(int key, double pg){
	int sizes = outgoing_links[key].size();
	if(sizes==0){
		emit1(1,pg);
	}
}

void emit(int key, double value){
	intermediate_pg[key].push_back(value);
}

void mapper(int key, double pg){
	emit(key, 0.0f);
	int sizes = outgoing_links[key].size();
	if(sizes!=0){
		for(int i =0; i<sizes; i++){
			emit(outgoing_links[key][i],(double)(pg/sizes));
		}
	}
}

void reducer(int key, vector<double> pg){
	double up_pg = 0.0f;
	for(int i =0; i<pg.size(); i++){
		up_pg += (double)pg[i];
	}
	pageranks_up[key] = up_pg;
}

int main(int argc, char const *argv[])
{

	double alpha = 0.85;
	double conv = 0.00001;
	int a,b,i;
	ifstream fopen;
	fopen.close();
	fopen.open(argv[1]);
	while(!fopen.eof()){
		fopen>>a>>b;
		num_webpages = max(num_webpages,max(a,b));
	}
	fopen.close();
	num_webpages++;

	fopen.open(argv[1]);
	while(!fopen.eof()){
		fopen>>a>>b;
		outgoing_links[a].push_back(b);
	}
	fopen.close();

	double pgr = double(1.0f/num_webpages);
	cout<<pgr;
	for(int i=0; i<num_webpages; i++){
		pageranks[i] = pgr;
	}

	while(true){
		for(i=0; i<num_webpages; i++){
			mapper1(i,pageranks[i]);
		}
		double dp =0.0f;
		for(i=0; i<sumlist.size(); i++){
			dp += sumlist[i];
		}
		dp = double(dp/num_webpages);
		sumlist.clear();
		for(i=0; i<num_webpages; i++){
			mapper(i,pageranks[i]);
		}
		for(i=0; i<num_webpages; i++){
			reducer(i,intermediate_pg[i]);
			intermediate_pg[i].clear();
		}	
		for(i=0; i<num_webpages; i++){
			pageranks_up[i] = (double)alpha*pageranks_up[i] + alpha*dp + (double)(1-alpha)/num_webpages;
		}
		bool converging = true;
		for(i=0; i<num_webpages; i++){
			if(pageranks[i]-pageranks_up[i]>conv)
				converging=false;
			pageranks[i] = pageranks_up[i];
		}
		if(converging)
			break;
	}

	double sumer = 0.0;
	for(int i=0; i<num_webpages; i++){
		cout<<i<<" = "<<pageranks[i]<<endl;
		sumer += pageranks[i];
	}
	cout<<"sum "<<sumer;

	return 0;
}
