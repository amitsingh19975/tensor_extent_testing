#include "includes/mdspan.h"
#include "includes/tensor.h"
#include <vector>
using namespace mdspan;
using namespace std;

int main(int argc, char const *argv[]) {
    
    extents<3,1,2,3> e; //static dimension
    // extents<static_dims> t; //static dimension
    extents<4> f = {5,6,7,8};  //dynamic dimension
    extents<dynamic_dims> g = {1,2,3};  //dynamic dimension
    // cout<<boolalpha<<(f==e)<<'\n';


    for(auto i = 0; i < e.rank(); i++){
        cout<<e.at(i)<<' ';
    }

    cout<<'\n';

    for(auto i = 0; i < f.rank(); i++){
        cout<<f.at(i)<<' ';
    }

    // cout<<'\n';
    test::tensor<int,test::dims<dynamic_dims>,int,storage_type::dense<int,int>> t;
    test::tensor<int,test::dims<dynamic_dims>> u;
    return 0;
}
