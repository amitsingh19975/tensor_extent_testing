#ifndef SEQ_H
#define SEQ_H

#include <iostream>
#include <type_traits>

namespace mdspan{
    constexpr ptrdiff_t dynamic_extent{-1};
    constexpr ptrdiff_t static_dims{-2};
}

namespace mdspan::detail{
    template< ptrdiff_t ...Is >
    struct seq{
        using type = seq;
        static constexpr ptrdiff_t dims = sizeof...(Is);
        static constexpr bool is_empty  = false;
    };

    template<>
    struct seq<>{
        using type = seq;
        static constexpr ptrdiff_t dims = 0;
        static constexpr bool is_empty  = true;
    };

    template< ptrdiff_t ...Is> 
    struct make_seq_impl{
        using type = seq < Is... >;
    };

    template < ptrdiff_t ...Is >
    using make_seq_impl_t = typename make_seq_impl<Is...>::type;

    template< ptrdiff_t dims_start, ptrdiff_t dims_end > 
    struct make_seq_dynamic_impl;

    template <ptrdiff_t dims_start, ptrdiff_t dims_end >
    using make_seq_dynamic_impl_t = typename make_seq_dynamic_impl<dims_start, dims_end>::type;
    
    template <ptrdiff_t dims>
    using make_seq_dynamic_t = typename make_seq_dynamic_impl<0, dims - 1>::type;

    template <class S1, class S2> 
    struct concat_seq;
    
    template <class S1, class S2> 
        using  concat_seq_t = typename concat_seq<S1,S2>::type;

    template < ptrdiff_t... lhs, ptrdiff_t... rhs>
    struct concat_seq< seq <lhs...>, seq <rhs...> > {
        using type = seq<lhs..., rhs...>;
    };

    template < ptrdiff_t dims_start, ptrdiff_t dims_end >
    struct make_seq_dynamic_impl {
        using type = concat_seq_t< make_seq_dynamic_impl_t< dims_start, ( dims_start + dims_end ) / 2 >, 
                            make_seq_dynamic_impl_t< ( dims_start + dims_end ) / 2 + 1, dims_end > >;
    };

    template <ptrdiff_t I>
    struct make_seq_dynamic_impl<I,I> {
        using type = seq <dynamic_extent>;
    };

    template< ptrdiff_t dims, ptrdiff_t ...Extents >
    struct make_seq;

    template< ptrdiff_t dims >
    struct make_seq<dims>{
        using type = make_seq_dynamic_t<dims>;
    };
    
    template< ptrdiff_t dims, ptrdiff_t el>
    struct make_seq<dims, el>{
        using type = make_seq_impl_t<el>;
    };
    
    template< ptrdiff_t dims, ptrdiff_t el, ptrdiff_t ...Extents >
    struct make_seq<dims, el, Extents...>{
        static_assert(dims == sizeof...(Extents) + 1," DIMENSION SHOULD BE EQUAL TO THE PARAMETER PACK ");
        using type = make_seq_impl_t<el, Extents...>;
    };
    
    template< >
    struct make_seq< -2 >{
        using type = seq<>;
    };

    template < ptrdiff_t dims, ptrdiff_t ...Extents >
    using make_seq_t = typename make_seq<dims,Extents...>::type;

    template < typename Seq >
    struct is_seq : std::false_type{};

    template < ptrdiff_t ...Extents >
    struct is_seq< seq<Extents...> > : std::true_type{};


    template< typename S1, typename S2 >
    struct compare_seq_equal;

    template<>
    struct compare_seq_equal< seq<>, seq<> >{
        static constexpr bool value = true;
    };

    template<ptrdiff_t el, ptrdiff_t ...rhs>
    struct compare_seq_equal< seq<>, seq<el,rhs...> >{
        static constexpr bool value = false;
    };

    template<ptrdiff_t el, ptrdiff_t ...lhs>
    struct compare_seq_equal< seq<el,lhs...>, seq<> >{
        static constexpr bool value = false;
    };
    template<ptrdiff_t el_1, ptrdiff_t ...lhs,ptrdiff_t el_2, ptrdiff_t ...rhs>
    struct compare_seq_equal< seq<el_1,lhs...>, seq<el_2,rhs...> >{
        static constexpr bool value = (el_1 == el_2) && compare_seq_equal< seq<lhs...>, seq<rhs...> >::value;
    };

}

namespace mdspan::detail{

    template < ptrdiff_t index, typename Seq >
    struct get_impl;

    template < ptrdiff_t el  >
    struct get_impl< 0, seq< el > >{
        constexpr auto operator()(){
            return el;
        }
    };

    template < ptrdiff_t el, ptrdiff_t ...Extents >
    struct get_impl<0, seq< el, Extents... >>{
        constexpr auto operator()(){
            return el;
        }
    };

    template < ptrdiff_t index, ptrdiff_t el, ptrdiff_t ...Extents >
    struct get_impl<index, seq< el, Extents... >>{
        
        static_assert(seq< el, Extents... >::dims > index && index >= 0);

        constexpr auto operator()(){
            return get_impl<index - 1, seq<Extents...> >()();
        }
    };

    template < ptrdiff_t index, ptrdiff_t ...Extents >
    ptrdiff_t get(seq<Extents...> s){
        return get_impl<index, seq<Extents...> >()();
    }

}

namespace mdspan::detail{

    template < ptrdiff_t index, typename Seq >
    struct remove_item_impl;

    template <>
    struct remove_item_impl<0, seq<> >{
        constexpr seq<> operator()(){
            return {};
        }
    };

    template <ptrdiff_t el, ptrdiff_t ...Extents>
    struct remove_item_impl<0, seq< el, Extents...> >{
        constexpr seq<Extents...> operator()(){
            return {};
        }
    };

    template <ptrdiff_t index, ptrdiff_t el, ptrdiff_t ...Extents>
    struct remove_item_impl<index, seq< el, Extents...> >{
        
        static_assert(seq< el, Extents... >::dims > index && index >= 0);

        constexpr decltype(auto) operator()(){
            auto lhs = remove_item_impl<index - 1, seq<Extents...> >()();
            using type = decltype(lhs);
            return concat_seq_t< seq<el>, type >{};
        }
    };

    template < ptrdiff_t index, ptrdiff_t ...Extents>
    decltype(auto) remove_item(seq<Extents...> s){
        return remove_item_impl<index, seq<Extents...> >()();
    }
}

#endif // SEQ_H
