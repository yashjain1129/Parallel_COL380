#include <boost/config.hpp>
#if defined(BOOST_MSVC)
#   pragma warning(disable: 4127)

// turn off checked iterators to avoid performance hit
#   if !defined(__SGI_STL_PORT)  &&  !defined(_DEBUG)
#       define _SECURE_SCL 0
#       define _HAS_ITERATOR_DEBUGGING 0
#   endif
#endif
#include <bits/stdc++.h>
using namespace std;
#include "mapreduce.hpp"

vector<vector<double>> intermediate_pg(100000);
vector<vector<int>> outgoing_links(100000);
vector<double> pageranks(100000, 0.0);
vector<double> pageranks_up(100000, 0.0);
vector<double> sumlist;

int num_webpages=1000;
double dp = 0.0;

namespace dangling {


template<typename MapTask>
class get_data1 : mapreduce::detail::noncopyable
{
  public:
    get_data1() : sequence_(0)
    {
    }

    bool const setup_key(typename MapTask::key_type &key)
    {
        key = sequence_++;
        return key < num_webpages;
    }

    bool const get_data(typename MapTask::key_type const &key, typename MapTask::value_type &value)
    {
        value = pageranks[key];
        return true;
    }

  private:
    unsigned sequence_;
};

struct map_task : public mapreduce::map_task<unsigned,double>
{
    template<typename Runtime>
    void operator()(Runtime &runtime, key_type const &key, value_type const &value) const
    {
        int sizes = outgoing_links[key].size();
        if (sizes==0){
            runtime.emit_intermediate(key/key, pageranks[key]);    
        }
    }
};

struct reduce_task: public mapreduce::reduce_task<unsigned,double >
{
    template<typename Runtime, typename It>
    void operator()(Runtime &runtime, key_type const &key, It it, It ite) const
    {
        value_type results(*it);
        double k  = 0.0f;
        for (It it1=it; it1!=ite; ++it1)
        {
            double r = *it1;
            k += r;
        }
        results = k;
        
        runtime.emit(key, results);
    } 
};

typedef
mapreduce::job<dangling::map_task,
               dangling::reduce_task,
               mapreduce::null_combiner,
               dangling::get_data1<dangling::map_task>
> job;

}

namespace pagerank {


template<typename MapTask>
class datanodes1 : mapreduce::detail::noncopyable
{
  public:
    datanodes1() : sequence_(0)
    {
    }

    bool const setup_key(typename MapTask::key_type &key)
    {
        key = sequence_++;
        return key < num_webpages;
    }

    bool const get_data(typename MapTask::key_type const &key, typename MapTask::value_type &value)
    {
        value = pageranks[key];
        return true;
    }

  private:
    unsigned sequence_;
};

struct map_task : public mapreduce::map_task<unsigned,double>
{
    template<typename Runtime>
    void operator()(Runtime &runtime, key_type const &key, value_type const &value) const
    {
        int sizes = outgoing_links[key].size();
        if (sizes==0){
            dp+=value;
        }
        runtime.emit_intermediate(key, 0);    
        for (int i =0; i<sizes; i++)
        {
            typename Runtime::reduce_task_type::key_type const emit_key = outgoing_links[key][i];
            double temp = (double)value/sizes;
            runtime.emit_intermediate(emit_key, temp);
        }
    }
};

struct reduce_task : public mapreduce::reduce_task<unsigned, double >
{
    template<typename Runtime, typename It>
    void operator()(Runtime &runtime, key_type const &key, It it, It ite) const
    {
        value_type results(*it);
        double k  = 0.0;
        for (It it1=it; it1!=ite; ++it1)
        {
            double r = *it1;
            k += r;
        }
        results = (double)k;
        runtime.emit(key, results);
    } 
};

typedef
mapreduce::job<pagerank::map_task,
               pagerank::reduce_task,
               mapreduce::null_combiner,
               pagerank::datanodes1<pagerank::map_task>
> job;

}

int main(int argc, char *argv[])
{   
    double alpha = 0.85;
    double conv = 0.000001;
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


    double pgr = double(1.0/num_webpages);
    for(int i=0; i<num_webpages; i++){
        pageranks[i] = pgr;
    }
    mapreduce::specification spec;

    spec.reduce_tasks = std::max(1U, std::thread::hardware_concurrency());

    int count = 0;
    while(true){
        count++;
        dangling::job::datasource_type get_data1;
        dangling::job job1(get_data1, spec);
        mapreduce::results result1; 
    #ifdef _DEBUG
        job1.run<mapreduce::schedule_policy::sequential<dangling::job> >(result1);
    #else
        job1.run<mapreduce::schedule_policy::cpu_parallel<dangling::job> >(result1);
    #endif
        auto it=job1.begin_results();
        dp = it->second/num_webpages;
        pagerank::job::datasource_type datanodes1;
        pagerank::job job(datanodes1, spec);
        mapreduce::results result; 
        bool converging = true;
    #ifdef _DEBUG
        job.run<mapreduce::schedule_policy::sequential<pagerank::job> >(result);
    #else
        job.run<mapreduce::schedule_policy::cpu_parallel<pagerank::job> >(result);   
    #endif
        for (auto it=job.begin_results(); it!=job.end_results(); ++it){
            double conv_temp = pageranks[it->first];
            i = it->first;
            pageranks[it->first] = (double)it->second*alpha + dp*alpha/num_webpages + (double)(1-alpha)/num_webpages;
            if(abs(conv_temp-pageranks[it->first])>conv)
                converging = false;
        }
        if(converging){
            cout<<"number of iterations are "<<count<<endl;
            break;
        }
    }

    double sumer = 0.0;
    for(int i=0; i<num_webpages; i++){
        cout<<i<<" = "<<pageranks[i]<<endl;
        sumer += pageranks[i];
    }
    cout<<"sum "<<sumer;    
    return 0;
}

