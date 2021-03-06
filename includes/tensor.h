#ifndef TENSOR_H
#define TENSOR_H
#include "mdspan.h"
#include "storage_policy.h"

namespace test{
    using namespace mdspan;
    
    template<ptrdiff_t D, ptrdiff_t ...E >
    using dims = extents_t<D,E...>;
    
    template< typename T,typename E, typename F, typename A >
    struct tensor;

    template<typename T,typename E = dims<dynamic_dims>,typename F = int, typename A = std::vector<T> >
    struct tensor{
        static_assert(is_extent<E>::value,"NOT A EXTENT TYPE");
        T at(int) const{
            return _base.at(0);
        }
        T& at(int){
            return _base.at(0);
        }
        void set(T val, size_t k){ _base.insert(val,k);}
        
        tensor() = default;
    private:
        E _extent;
        A _base;
    };
}


#endif // TENSOR_H
