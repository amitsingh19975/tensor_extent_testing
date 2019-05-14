#ifndef MDSPAN_HELPER_H
#define MDSPAN_HELPER_H

#include "seq.h"
#include <initializer_list>

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

        template<typename T>
        constexpr extents_impl(T const* const) noexcept{}

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

        template< typename T >
        explicit constexpr extents_impl( T const * const DN ) noexcept
            : next( DN + 1 ), N(*DN) {}

        template< typename IndexType , typename ... Args >
        constexpr bool in_bounds( IndexType const & idx , Args ... args ) const noexcept
            { return 0 <= idx && idx < N && next::in_bounds( args... ); }
        

        ~extents_impl() = default;
    };

    template< int R, ptrdiff_t SN, ptrdiff_t ...Tail >
    struct extents_impl< R, seq<SN, Tail...> > : extents_impl < R + 1, seq<Tail...> > {
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
        
        template< typename T >
        explicit constexpr extents_impl( T const * const DN ) noexcept
            : next( DN + 1 ) {}

        template< typename IndexType , typename ... Args >
        constexpr bool in_bounds( IndexType const & idx , Args ... args ) const noexcept
            { return 0 <= idx && idx < N && next::in_bounds( args... ); }
        

        ~extents_impl() = default;
    };

}


#endif // MDSPAN_HELPER_H
