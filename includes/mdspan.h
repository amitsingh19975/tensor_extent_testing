#ifndef MDSPAN_H
#define MDSPAN_H

#include "mdspan_helper.h"
#include "extents_algorithm.h"
#include <vector>
#include <numeric>

namespace mdspan{
    ptrdiff_t const dynamic_dims = -2;

    template< ptrdiff_t dims, ptrdiff_t ... StaticExtents >
    struct extents ;

    template< >
    struct extents<dynamic_dims>{

        using base_type = std::vector<ptrdiff_t>;
        using value_type = typename base_type::value_type;
        using const_reference = typename base_type::const_reference;
        using reference = typename base_type::reference;
        using size_type = typename base_type::size_type;
        using const_pointer = typename base_type::const_pointer;
        using const_iterator = typename base_type::const_iterator;

        using index_type = ptrdiff_t;

            
        /** @brief Default constructs extents
         *
         * @code auto ex = extents<unsigned>{};
         */
        explicit extents()
        : _base{}
        {
        }

        /** @brief Copy constructs extents from a one-dimensional container
         *
         * @code auto ex = extents<unsigned>(  std::vector<unsigned>(3u,3u) );
         *
         * @note checks if size > 1 and all elements > 0
         *
         * @param b one-dimensional std::vector<int_type> container
         */
        explicit extents(base_type const& b)
        : _base(b)
        {
            if (!this->valid()){
                throw std::length_error("Error in extents::extents() : shape tuple is not a valid permutation: has zero elements.");
            }
        }

        /** @brief Move constructs extents from a one-dimensional container
         *
         * @code auto ex = extents<unsigned>(  std::vector<unsigned>(3u,3u) );
         *
         * @note checks if size > 1 and all elements > 0
         *
         * @param b one-dimensional container of type std::vector<int_type>
         */
        explicit extents(base_type && b)
        : _base(std::move(b))
        {
            if (!this->valid()){
                throw std::length_error("Error in extents::extents() : shape tuple is not a valid permutation: has zero elements.");
            }
        }

        /** @brief Constructs extents from an initializer list
         *
         * @code auto ex = extents<unsigned>{3,2,4};
         *
         * @note checks if size > 1 and all elements > 0
         *
         * @param l one-dimensional list of type std::initializer<int_type>
         */
        extents(std::initializer_list<value_type> l)
        : extents( base_type(std::move(l)) )
        {
        }

        /** @brief Constructs extents from a range specified by two iterators
         *
         * @code auto ex = extents<unsigned>(a.begin(), a.end());
         *
         * @note checks if size > 1 and all elements > 0
         *
         * @param first iterator pointing to the first element
         * @param last iterator pointing to the next position after the last element
         */
        extents(const_iterator first, const_iterator last)
        : extents ( base_type( first,last ) )
        {
        }

        /** @brief Copy constructs extents */
        extents(extents const& l )
        : _base(l._base)
        {
        }

        /** @brief Move constructs extents */
        extents(extents && l ) noexcept
        : _base(std::move(l._base))
        {
        }

        ~extents() = default;

        extents& operator=(extents other) noexcept
        {
            swap (*this, other);
            return *this;
        }

        friend void swap(extents& lhs, extents& rhs) {
            std::swap(lhs._base   , rhs._base   );
        }


        constexpr auto rank() noexcept {
            return _base.size();
        }
        constexpr auto rank_dyanmic() noexcept {
            return _base.size();
        }
        constexpr auto static_extent(int) noexcept {
            return 0;
        }

        constexpr auto extent(int k) const noexcept {
            return _base.at(k);
        }

        auto size(int k) const noexcept {
            return std::accumulate(_base.cbegin() + k, _base.cend(),1ul,std::multiplies<>());
        }

        constexpr auto size() const noexcept {
            return product();
        }


        /** @brief Returns true if this has a scalar shape
         *
         * @returns true if (1,1,[1,...,1])
        */
        bool is_scalar() const
        {
            return
                _base.size() != 0 &&
                std::all_of(_base.begin(), _base.end(),
                            [](const_reference a){ return a == 1;});
        }

