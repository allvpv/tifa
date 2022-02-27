#pragma once
#include <impl/integer/tifa_int.hpp>
#include <impl/literals.hpp>

namespace tifa {
    using u8 = impl::integer<uint8_t>;
    using s8 = impl::integer<int8_t>;
    using u16 = impl::integer<uint16_t>;
    using s16 = impl::integer<int16_t>;
    using u32 = impl::integer<uint32_t>;
    using s32 = impl::integer<int32_t>;
    using u64 = impl::integer<uint64_t>;
    using s64 = impl::integer<int64_t>;
}

namespace tifa {
    /* true if T is a built-in integer or proper instantiation of integer<>
     * templated class */
    template<typename T>
    concept int_ct = impl::int_ct<T>;

    /* true if T is a proper instantiation of integer<> templated class */
    template<typename T>
    concept tifa_int_ct = impl::tifa_int_ct<T>;

    /* true if T is a built-in integer */
    template<typename T>
    concept primitive_int_ct = impl::primitive_int_ct<T>;
}

#define __TIFA_IMPL_DEFINE_LOGIC_OPERATOR(operator_name, operand)     \
    template<primitive_int_ct lhs_t, tifa_int_ct rhs_t>               \
    constexpr bool operator_name (lhs_t lhs, rhs_t rhs) {             \
        using namespace impl;                                         \
        safe_arithmetic_operation_conversion_assert<lhs_t, rhs_t>();  \
        return lhs operand rhs.value;                                 \
    }                                                                 \
                                                                      \
    template<tifa_int_ct lhs_t, primitive_int_ct rhs_t>               \
    constexpr bool operator_name (lhs_t lhs, rhs_t rhs) {             \
        using namespace impl;                                         \
        safe_arithmetic_operation_conversion_assert<lhs_t, rhs_t>();  \
        return lhs.value operand rhs;                                 \
    }                                                                 \
                                                                      \
    template<tifa_int_ct lhs_t, tifa_int_ct rhs_t>                    \
    constexpr bool operator_name (lhs_t lhs, rhs_t rhs) {             \
        using namespace impl;                                         \
        safe_arithmetic_operation_conversion_assert<lhs_t, rhs_t>();  \
        return lhs.value operand rhs.value;                           \
    }

namespace tifa {
__TIFA_IMPL_DEFINE_LOGIC_OPERATOR(operator==, ==)
__TIFA_IMPL_DEFINE_LOGIC_OPERATOR(operator!=, !=)
__TIFA_IMPL_DEFINE_LOGIC_OPERATOR(operator<, <)
__TIFA_IMPL_DEFINE_LOGIC_OPERATOR(operator>, >)
__TIFA_IMPL_DEFINE_LOGIC_OPERATOR(operator<=, <=)
__TIFA_IMPL_DEFINE_LOGIC_OPERATOR(operator>=, >=)
}

#define __TIFA_IMPL_DEFINE_ARITHMETIC_OPERATOR(operator_name, operand)                    \
    template<tifa_int_ct lhs_t, tifa_int_ct rhs_t>                                        \
    constexpr auto operator_name (lhs_t lhs, rhs_t rhs) {                                 \
        using namespace impl;                                                             \
        safe_arithmetic_operation_conversion_assert<lhs_t, rhs_t>();                      \
        using result_type = typename arithmetic_operation_conversion<lhs_t, rhs_t>::type; \
        return static_cast<corresponding_tifa_int_t<result_type>>(                        \
                lhs.value operand rhs.value);                                             \
    }                                                                                     \
                                                                                          \
    template<tifa_int_ct lhs_t, primitive_int_ct rhs_t>                                   \
    constexpr auto operator_name (lhs_t lhs, rhs_t rhs) {                                 \
        using namespace impl;                                                             \
        safe_arithmetic_operation_conversion_assert<lhs_t, rhs_t>();                      \
        using result_type = typename arithmetic_operation_conversion<lhs_t, rhs_t>::type; \
        return static_cast<corresponding_tifa_int_t<result_type>>(                        \
                lhs.value operand rhs);                                                   \
    }                                                                                     \
                                                                                          \
    template<primitive_int_ct lhs_t, tifa_int_ct rhs_t>                                   \
    constexpr auto operator_name (lhs_t lhs, rhs_t rhs) {                                 \
        using namespace impl;                                                             \
        safe_arithmetic_operation_conversion_assert<lhs_t, rhs_t>();                      \
        using result_type = typename arithmetic_operation_conversion<lhs_t, rhs_t>::type; \
        return static_cast<corresponding_tifa_int_t<result_type>>(                        \
                lhs operand rhs.value);                                                   \
    }

