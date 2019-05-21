#ifndef STORAGE_POLICY_H
#define STORAGE_POLICY_H

#include <unordered_map>

namespace compressions_policy{
    
    template< typename T>
    struct map_compression{

        map_compression(std::vector<T> const& obj){
            compress(obj);
        }
        map_compression() = default;
        void compress(std::vector<T>);
        std::vector<T> uncompress();
        // T& at(size_t){return _m[0];}
        T at(size_t) const{return 0;}
        T at(size_t) {return 0;}
        void insert(T, size_t){}
    private:
        std::unordered_map<size_t,T> _m;
    };
}

namespace storage_type{
    template < typename T, typename C = compressions_policy::map_compression<T>>
    struct sparse : public C {
        sparse() = default;
        sparse( std::vector<T> const& obj ):C(obj){}
    };
    
    template < typename T, typename A >
    struct dense{};
}

#endif // STORAGE_POLICY_H
