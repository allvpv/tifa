#pragma once
#include <stdint.h>
#include <impl/integer/primitive_int_traits.hpp>
#include <impl/integer/tifa_int_traits.hpp>
#include <impl/integer/int_info.hpp>
#include <impl/integer/safe_conversion.hpp>

namespace tifa::impl {
    using u8 = integer<uint8_t>;
    using s8 = integer<int8_t>;
    using u16 = integer<uint16_t>;
    using s16 = integer<int16_t>;
    using u32 = integer<uint32_t>;
    using s32 = integer<int32_t>;
    using u64 = integer<uint64_t>;
    using s64 = integer<int64_t>;
}

namespace tifa::impl {
    /* Prevent creating tifa integer when underlying type has qualifiers. */
    template<typename T>
    concept is_dequalified =
        is_same_ct<T, dequalified_t<T>>;

    template<primitive_int_ct I>
    requires is_dequalified<I>
    struct integer<I> {
        using underlying_t = I;
        underlying_t value;

        // clang does not support this (recursion in literal type definition) (-_-)
        //
        // static constexpr integer min = limit<integer>::min;
        // static constexpr integer max = limit<integer>::max;

        static consteval integer min() { return limit<integer>::min; }
        static consteval integer max() { return limit<integer>::max; }

        constexpr integer() { value = 0; }

        /* Implicit constructors */
        template<int_ct src_t>
        requires safe_conversion_from_to_ct<src_t, integer>
        constexpr integer(src_t src) {
            value = get_primitive_value(src);
        }

        /* Implicit conversion operator */
        template<primitive_int_ct dst_t>
        requires safe_conversion_from_to_ct<integer, dst_t>
        constexpr operator dst_t() const {
            return value;
        }


        /* Explicit constructors */
        template<int_ct src_t>
        constexpr explicit integer(src_t src) {
            value = static_cast<underlying_t>(get_primitive_value(src));
        }

        /* Explicit conversion operator */
        template<typename dst_t>
        constexpr explicit operator dst_t() const {
            return static_cast<dst_t>(value);
        }
    };
}

