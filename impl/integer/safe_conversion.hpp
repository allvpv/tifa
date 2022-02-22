#pragma once 
#include <impl/integer/primitive_int_traits.hpp>
#include <impl/integer/tifa_int_traits.hpp>

namespace tifa::impl {
    template<typename T>
    consteval int primitive_int_conv_rank_helper();

    template<> consteval int primitive_int_conv_rank_helper<signed char>() { return 1; };
    template<> consteval int primitive_int_conv_rank_helper<short>() { return 2; };
    template<> consteval int primitive_int_conv_rank_helper<int>() { return 3; };
    template<> consteval int primitive_int_conv_rank_helper<long>() { return 4; };
    template<> consteval int primitive_int_conv_rank_helper<long long>() { return 5; };

    template<> consteval int primitive_int_conv_rank_helper<unsigned char>() { return 1; };
    template<> consteval int primitive_int_conv_rank_helper<unsigned short>() { return 2; };
    template<> consteval int primitive_int_conv_rank_helper<unsigned int>() { return 3; };
    template<> consteval int primitive_int_conv_rank_helper<unsigned long>() { return 4; };
    template<> consteval int primitive_int_conv_rank_helper<unsigned long long>() { return 5; };

    template<> consteval int primitive_int_conv_rank_helper<char>() { return 1; };

    template<typename T> consteval int primitive_int_conv_rank() {
        return primitive_int_conv_rank_helper<dequalified_t<T>>();
    }

    template<typename T, typename U>
    concept primitive_int_greater_rank_ct =
        (primitive_int_conv_rank<T>() > primitive_int_conv_rank<U>());
};

namespace tifa::impl {
    template<typename int_src_t, typename int_dst_t>
    concept safe_conversion_from_to_helper_ct = (
            primitive_int_same_signedness_ct<int_src_t, int_dst_t> &&
            greater_or_equal_size_ct<int_dst_t, int_src_t>
        ) || (
            primitive_unsigned_int_ct<int_src_t> &&
            primitive_signed_int_ct<int_dst_t> &&
            greater_size_ct<int_dst_t, int_src_t>
        );

    template<typename int_src_t, typename int_dst_t>
    concept safe_conversion_from_to_ct =
            safe_conversion_from_to_helper_ct<
                underlying_primitive_t<int_src_t>,
                underlying_primitive_t<int_dst_t>
            >;
};

namespace tifa::impl {
    template<typename T, typename U>
    struct binary_operation_conversion_helper {
        using type = void;
    };

    template<typename T>
    requires primitive_int_ct<T>
    struct binary_operation_conversion_helper<T, T> {
        using type = T;
    };

    template<typename T, typename U>
    requires primitive_int_same_signedness_ct<T, U> &&
             primitive_int_greater_rank_ct<T, U>
    struct binary_operation_conversion_helper<T, U> {
        using type = T;
    };

    template<typename T, typename U>
    requires primitive_int_same_signedness_ct<T, U> &&
             primitive_int_greater_rank_ct<U, T>
    struct binary_operation_conversion_helper<T, U> {
        using type = U;
    };

    template<typename T, typename U>
    requires primitive_signed_int_ct<T> &&
             primitive_unsigned_int_ct<U> &&
             greater_size_ct<T, U>
    struct binary_operation_conversion_helper<T, U> {
        using type = T;
    };

    template<typename T, typename U>
    struct binary_operation_conversion;

    template<typename T, typename U>
    requires int_ct<T> && int_ct<U>
    struct binary_operation_conversion<T, U> {
        using type = typename binary_operation_conversion_helper<
            underlying_primitive_t<T>,
            underlying_primitive_t<U>
        >::type;
        static constexpr bool is_safe = !is_same_ct<type, void>;
    };

    template<typename lhs_t, typename rhs_t>
    consteval void safe_binary_operation_conversion_assert() {
        static_assert(binary_operation_conversion<lhs_t, rhs_t>::is_safe,
                "Cannot safely convert between integral types. "
                "Use proper numeric literal, change your types or use explicit cast.");
    }
}
