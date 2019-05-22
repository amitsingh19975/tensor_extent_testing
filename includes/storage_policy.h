#ifndef STORAGE_POLICY_H
#define STORAGE_POLICY_H

#include <unordered_map>
#include "mdspan.h"

namespace storage_type{
    namespace sparse_tensor{
        
        template< typename T >
        struct storage_interface{
            virtual void compress() = 0;
            virtual std::vector<T> uncompress() = 0;
            virtual T at(size_t) const = 0;
            virtual void set(T, size_t) = 0;
            virtual T get(size_t) = 0;
        };

        template< typename T>
        struct map_compression: storage_interface<T>{
            void compress() override {}

            std::vector<T> uncompress() override {return {};}
            
            T at(size_t) const override{return 0;}

            void set(T, size_t) override{}
            T get(size_t) override{return T{};}
        private:
            std::unordered_map<size_t,T> _m;
        };

    }
    
    namespace dense_tensor{
        
        template< typename T >
        struct storage_interface{
            virtual T& at(size_t) = 0;
            virtual T at(size_t) const = 0;
            virtual void set(T, size_t) = 0;
            virtual T get(T, size_t) = 0;
        };

        template < typename T, typename A >
        struct dense:storage_interface<T>{
            T& at(size_t) override {return _m[0];};
            T at(size_t) const override{return _m[0];};
            void set(T, size_t) override{};
            T get(T, size_t) override{return at(0);};
            std::vector<T> _m;
        };

    }

}

#endif // STORAGE_POLICY_H
