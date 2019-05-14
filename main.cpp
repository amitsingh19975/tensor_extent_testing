#include "includes/mdspan.h"
#include <vector>
using namespace mdspan;
using namespace std;

int main(int argc, char const *argv[]) {
    
    extents<static_dims,1,2,3> e; //static dimension
    // extents<static_dims> t; //static dimension
    extents<4> f = {1,2,3,4};  //dynamic dimension

    for(auto i = 0; i < e.rank(); i++){
        cout<<e.static_extent(i)<<' ';
    }

    cout<<'\n';

    for(auto i = 0; i < f.rank(); i++){
        cout<<f.static_extent(i)<<' ';
    }

    cout<<'\n';

    for(auto i = 0; i < f.rank(); i++){
        cout<<f.extent(i)<<' ';
    }
    cout<<'\n';
    return 0;
}