namespace tifa {
__TIFA_IMPL_DEFINE_ARITHMETIC_OPERATOR(operator+, +)
__TIFA_IMPL_DEFINE_ARITHMETIC_OPERATOR(operator-, -)
__TIFA_IMPL_DEFINE_ARITHMETIC_OPERATOR(operator*, *)
__TIFA_IMPL_DEFINE_ARITHMETIC_OPERATOR(operator/, /)
__TIFA_IMPL_DEFINE_ARITHMETIC_OPERATOR(operator%, %)
}

#define __TIFA_IMPL_DEFINE_BITWISE_OPERATOR(operator_name, operand)     \
    template<tifa_int_ct lhs_t, tifa_int_ct rhs_t>                      \
    constexpr auto operator_name (lhs_t lhs, rhs_t rhs) {               \
        using namespace impl;                                           \
        safe_bitwise_operation_conversion_assert<lhs_t, rhs_t>();       \
        using result_type = corresponding_tifa_int_t<                   \
            typename bitwise_operation_conversion<lhs_t, rhs_t>::type>; \
        return static_cast<result_type>(lhs.value operand rhs.value);   \
    }                                                                   \
                                                                        \
    template<primitive_int_ct lhs_t, tifa_int_ct rhs_t>                 \
    constexpr auto operator_name (lhs_t lhs, rhs_t rhs) {               \
        using namespace impl;                                           \
        safe_bitwise_operation_conversion_assert<lhs_t, rhs_t>();       \
        using result_type = corresponding_tifa_int_t<                   \
            typename bitwise_operation_conversion<lhs_t, rhs_t>::type>; \
        return static_cast<result_type>(lhs operand rhs.value);         \
    }                                                                   \
                                                                        \
    template<tifa_int_ct lhs_t, primitive_int_ct rhs_t>                 \
    constexpr auto operator_name (lhs_t lhs, rhs_t rhs) {               \
        using namespace impl;                                           \
        safe_bitwise_operation_conversion_assert<lhs_t, rhs_t>();       \
        using result_type = corresponding_tifa_int_t<                   \
            typename bitwise_operation_conversion<lhs_t, rhs_t>::type>; \
        return static_cast<result_type>(lhs.value operand rhs);         \
    }

namespace tifa {
__TIFA_IMPL_DEFINE_BITWISE_OPERATOR(operator^, ^)
__TIFA_IMPL_DEFINE_BITWISE_OPERATOR(operator&, &)
__TIFA_IMPL_DEFINE_BITWISE_OPERATOR(operator|, |)
}

#define __TIFA_IMPL_DEFINE_BITWISE_ASSIGNMENT_OPERATOR(operator_name, operand) \
    template<primitive_int_ct lhs_t, tifa_int_ct rhs_t>                        \
    constexpr auto& operator_name (lhs_t& lhs, rhs_t rhs) {                    \
        using namespace impl;                                                  \
        safe_bitwise_operation_conversion_assert<lhs_t, rhs_t>();              \
        return lhs operand rhs.value;                                          \
    }                                                                          \
                                                                               \
    template<tifa_int_ct lhs_t, primitive_int_ct rhs_t>                        \
    constexpr auto& operator_name (lhs_t& lhs, rhs_t rhs) {                    \
        using namespace impl;                                                  \
        safe_bitwise_operation_conversion_assert<lhs_t, rhs_t>();              \
        lhs.value operand rhs;                                                 \
        return lhs;                                                            \
    }                                                                          \
                                                                               \
    template<tifa_int_ct lhs_t, tifa_int_ct rhs_t>                             \
    constexpr auto& operator_name (lhs_t& lhs, rhs_t rhs) {                    \
        using namespace impl;                                                  \
        safe_bitwise_operation_conversion_assert<lhs_t, rhs_t>();              \
        lhs.value operand rhs.value;                                           \
        return lhs;                                                            \
    }

namespace tifa {
__TIFA_IMPL_DEFINE_BITWISE_ASSIGNMENT_OPERATOR(operator|=, |=)
__TIFA_IMPL_DEFINE_BITWISE_ASSIGNMENT_OPERATOR(operator&=, &=)
__TIFA_IMPL_DEFINE_BITWISE_ASSIGNMENT_OPERATOR(operator^=, ^=)
}

