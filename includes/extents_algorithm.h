#ifndef EXTENTS_ALGORITHM_H
#define EXTENTS_ALGORITHM_H
#include <iostream>

namespace mdspan{
    
    template< ptrdiff_t dims, ptrdiff_t ... StaticExtents >
    struct extents ;

    template< ptrdiff_t dims, ptrdiff_t ... StaticExtents, typename UinaryFunc >
    constexpr auto all_of(extents<dims, StaticExtents...> const& e, size_t begin, size_t end, UinaryFunc func){
        auto all = 0u;
        for(auto i = begin; i < end; i++){
            if( func(e.at(i)) )
                all++;
        }
        return all == ( end - begin );
    }

    template< ptrdiff_t dims, ptrdiff_t ... StaticExtents, typename UinaryFunc >
    constexpr auto all_of(extents<dims, StaticExtents...> const& e, UinaryFunc func){
        return all_of(e,0,e.rank(),func);
    }

    template< ptrdiff_t dims, ptrdiff_t ... StaticExtents, typename UinaryFunc >
    constexpr auto none_of(extents<dims, StaticExtents...> const& e, UinaryFunc func){
        return !all_of(e,func);
    }

    template< ptrdiff_t dims, ptrdiff_t ... StaticExtents, typename UinaryFunc >
    constexpr auto none_of(extents<dims, StaticExtents...> const& e, size_t begin, size_t end, UinaryFunc func){
        return !all_of(e,begin,end,func);
    }

    template< ptrdiff_t dims, ptrdiff_t ... StaticExtents, typename UinaryFunc >
    constexpr bool any_of(extents<dims, StaticExtents...> const& e, size_t begin, size_t end, UinaryFunc func){
        for(auto i = begin; i < end; i++){
            if( func(e.at(i)) )
                return true;
        }
        return false;
    }

    template< ptrdiff_t dims, ptrdiff_t ... StaticExtents, typename UinaryFunc >
    constexpr bool any_of(extents<dims, StaticExtents...> const& e, UinaryFunc func){
        return any_of(e,0,e.rank(),func);
    }

    template< typename T, ptrdiff_t dims, ptrdiff_t ... StaticExtents, typename BinaryFunc >
    constexpr auto accumulate(extents<dims, StaticExtents...> const& e, size_t begin, size_t end, T initial, BinaryFunc func){
        for(auto i = begin; i < end; i++){
            initial = func(initial, e.at(i));
        }
        return initial;
    }
    
    template< typename T, ptrdiff_t dims, ptrdiff_t ... StaticExtents, typename BinaryFunc >
    constexpr auto accumulate(extents<dims, StaticExtents...> const& e, T initial, BinaryFunc func){
        return accumulate(e,0,e.rank(),initial,func);
    }

    template< typename T, ptrdiff_t dims, ptrdiff_t ... StaticExtents>
    constexpr auto accumulate(extents<dims, StaticExtents...> const& e, size_t begin, size_t end, T initial){
        for(auto i = begin; i < end; i++){
            initial += e.at(i);
        }
        return initial;
    }

    template< typename T, ptrdiff_t dims, ptrdiff_t ... StaticExtents>
    constexpr auto accumulate(extents<dims, StaticExtents...> const& e, T initial){
        return accumulate(e,0,e.rank(),initial);
    }

    template< typename T, ptrdiff_t dims, ptrdiff_t ... StaticExtents>
    constexpr auto find(extents<dims, StaticExtents...> const& e, T value){
        for(auto i = 0u; i < e.rank() + 1; i++){
            if(e.at(i) == value) return true;
        }
        return false;
    }

    template< typename T, ptrdiff_t dims, ptrdiff_t ... StaticExtents, typename UinaryFunc>
    constexpr auto find_if(extents<dims, StaticExtents...> const& e, UinaryFunc func){
        for(auto i = 0u; i < e.rank(); i++){
            if( func( e.at(i) ) ) return true;
        }
        return false;
    }
}

#endif // EXTENTS_ALGORITHM_H
