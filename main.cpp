#include "includes/mdspan.h"
#include "includes/tensor.h"
#include <vector>
using namespace mdspan;
using namespace std;

int main(int argc, char const *argv[]) {
    
    extents<3,1,2,3> e; //static dimension
    extents<3> f = {1,2,1};  //dynamic extents
    extents<dynamic_dims> g = {1,2,3};  //dynamic dimension

    cout<<boolalpha<<(f.is_tensor())<<'\n';


    for(auto const& i : e){
        cout<<i<<' ';
    }

    cout<<'\n';
    for(auto i = f.begin(); i != f.end(); i++){
        cout<<*i<<' ';
    }

    cout<<'\n';

    for(auto i = 0; i < e.rank(); i++){
        cout<<e.at(i)<<' ';
    }

    cout<<'\n';

    cout<<std::accumulate(e.begin(),e.end(),0)<<'\n';

    // cout<<'\n';
    test::tensor<int,test::dims<dynamic_dims>,int,storage_type::dense_tensor::dense<int,std::vector<int>> > t;
    test::tensor<int,test::dims<dynamic_dims>,int,storage_type::sparse_tensor::map_compression<int>> u;
    test::tensor<int,test::dims<10>> v;
    test::tensor<int,test::dims<3,1,2,3>> w;
    return 0;
}