#define __TIFA_IMPL_DEFINE_ARITHMETIC_ASSIGNMENT_OPERATOR(operator_name, operand) \
    template<tifa_int_ct lhs_t, primitive_int_ct rhs_t>                           \
    constexpr auto& operator_name (lhs_t &lhs, rhs_t rhs) {                       \
        using namespace impl;                                                     \
        safe_assignment_operation_conversion_assert<rhs_t, lhs_t>();              \
        lhs.value operand rhs;                                                    \
        return lhs;                                                               \
    }                                                                             \
                                                                                  \
    template<primitive_int_ct lhs_t, tifa_int_ct rhs_t>                           \
    constexpr auto operator_name (lhs_t &lhs, rhs_t rhs) {                        \
        using namespace impl;                                                     \
        safe_assignment_operation_conversion_assert<rhs_t, lhs_t>();              \
        return lhs operand rhs.value;                                             \
    }                                                                             \
                                                                                  \
    template<tifa_int_ct lhs_t, tifa_int_ct rhs_t>                                \
    constexpr auto operator_name (lhs_t &lhs, rhs_t rhs) {                        \
        using namespace impl;                                                     \
        safe_assignment_operation_conversion_assert<rhs_t, lhs_t>();              \
        lhs.value operand rhs.value;                                              \
        return lhs;                                                               \
    }


namespace tifa {
__TIFA_IMPL_DEFINE_ARITHMETIC_ASSIGNMENT_OPERATOR(operator+=, +=)
__TIFA_IMPL_DEFINE_ARITHMETIC_ASSIGNMENT_OPERATOR(operator-=, -=)
__TIFA_IMPL_DEFINE_ARITHMETIC_ASSIGNMENT_OPERATOR(operator*=, *=)
__TIFA_IMPL_DEFINE_ARITHMETIC_ASSIGNMENT_OPERATOR(operator/=, /=)
__TIFA_IMPL_DEFINE_ARITHMETIC_ASSIGNMENT_OPERATOR(operator%=, %=)
}

#undef __TIFA_IMPL_DEFINE_LOGIC_OPERATOR
#undef __TIFA_IMPL_DEFINE_ARITHMETIC_OPERATOR
#undef __TIFA_IMPL_DEFINE_BITWISE_OPERATOR
#undef __TIFA_IMPL_DEFINE_BITWISE_ASSIGNMENT_OPERATOR
#undef __TIFA_IMPL_DEFINE_ARITHMETIC_ASSIGNMENT_OPERATOR

namespace tifa {
    template<int_ct T>
    struct limit {
        static constexpr T min = impl::limit<T>::min;
        static constexpr T max = impl::limit<T>::max;
    };
}

namespace tifa {
    template <char... number>
    consteval auto operator""_() {
        return impl::parse_literal<number...>();
    }
};

#define __TIFA_IMPL_CREATE_LITERAL_OPERATOR(type)                                          \
    template <char... number>                                                              \
    consteval type operator""_ ## type () {                                                \
        auto constexpr literal = impl::parse_literal<number...>();                         \
        static_assert(impl::is_below_type_upper_limit_ct<literal.get(), type>,             \
                      "Number in a literal cannot be represented by a specified type.");   \
                                                                                           \
        return static_cast<type>(literal.get());                                           \
    }

namespace tifa {
__TIFA_IMPL_CREATE_LITERAL_OPERATOR(u8)
__TIFA_IMPL_CREATE_LITERAL_OPERATOR(u16)
__TIFA_IMPL_CREATE_LITERAL_OPERATOR(u32)
__TIFA_IMPL_CREATE_LITERAL_OPERATOR(u64)
__TIFA_IMPL_CREATE_LITERAL_OPERATOR(s8)
__TIFA_IMPL_CREATE_LITERAL_OPERATOR(s16)
__TIFA_IMPL_CREATE_LITERAL_OPERATOR(s32)
__TIFA_IMPL_CREATE_LITERAL_OPERATOR(s64)
}

#undef __TIFA_IMPL_CREATE_LITERAL_OPERATOR

namespace tifa {
    template<int_ct T>
    constexpr T compliment(T int_object) {
        return T(-int_object.value);
    }

    template<int_ct dst_t>
    constexpr dst_t zeroext(auto int_object) {
        using namespace impl;
        using src_t = decltype(int_object);
        safe_extend_operation_assert<src_t, dst_t>();
        return static_cast<dst_t>(static_cast<impl::unsigned_equivalent_t<src_t>>(int_object));
    }

    template<int_ct dst_t>
    constexpr dst_t signext(auto int_object) {
        using namespace impl;
        using src_t = decltype(int_object);
        safe_extend_operation_assert<src_t, dst_t>();
        return static_cast<dst_t>(static_cast<impl::signed_equivalent_t<src_t>>(int_object));
    }
}
