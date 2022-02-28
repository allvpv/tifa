#pragma once
#include <stdint.h>
#include <limits.h>
#include <type_traits.hpp>

#define __TIFA_IMPL_CRITICAL_STATIC_ASSERT_FAILURE  "This should not happen. " \
    "Report the issue at https://github.com/allvpv/tifa/issues"

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

namespace tifa::impl {
    template<typename T> struct primitive_unsigned_equivalent { using type = T; };
    template<typename T> struct primitive_signed_equivalent { using type = T; };

    template<> struct primitive_unsigned_equivalent<char> { using type = unsigned char; };
    template<> struct primitive_unsigned_equivalent<signed char> { using type = unsigned char; };
    template<> struct primitive_unsigned_equivalent<short> { using type = unsigned short; };
    template<> struct primitive_unsigned_equivalent<int> { using type = unsigned int; };
    template<> struct primitive_unsigned_equivalent<long> { using type = unsigned long; };
    template<> struct primitive_unsigned_equivalent<long long> { using type = unsigned long long; };

    template<> struct primitive_signed_equivalent<char> { using type = signed char; };
    template<> struct primitive_signed_equivalent<unsigned char> { using type = signed char; };
    template<> struct primitive_signed_equivalent<unsigned short> { using type = short; };
    template<> struct primitive_signed_equivalent<unsigned int> { using type = int; };
    template<> struct primitive_signed_equivalent<unsigned long> { using type = long; };
    template<> struct primitive_signed_equivalent<unsigned long long> { using type = long long; };
}

namespace tifa::impl {
    template<typename T>
    using primitive_signed_equivalent_t = copy_qualifiers_t<
        T, typename primitive_signed_equivalent<dequalified_t<T>>::type
    >;

    template<typename T>
    using primitive_unsigned_equivalent_t = copy_qualifiers_t<
        T, typename primitive_unsigned_equivalent<dequalified_t<T>>::type
    >;
}

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
    template<primitive_int_ct I>
    struct integer;

    template<typename T>
    concept tifa_int_ct =
        primitive_int_ct<typename T::underlying_t> &&
        is_same_ct<integer<typename T::underlying_t>, T>;

    template<typename T>
    concept int_ct = tifa_int_ct<T> || primitive_int_ct<T>;
}

namespace tifa::impl {
    template<typename T>
    struct corresponding_primitive_type_helper;

    template<primitive_int_ct T>
    struct corresponding_primitive_type_helper<T> {
        using type = T;
    };

    template<tifa_int_ct T>
    struct corresponding_primitive_type_helper<T> {
        using type = typename T::underlying_t;
    };

    template<int_ct T>
    using corresponding_primitive_t =
        typename corresponding_primitive_type_helper<dequalified_t<T>>::type;
}

namespace tifa::impl {
    template<typename T>
    struct corresponding_tifa_type_helper {
        using type = void;
    };

    template<primitive_int_ct T>
    struct corresponding_tifa_type_helper<T> {
        using type = integer<T>;
    };

    template<tifa_int_ct T>
    struct corresponding_tifa_type_helper<T> {
        using type = T;
    };

    template<typename T>
    using corresponding_tifa_int_t =
        typename corresponding_tifa_type_helper<dequalified_t<T>>::type;
}

namespace tifa::impl {
    template<typename T>
    struct limit_helper;

    template<>
    struct limit_helper<signed char> {
        static constexpr signed char min = SCHAR_MIN;
        static constexpr signed char max = SCHAR_MAX;
    };

    template<>
    struct limit_helper<short> {
        static constexpr short min = SHRT_MIN;
        static constexpr short max = SHRT_MAX;
    };

    template<>
    struct limit_helper<int> {
        static constexpr int min = INT_MIN;
        static constexpr int max = INT_MAX;
    };

    template<>
    struct limit_helper<long> {
        static constexpr long min = LONG_MIN;
        static constexpr long max = LONG_MAX;
    };

    template<>
    struct limit_helper<long long> {
        static constexpr long long min = LLONG_MIN;
        static constexpr long long max = LLONG_MAX;
    };

    template<>
    struct limit_helper<unsigned char> {
        static constexpr unsigned char min = 0;
        static constexpr unsigned char max = UCHAR_MAX;
    };

    template<>
    struct limit_helper<unsigned short> {
        static constexpr unsigned short min = 0;
        static constexpr unsigned short max = USHRT_MAX;
    };

    template<>
    struct limit_helper<unsigned int> {
        static constexpr unsigned int min = 0;
        static constexpr unsigned int max = UINT_MAX;
    };

