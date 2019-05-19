#ifndef STORAGE_POLICY_H
#define STORAGE_POLICY_H

#include <unordered_map>

namespace compressions_policy{
    
    template< typename T, typename A >
    struct map_compression{

        map_compression(A const& obj){
            compress(obj);
        }

        void compress(A);
        A uncompress();
        T& at(size_t);
        T at(size_t) const;
        void insert(T, size_t);
    private:
        std::unordered_map<size_t,T> _m;
    };
}

namespace storage_type{
    template < typename T, typename A, typename C = compressions_policy::map_compression<T,A> >
    struct sparse{
        sparse() = default;
        sparse( A const& obj):_m(obj){}

        T& at(size_t k){ return _m.at(k);}
        T at(size_t k) const { return _m.at(k);}
        void insert(T val, size_t k){ _m.insert(val,k);}
    private:
        C _m;
    };
    
    template < typename T, typename A >
    struct dense{};
}

#endif // STORAGE_POLICY_H
