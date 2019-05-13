#include "includes/mdspan.h"
#include <vector>
using namespace mdspan;
using namespace std;

int main(int argc, char const *argv[]) {
    
    extents<static_dims,1,2,3> e;
    extents<4> f(1,2,3,4);

    for(auto i = 0; i < e.rank(); i++){
        cout<<e.static_extent(i)<<' ';
    }
    cout<<'\n';
    for(auto i = 0; i < e.rank(); i++){
        cout<<f.static_extent(i)<<' ';
    }
    cout<<'\n';
    for(auto i = 0; i < e.rank(); i++){
        cout<<f.extent(i)<<' ';
    }
    cout<<'\n';
    return 0;
}