        /** @brief Returns true if this has a vector shape
         *
         * @returns true if (1,n,[1,...,1]) or (n,1,[1,...,1]) with n > 1
        */
        bool is_vector() const
        {
            if(_base.size() == 0){
                return false;
            }

            if(_base.size() == 1){
                return _base.at(0) > 1;
            }

            auto greater_one = [](const_reference a){ return a >  1;};
            auto equal_one   = [](const_reference a){ return a == 1;};

            return
                std::any_of(_base.begin(),   _base.begin()+2, greater_one) &&
                std::any_of(_base.begin(),   _base.begin()+2, equal_one  ) &&
                std::all_of(_base.begin()+2, _base.end(),     equal_one);
        }

        /** @brief Returns true if this has a matrix shape
         *
         * @returns true if (m,n,[1,...,1]) with m > 1 and n > 1
        */
        bool is_matrix() const
        {
            if(_base.size() < 2){
                return false;
            }

            auto greater_one = [](const_reference a){ return a >  1;};
            auto equal_one   = [](const_reference a){ return a == 1;};

            return
                std::all_of(_base.begin(),   _base.begin()+2, greater_one) &&
                std::all_of(_base.begin()+2, _base.end(),     equal_one  );
        }

        /** @brief Returns true if this is has a tensor shape
         *
         * @returns true if !empty() && !is_scalar() && !is_vector() && !is_matrix()
        */
        bool is_tensor() const
        {
            if(_base.size() < 3){
                return false;
            }

            auto greater_one = [](const_reference a){ return a > 1;};

            return std::any_of(_base.begin()+2, _base.end(), greater_one);
        }

        const_pointer data() const
        {
            return this->_base.data();
        }

        const_reference operator[] (size_type p) const
        {
            return this->_base[p];
        }

        const_reference at (size_type p) const
        {
            return this->_base.at(p);
        }

        reference operator[] (size_type p)
        {
            return this->_base[p];
        }

        reference at (size_type p)
        {
            return this->_base.at(p);
        }


        bool empty() const
        {
            return this->_base.empty();
        }

        // size_type size() const
        // {
        //     return this->_base.size();
        // }

        /** @brief Returns true if size > 1 and all elements > 0 */
        bool valid() const
        {
            return
                this->size() > 1 &&
                std::none_of(_base.begin(), _base.end(),
                            [](const_reference a){ return a == value_type(0); });
        }

        /** @brief Returns the number of elements a tensor holds with this */
        size_type product() const
        {
            if(_base.empty()){
                return 0;
            }

            return std::accumulate(_base.begin(), _base.end(), 1ul, std::multiplies<>());

        }


        /** @brief Eliminates singleton dimensions when size > 2
         *
         * squeeze {  1,1} -> {  1,1}
         * squeeze {  2,1} -> {  2,1}
         * squeeze {  1,2} -> {  1,2}
         *
         * squeeze {1,2,3} -> {  2,3}
         * squeeze {2,1,3} -> {  2,3}
         * squeeze {1,3,1} -> {  1,3}
         *
        */
        extents squeeze() const noexcept
        {
            if(this->size() <= 2){
                return *this;
            }

            auto new_extent = extents{};
            auto insert_iter = std::back_insert_iterator<typename extents::base_type>(new_extent._base);
            auto c = std::count_if(this->begin(),this->end(),[](auto& n){return n == value_type{1};});
            auto num = this->size() - c;
            if((*this)[0] == value_type{1} && (*this)[1] != value_type{1} && num == 1){
                new_extent._base.push_back(value_type{1});
                new_extent._base.push_back((*this)[1]);
            }else{
                std::remove_copy(this->_base.begin(), this->_base.end(), insert_iter ,value_type{1});
            }

            while(new_extent.size() < 2) new_extent._base.push_back(value_type{1});
            
            return new_extent;

        }

        void clear()
        {
            this->_base.clear();
        }

        bool operator == (extents const& b) const
        {
            return _base == b._base;
        }

        bool operator != (extents const& b) const
        {
            return !( _base == b._base );
        }

        const_iterator
        begin() const
        {
            return _base.begin();
        }

        const_iterator
        end() const
        {
            return _base.end();
        }

        base_type const& base() const { return _base; }

    private:
        base_type _base;
    };

