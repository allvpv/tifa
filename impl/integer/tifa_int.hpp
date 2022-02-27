#pragma once
#include <stdint.h>
#include <impl/integer/primitive_int_traits.hpp>
#include <impl/integer/tifa_int_traits.hpp>
#include <impl/integer/int_info.hpp>
#include <impl/integer/safe_conversion.hpp>

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
