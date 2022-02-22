#pragma once 
#include <impl/type_traits.hpp>

namespace tifa::impl {
    template<typename T> struct is_primitive_signed_int_helper : constraint<false> {};
    template<> struct is_primitive_signed_int_helper<signed char> : constraint<true> {};
    template<> struct is_primitive_signed_int_helper<short> : constraint<true> {};
    template<> struct is_primitive_signed_int_helper<int> : constraint<true> {};
    template<> struct is_primitive_signed_int_helper<long> : constraint<true> {};
    template<> struct is_primitive_signed_int_helper<long long> : constraint<true> {};

    template<typename T> struct is_primitive_unsigned_int_helper : constraint<false> {};
    template<> struct is_primitive_unsigned_int_helper<unsigned char> : constraint<true> {};
    template<> struct is_primitive_unsigned_int_helper<unsigned short> : constraint<true> {};
    template<> struct is_primitive_unsigned_int_helper<unsigned int> : constraint<true> {};
    template<> struct is_primitive_unsigned_int_helper<unsigned long> : constraint<true> {};
    template<> struct is_primitive_unsigned_int_helper<unsigned long long> : constraint<true> {};

#if CHAR_MIN == 0
    template<> struct is_primitive_unsigned_int_helper<char> : constraint<true> {};
#else
    template<> struct is_primitive_signed_int_helper<char> : constraint<true> {};
#endif

    template<typename T> struct is_primitive_signed_int
        : is_primitive_signed_int_helper<dequalified_t<T>>::type {};

    template<typename T> struct is_primitive_unsigned_int
        : is_primitive_unsigned_int_helper<dequalified_t<T>>::type {};

};

namespace tifa::impl {
    template<typename T>
    concept primitive_signed_int_ct = is_primitive_signed_int<T>::test;

    template<typename T>
    concept primitive_unsigned_int_ct = is_primitive_unsigned_int<T>::test;
    
    template<typename T>
    concept primitive_int_ct = primitive_signed_int_ct<T> || primitive_unsigned_int_ct<T>;

    template<typename T, typename U>
    concept primitive_int_same_signedness_ct =
        primitive_int_ct<T> && primitive_int_ct<U> && (
            (primitive_signed_int_ct<T> && primitive_signed_int_ct<U>) ||
            (primitive_unsigned_int_ct<T> && primitive_unsigned_int_ct<U>)
        );
};
