#include "includes/mdspan.h"
#include <vector>
using namespace mdspan;
using namespace std;

int main(int argc, char const *argv[]) {
    
    extents<3,1,2,3> e; //static dimension
    // extents<static_dims> t; //static dimension
    extents<4> f = {5,6,7,8};  //dynamic dimension
    // cout<<boolalpha<<(f==e)<<'\n';

    for(auto i = 0; i < e.rank(); i++){
        cout<<e.static_extent(i)<<' ';
    }

    cout<<'\n';

    for(auto i = 0; i < f.rank(); i++){
        cout<<f.at(i)<<' ';
    }

    cout<<'\n';
    return 0;
}
