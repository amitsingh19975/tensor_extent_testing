#ifndef MDSPAN_H
#define MDSPAN_H

#include "mdspan_helper.h"
#include "extents_algorithm.h"
#include <vector>

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

        constexpr auto at(int k) const noexcept {
            return extent(k);
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

        template< typename IndexType>
        constexpr extents(std::initializer_list<IndexType> li) 
            : impl(li.begin(),li.end()){}

        template< typename ForwordIterator>
        constexpr extents(ForwordIterator* begin, ForwordIterator* end) 
            : impl( begin, end ){}

        constexpr bool is_scalar() const noexcept{
            return
                rank() != 0 &&
                all_of(*this, [](auto const& a){ return a == 1;});
        }

        constexpr auto begin() const noexcept{
            return 0u;
        }

        constexpr auto end() const noexcept{
            return rank();
        }

        constexpr bool is_vector() const noexcept{
            if(rank() == 0){
                return false;
            }

            if(rank() == 1){
                return at(0) > 1;
            }

            auto greater_one = [](auto const& a){ return a >  1;};
            auto equal_one   = [](auto const& a){ return a == 1;};

            return
                any_of(*this, begin(), begin() + 2, greater_one) &&
                any_of(*this, begin(), begin() + 2, equal_one  ) &&
                all_of(*this, begin() + 2, end(), equal_one);
        }

        constexpr bool is_matrix() const noexcept{
            if(rank() < 2){
                return false;
            }

            auto greater_one = [](auto const& a){ return a >  1;};
            auto equal_one   = [](auto const& a){ return a == 1;};

            return
                all_of ( *this, begin(), begin() + 2, greater_one) &&
                all_of ( *this, begin() + 2, end(),     equal_one  );
        }

        constexpr bool is_tensor() const noexcept{
            if(rank() < 3){
                return false;
            }

            auto greater_one = [](auto const& a){ return a > 1;};

            return any_of(*this,begin() + 2, end(), greater_one);
        }

        auto to_vector() const noexcept{
            std::vector<index_type> temp(dims);
            for(auto i = 0u; i < temp.size(); i++){
                temp[i] = at(i);
            }
            return temp;
        } 

        auto to_array() const noexcept{
            std::array<index_type,dims> temp;
            for(auto i = 0u; i < temp.size(); i++){
                temp[i] = at(i);
            }
            return temp;
        } 

        constexpr auto empty() const noexcept{
            return size() == 0;
        }

        constexpr bool valid() const noexcept{
            return
                rank() > 1 &&
                none_of(*this, begin(), end(),
                            [](auto const& a){ return a == ptrdiff_t(0); });
        }

        constexpr auto product() const noexcept{
            return size();
        }

        template< ptrdiff_t rhs_dims, ptrdiff_t ... rhs >
        constexpr auto operator==(extents<rhs_dims,rhs...> const& other) const noexcept{
            for(auto i = 0u; i < rank(); i++){
                if(other.at(i) != at(i))
                    return false;
            }
            return true;
        }

        template< ptrdiff_t rhs_dims, ptrdiff_t ... rhs >
        constexpr auto operator!=(extents<rhs_dims,rhs...> const& other) const noexcept{
            return !(*this == other);
        }

        ~extents() = default;

    private:
        using impl = detail::extents_impl< 0, detail::make_seq_t<dims, StaticExtents...> > ;
    };

}


#endif // MDSPAN_H
