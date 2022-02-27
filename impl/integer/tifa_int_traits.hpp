#pragma once
#include <impl/integer/primitive_int_traits.hpp>

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
    struct get_underlying_primitive_type_helper;

    template<primitive_int_ct T>
    struct get_underlying_primitive_type_helper<T> {
        using type = T;
    };

    template<tifa_int_ct T>
    struct get_underlying_primitive_type_helper<T> {
        using type = typename T::underlying_t;
    };

    template<int_ct T>
    using underlying_primitive_t =
        typename get_underlying_primitive_type_helper<dequalified_t<T>>::type;
}

namespace tifa::impl {
    template<typename T>
    struct wrap_in_tifa_type_helper {
        using type = void;
    };

    template<primitive_int_ct T>
    struct wrap_in_tifa_type_helper<T> {
        using type = integer<T>;
    };

    template<tifa_int_ct T>
    struct wrap_in_tifa_type_helper<T> {
        using type = T;
    };

    template<typename T>
    using corresponding_tifa_int_t = typename wrap_in_tifa_type_helper<dequalified_t<T>>::type;
}