    template< ptrdiff_t dims, ptrdiff_t ... StaticExtents >
    struct extents : private detail::extents_impl< 0, detail::make_seq_t<dims, StaticExtents...> >{

        using base_type = ptrdiff_t;
        using value_type = base_type;
        using const_reference = base_type const& ;
        using reference = base_type & ;
        using size_type = base_type;
        using const_pointer = base_type *const;
        using const_iterator = base_type;

        static constexpr auto rank() noexcept {
            return impl::Rank;
        }
        static constexpr auto rank_dyanmic() noexcept {
            return impl::DynamicRank;
        }
        static constexpr auto static_extent(int k) noexcept {
            return impl::static_extent(k);
        }

        constexpr auto extent(size_type k) const noexcept {
            return impl::extent(k);
        }

        constexpr auto at(size_type k) const noexcept {
            return extent(k);
        }

        constexpr auto size(size_type k) const noexcept {
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
            return Iterator<decltype(*this)>(*this,0);
        }

        constexpr auto end() const noexcept{
            return Iterator<decltype(*this)>(*this,rank());
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
                std::any_of(begin(), begin() + 2, greater_one) &&
                std::any_of(begin(), begin() + 2, equal_one  ) &&
                std::all_of(begin() + 2, end(), equal_one);
        }

        constexpr bool is_matrix() const noexcept{
            if(rank() < 2){
                return false;
            }

            auto greater_one = [](auto const& a){ return a >  1;};
            auto equal_one   = [](auto const& a){ return a == 1;};

            return
                std::all_of ( begin(), begin() + 2, greater_one) &&
                std::all_of ( begin() + 2, end(),     equal_one  );
        }

        constexpr bool is_tensor() const noexcept{
            if(rank() < 3){
                return false;
            }

            auto greater_one = [](auto const& a){ return a > 1;};

            return std::any_of(begin() + 2, end(), greater_one);
        }

        auto to_vector() const noexcept{
            std::vector<base_type> temp(dims);
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
                std::none_of(begin(), end(),
                            [](auto const& a){ return a == ptrdiff_t(0); });
        }

        constexpr auto product() const noexcept{
            return size();
        }

        template< ptrdiff_t rhs_dims, ptrdiff_t ... rhs >
        constexpr auto operator==(extents<rhs_dims,rhs...> const& other) const noexcept{
            if (rank() != other.rank()){
                return false;
            }
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

        extents<dynamic_dims> squeeze() const noexcept{
            std::vector<base_type> arr(dims);
            for(auto i = 0u; i < rank(); i++){
                arr.at(i) = at(i);
            }
            extents<dynamic_dims> e(arr.begin(),arr.end());
            return e.squeeze();
        }

        ~extents() = default;

    private:
        using impl = detail::extents_impl< 0, detail::make_seq_t<dims, StaticExtents...> > ;
        
        template< typename Parent >
        struct Iterator{

            Iterator(Parent const& p,size_type pos):_p(p),_pos(pos){}

            bool operator==( Iterator const& rhs){
                return rhs._pos == _pos;
            }

            bool operator!=( Iterator const& rhs){
                return !(*this == rhs);
            }

            Iterator& operator++(){
                ++_pos;
                return *this;
            }

            Iterator operator++(int){
                auto temp = *this;
                ++(*this);
                return temp;
            }

            Iterator operator+(size_type k){
                assert(k >= size_type{0} && k < impl::Rank && _pos + k < impl::Rank);
                auto temp = *this;
                temp._pos += k;
                return temp;
            }

            Iterator& operator+=(size_type k){
                assert(k >= size_type{0} && k < impl::Rank && _pos + k < impl::Rank);
                _pos += k;
                return *this;
            }

            constexpr auto operator*() const noexcept {
                return _p.at(_pos);
            }

        private:
            Parent const& _p;
            size_type _pos{0};
        };
    };

    template < ptrdiff_t D, ptrdiff_t ...E >
    using extents_t = std::conditional_t<
        D == dynamic_dims,
        extents<D>,
        std::conditional_t<
            ( D > 500 || D < 0),
            extents<dynamic_dims>,
            extents<D>
        >
    >;

}


#endif // MDSPAN_H