    template<>
    struct limit_helper<unsigned long> {
        static constexpr unsigned long min = 0;
        static constexpr unsigned long max = ULONG_MAX;
    };

    template<>
    struct limit_helper<unsigned long long> {
        static constexpr long long min = 0;
        static constexpr long long max = ULLONG_MAX;
    };

    template<>
    struct limit_helper<char> {
        static constexpr char min = CHAR_MIN;
        static constexpr char max = CHAR_MAX;
    };

    template<tifa_int_ct T>
    struct limit_helper<T> {
        static constexpr T min = limit_helper<typename T::underlying_t>::min;
        static constexpr T max = limit_helper<typename T::underlying_t>::max;
    };
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
                corresponding_primitive_t<int_src_t>,
                corresponding_primitive_t<int_dst_t>
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
    requires primitive_unsigned_int_ct<T> &&
             primitive_signed_int_ct<U> &&
             greater_size_ct<U, T>
    struct arithmetic_operation_conversion_helper<T, U> {
        using type = U;
    };

    template<typename T, typename U>
    struct arithmetic_operation_conversion;

    template<typename T, typename U>
    requires int_ct<T> && int_ct<U>
    struct arithmetic_operation_conversion<T, U> {
        using type = corresponding_tifa_int_t<
            typename arithmetic_operation_conversion_helper<
                corresponding_primitive_t<T>,
                corresponding_primitive_t<U>
            >::type>;
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
        using type = primitive_unsigned_equivalent_t<T>;
    };

    template<typename T, typename U>
    requires primitive_unsigned_int_ct<T> &&
             primitive_signed_int_ct<U> &&
             primitive_int_greater_rank_ct<U, T>
    struct bitwise_operation_conversion_helper<T, U> {
        using type = primitive_unsigned_equivalent_t<U>;
    };

    template<typename T, typename U>
    struct bitwise_operation_conversion {
        using type = corresponding_tifa_int_t<
            typename bitwise_operation_conversion_helper<
                corresponding_primitive_t<T>,
                corresponding_primitive_t<U>
            >::type>;
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
        constexpr bool is_safe = bitwise_operation_conversion<lhs_t, rhs_t>::is_safe;
        constexpr bool equal_size = sizeof(lhs_t) == sizeof(rhs_t);
        constexpr bool safety_iff_equal_size = (equal_size && is_safe) || (!equal_size && !is_safe);

        static_assert(safety_iff_equal_size, __TIFA_IMPL_CRITICAL_STATIC_ASSERT_FAILURE);
        static_assert(sizeof(lhs_t) == sizeof(rhs_t),
                "Bitwise operation on types with different width requested. "
                "Use explicit cast or zeroext() or signext() method");
    }

    template<typename src_t, typename dst_t>
    consteval void safe_extend_operation_assert() {
        static_assert(sizeof(dst_t) >= sizeof(src_t),
                "Extension is possible only from narrower to wider type");
    }
}

namespace tifa::impl {
    /* Do not instantiate integer<> class when underlying type has qualifiers */
    template<typename T>
    concept is_dequalified =
        is_same_ct<T, dequalified_t<T>>;

    template<primitive_int_ct I>
    requires is_dequalified<I>
    struct integer<I> {
        using underlying_t = I;
        underlying_t value;

        /* Implicit constructor */
        template<int_ct src_t>
        requires safe_conversion_from_to_ct<src_t, integer>
        constexpr integer(src_t src) {
            value = src;
        }

        template<int_ct src_t>
        requires safe_conversion_from_to_ct<src_t, integer>
        constexpr void operator=(src_t src) {
            value = src;
        }

        /* Implicit conversion operator */
        template<primitive_int_ct dst_t>
        requires safe_conversion_from_to_ct<integer, dst_t>
        constexpr operator dst_t() const {
            return value;
        }

        /* Explicit constructor */
        template<typename src_t>
        constexpr explicit integer(src_t src) {
            value = static_cast<underlying_t>(src);
        }

        /* Explicit conversion operator */
        template<typename dst_t>
        constexpr explicit operator dst_t() const {
            return static_cast<dst_t>(value);
        }

        /* Operators */
        constexpr integer operator-() const {
            static_assert(primitive_signed_int_ct<underlying_t>,
                    "To get two's complement of unsigned integer, use complement() method");
            return integer(-value);
        }

        constexpr integer operator++(int) { return value++; }
        constexpr integer operator--(int) { return value--; }

        constexpr integer& operator++() { ++value; return *this; }
        constexpr integer& operator--() { --value; return *this; }
    };
}
