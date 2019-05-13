#ifndef MDSPAN_H
#define MDSPAN_H

#include "mdspan_helper.h"

namespace mdspan{

    template< ptrdiff_t dims, ptrdiff_t ... StaticExtents >
    struct extents ;

    template< ptrdiff_t dims, ptrdiff_t ... StaticExtents >
    struct extents : private detail::extents_impl< 0, detail::make_seq_t<dims, StaticExtents...> >{

        using index_type = ptrdiff_t;

        static constexpr auto rank() noexcept {
            return impl::Rank;
        }
        static constexpr auto rank_dyanmic() noexcept {
            return impl::DynamicRank;
        }
        static constexpr auto static_extent(int k) noexcept {
            return impl::static_extent(k);
        }

        constexpr auto extent(int k) const noexcept {
            return impl::extent(k);
        }

        constexpr auto size(int k) const noexcept {
            return impl::size(k);
        }

        constexpr auto size() const noexcept {
            return impl::size();
        }

        constexpr extents() = default;
        constexpr extents(extents const& other) = default;
        constexpr extents& operator=(extents const& other) = default;

        template< typename ...IndexType>
        constexpr extents(IndexType ...DynamicExtents) 
            : impl(DynamicExtents...){}

        ~extents() = default;

    private:
        using impl = detail::extents_impl< 0, detail::make_seq_t<dims, StaticExtents...> >;
    };


}


#endif // MDSPAN_H
