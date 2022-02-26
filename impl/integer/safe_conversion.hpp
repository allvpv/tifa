#pragma once
#include <impl/integer/tifa_int_traits.hpp>

#define __TIFA_IMPL_CRITICAL_STATIC_ASSERT_FAILURE  "This should not happen. " \
    "Report the issue at https://github.com/allvpv/tifa/issues"

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

    template<typename T, typename U>
    concept primitive_int_greater_or_equal_rank_ct =
        (primitive_int_conv_rank<T>() >= primitive_int_conv_rank<U>());
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
    struct arithmetic_operation_conversion_helper {
        using type = void;
    };

    template<typename T>
    requires primitive_int_ct<T>
    struct arithmetic_operation_conversion_helper<T, T> {
        using type = T;
    };

    template<typename T, typename U>
    requires primitive_int_same_signedness_ct<T, U> &&
             primitive_int_greater_rank_ct<T, U>
    struct arithmetic_operation_conversion_helper<T, U> {
        using type = T;
    };

    template<typename T, typename U>
    requires primitive_int_same_signedness_ct<T, U> &&
             primitive_int_greater_rank_ct<U, T>
    struct arithmetic_operation_conversion_helper<T, U> {
        using type = U;
    };

    template<typename T, typename U>
    requires primitive_signed_int_ct<T> &&
             primitive_unsigned_int_ct<U> &&
             greater_size_ct<T, U>
    struct arithmetic_operation_conversion_helper<T, U> {
        using type = T;
    };

    template<typename T, typename U>
    struct arithmetic_operation_conversion;

    template<typename T, typename U>
    requires int_ct<T> && int_ct<U>
    struct arithmetic_operation_conversion<T, U> {
        using type = typename arithmetic_operation_conversion_helper<
            underlying_primitive_t<T>,
            underlying_primitive_t<U>
        >::type;
        static constexpr bool is_safe = !is_same_ct<type, void>;
    };
}

namespace tifa::impl {
    template<typename T, typename U>
    struct bitwise_operation_conversion_helper {
        using type = void;
    };

    template<typename T>
    requires primitive_int_ct<T>
    struct bitwise_operation_conversion_helper<T, T> {
        using type = T;
    };

    template<typename T, typename U>
    requires primitive_int_same_signedness_ct<T, U> &&
             primitive_int_greater_rank_ct<T, U>
    struct bitwise_operation_conversion_helper<T, U> {
        using type = T;
    };

    template<typename T, typename U>
    requires primitive_int_same_signedness_ct<T, U> &&
             primitive_int_greater_rank_ct<U, T>
    struct bitwise_operation_conversion_helper<T, U> {
        using type = U;
    };

    template<typename T, typename U>
    requires primitive_unsigned_int_ct<T> &&
             primitive_signed_int_ct<U> &&
             primitive_int_greater_or_equal_rank_ct<T, U>
    struct bitwise_operation_conversion_helper<T, U> {
        using type = T;
    };

    template<typename T, typename U>
    requires primitive_signed_int_ct<T> &&
             primitive_unsigned_int_ct<U> &&
             primitive_int_greater_or_equal_rank_ct<U, T>
    struct bitwise_operation_conversion_helper<T, U> {
        using type = U;
    };

    template<typename T, typename U>
    requires primitive_signed_int_ct<T> &&
             primitive_unsigned_int_ct<U> &&
             primitive_int_greater_rank_ct<T, U>
    struct bitwise_operation_conversion_helper<T, U> {
        using type = unsigned_equivalent_t<T>;
    };

    template<typename T, typename U>
    requires primitive_unsigned_int_ct<T> &&
             primitive_signed_int_ct<U> &&
             primitive_int_greater_rank_ct<U, T>
    struct bitwise_operation_conversion_helper<T, U> {
        using type = unsigned_equivalent_t<U>;
    };

    template<typename T, typename U>
    struct bitwise_operation_conversion {
        using type = typename bitwise_operation_conversion_helper<
            underlying_primitive_t<T>,
            underlying_primitive_t<U>
        >::type;
        static constexpr bool is_safe = !is_same_ct<type, void>;
    };
}

namespace tifa::impl {
    template<typename lhs_t, typename rhs_t>
    consteval void safe_arithmetic_operation_conversion_assert() {
        static_assert(arithmetic_operation_conversion<lhs_t, rhs_t>::is_safe,
                "Cannot safely convert between integral types. "
                "Use proper numeric literal, change your types or use explicit cast");
    }

    template<typename lhs_t, typename rhs_t>
    consteval void safe_assignment_operation_conversion_assert() {
        static_assert(safe_conversion_from_to_ct<lhs_t, rhs_t>,
                "Cannot safely convert between integral types. "
                "Use proper numeric literal, change your types or use explicit cast");
    }

    template<typename lhs_t, typename rhs_t>
    consteval void safe_bitwise_operation_conversion_assert() {
        constexpr bool safety_iff_equal_size =
            (sizeof(lhs_t) == sizeof(rhs_t) && bitwise_operation_conversion<lhs_t, rhs_t>::is_safe) ||
            (sizeof(lhs_t) != sizeof(rhs_t) && !bitwise_operation_conversion<lhs_t, rhs_t>::is_safe);

        static_assert(safety_iff_equal_size, __TIFA_IMPL_CRITICAL_STATIC_ASSERT_FAILURE);
        static_assert(sizeof(lhs_t) == sizeof(rhs_t),
                "arithmetic operation on types with different width requested. "
                "Use explicit cast or zeroext() or signext() method");
    }

    template<typename src_t, typename dst_t>
    consteval void safe_extend_operation_assert() {
        static_assert(sizeof(dst_t) <= sizeof(src_t),
                "Extension is possible only from narrower to wider type");
    }
}
