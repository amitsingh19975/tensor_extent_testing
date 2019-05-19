#ifndef MDSPAN_HELPER_H
#define MDSPAN_HELPER_H

#include "seq.h"
#include <initializer_list>
#include <array>

namespace mdspan::detail{

    template< int R, typename Seq >
    struct extents_impl;

    template< int R >
    struct extents_impl< R , seq<> >{
        using next = extents_impl;

        static constexpr int Rank           = 0;
        static constexpr int DynamicRank    = 0;
        static constexpr ptrdiff_t staticN  = 1;
        static constexpr ptrdiff_t N        = 1;

        static constexpr auto static_extent(int) noexcept {return ptrdiff_t{1};}
        constexpr auto extent(int) const noexcept {return ptrdiff_t{1};}
        constexpr auto size(int) const noexcept {return ptrdiff_t{1};}
        constexpr auto size() const noexcept {return ptrdiff_t{1};}

        template<typename IndexType, typename ...Args>
        constexpr auto in_bound(IndexType const& idx, Args ...args) const noexcept{
            return idx >= 0 && idx < N && in_bound(args...); 
        }

        constexpr extents_impl() noexcept = default;
        constexpr extents_impl(extents_impl const& other) noexcept = default;
        constexpr extents_impl& operator=(extents_impl const& other) noexcept = default;

        template< typename ForwordIterator >
        constexpr extents_impl( ForwordIterator* begin, ForwordIterator* end) noexcept{}

        constexpr auto in_bound() const noexcept{
            return true;
        }

        ~extents_impl() = default;
    };

    template< int R, ptrdiff_t ...Tail >
    struct extents_impl< R, seq<dynamic_extent, Tail...> > 
        : extents_impl < R + 1, seq<Tail...> > {
        
        using next = extents_impl < R + 1, seq<Tail...> >;

        static constexpr int Rank           = 1 + next::Rank;
        static constexpr int DynamicRank    = 1 + next::DynamicRank;
        static constexpr ptrdiff_t staticN  = dynamic_extent;

        ptrdiff_t N{-1};

        static constexpr auto static_extent( int k ) noexcept{
            return k == R ? staticN : next::static_extent(k);
        }

        constexpr auto extent( int k ) const noexcept{
            return k == R ? N : next::extent(k);
        }

        constexpr auto size( int k ) const noexcept{
            return k == R ? N * next::size() : next::size(k);
        }

        constexpr auto size() const noexcept{
            return N * next::size();
        }

        constexpr extents_impl() noexcept : next(), N(0) {}
        constexpr extents_impl(extents_impl const& other) noexcept = default;
        constexpr extents_impl& operator=(extents_impl const& other) noexcept = default;
        
        template< typename ...IndexType >
        explicit constexpr extents_impl( ptrdiff_t ND, IndexType ...DynamicExtents ) noexcept
            :N(ND),next(DynamicExtents...){}
        
        template< typename ForwordIterator >
        constexpr extents_impl( ForwordIterator* begin, ForwordIterator* end) noexcept
            :N(*begin),next(begin + 1, end){}

        template< typename IndexType , typename ... Args >
        constexpr bool in_bounds( IndexType const & idx , Args ... args ) const noexcept
            { return 0 <= idx && idx < N && next::in_bounds( args... ); }
        

        ~extents_impl() = default;
    };

    template< int R, ptrdiff_t SN, ptrdiff_t ...Tail >
    struct extents_impl< R, seq<SN, Tail...> > 
        : extents_impl < R + 1, seq<Tail...> > {

        using next = extents_impl < R + 1, seq<Tail...> >;

        static constexpr int Rank           = 1 + next::Rank;
        static constexpr int DynamicRank    = 0 + next::DynamicRank;
        static constexpr ptrdiff_t staticN  = SN;
        static constexpr ptrdiff_t N        = SN;

        static constexpr auto static_extent( int k ) noexcept{
            return k == R ? staticN : next::static_extent(k);
        }

        constexpr auto extent( int k ) const noexcept{
            return k == R ? N : next::extent(k);
        }

        constexpr auto size( int k ) const noexcept{
            return k == R ? N * next::size() : next::size(k);
        }

        constexpr auto size() const noexcept{
            return N * next::size();
        }

        constexpr extents_impl() noexcept = default;
        constexpr extents_impl(extents_impl const& other) noexcept = default;
        constexpr extents_impl& operator=(extents_impl const& other) noexcept = default;
        
        template< typename ...IndexType >
        explicit constexpr extents_impl( IndexType ...DynamicExtents ) noexcept
            : next(DynamicExtents...){}

        template< typename ForwordIterator >
        constexpr extents_impl( ForwordIterator* begin, ForwordIterator* end) noexcept
            : next( begin , end ){}

        template< typename IndexType , typename ... Args >
        constexpr bool in_bounds( IndexType const & idx , Args ... args ) const noexcept
            { return 0 <= idx && idx < N && next::in_bounds( args... ); }
        

        ~extents_impl() = default;
    };

    template< typename E >
    struct is_extent_impl : std::false_type{};

    template< int R, typename Seq  >
    struct is_extent_impl< extents_impl<R,Seq> > : std::true_type{};

}

namespace mdspan{
    
    template< ptrdiff_t dims, ptrdiff_t ... StaticExtents >
    struct extents ;
        
    template < ptrdiff_t dims_1, ptrdiff_t dims_2, ptrdiff_t el_1, ptrdiff_t... lhs, ptrdiff_t el_2, ptrdiff_t... rhs>
    auto concat_extent(extents< dims_1, el_1, lhs... > const& lhs_extent, 
        extents< dims_2, el_2, rhs... > const& rhs_extent ){

        return extents< ( dims_1 + dims_2 ), el_1, lhs..., el_2, rhs... >{};
    } 

    template < ptrdiff_t dims_1, ptrdiff_t dims_2, ptrdiff_t el, ptrdiff_t... lhs>
    auto concat_extent(extents< dims_1, el, lhs... > const& lhs_extent, 
        extents< dims_2 > const& rhs_extent ){
        
            auto const dims = dims_1 + dims_2;
            using type = extents< dims >;
            std::array<ptrdiff_t,dims> arr;
            
            size_t j = 0;
            for( auto i = 0u; i < arr.size(); i++){
                if( i < dims_1 ){
                    arr[i] = lhs_extent.extent(i);
                }else{
                    arr[i] = rhs_extent.extent(j++);
                }
            }
            return type( arr.begin(), arr.end() );
        } 

    template < ptrdiff_t dims_1, ptrdiff_t dims_2, ptrdiff_t el, ptrdiff_t... rhs>
    auto concat_extent(extents< dims_1> const& lhs_extent, 
        extents< dims_2, el, rhs... > const& rhs_extent ){
            return concat_extent(rhs_extent,lhs_extent);
    } 


    template < ptrdiff_t dims, ptrdiff_t... lhs>
    auto remove_extent_item(extents< dims, lhs... > const& lhs_extent, size_t pos){
        static_assert(dims > ptrdiff_t{0});
        
        using type = extents< dims - 1 >;
        std::array<ptrdiff_t,dims - 1> arr;

        auto j = 0u;
        for( auto i = 0u; i < lhs_extent.rank(); i++){
            if( i != pos ){
                arr[j++] = lhs_extent.extent(i);
            }
        }

        return type( arr.begin(), arr.end() );
    }

    template< typename E >
    struct is_extent : std::false_type{};

    template< ptrdiff_t D, ptrdiff_t ...E  >
    struct is_extent< extents<D,E...> > : std::true_type{};

}


#endif // MDSPAN_HELPER_H
