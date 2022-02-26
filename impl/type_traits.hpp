#pragma once

namespace tifa::impl {
    template<bool v>
    struct constraint {
        static constexpr bool test = v;
        using type = constraint;
    };
}

namespace tifa::impl {
    template<typename T, typename U>
    struct is_same : constraint<false> {};

    template<typename T>
    struct is_same<T, T> : constraint<true> {};

    template<typename T, typename U>
    concept same_partial_ct = is_same<T, U>::test;
}

namespace tifa::impl {
    /* https://stackoverflow.com/a/58511321 */
    template<typename T, typename U>
    concept is_same_ct = same_partial_ct<T, U> && same_partial_ct<U, T>;
}

namespace tifa::impl {
    template<typename T>
    struct dequalify {
        using type = T;
    };

    template<typename T>
    struct dequalify<const T> {
        using type = T;
    };

    template<typename T>
    struct dequalify<volatile T> {
        using type = T;
    };

    template<typename T>
    struct dequalify<const volatile T> {
        using type = T;
    };
}

namespace tifa::impl {
    template<typename T>
    using dequalified_t = typename dequalify<T>::type;
}

namespace tifa::impl {
    template<typename T>
    struct detect_qualifiers {
        static constexpr bool is_const = false;
        static constexpr bool is_volatile = false;
    };

    template<typename T>
    struct detect_qualifiers<const T> {
        static constexpr bool is_const = true;
        static constexpr bool is_volatile = false;
    };

    template<typename T>
    struct detect_qualifiers<volatile T> {
        static constexpr bool is_const = false;
        static constexpr bool is_volatile = true;
    };

    template<typename T>
    struct detect_qualifiers<const volatile T> {
        static constexpr bool is_const = true;
        static constexpr bool is_volatile = true;
    };
}

namespace tifa::impl {
    template<typename T, bool is_const, bool is_volatile>
    struct qualify {
        using type = dequalified_t<T>;
    };

    template<typename T>
    struct qualify<T, true, false>  {
        using type = const dequalified_t<T>;
    };

    template<typename T>
    struct qualify<T, false, true>  {
        using type = volatile dequalified_t<T>;
    };

    template<typename T>
    struct qualify<T, true, true>  {
        using type = const volatile dequalified_t<T>;
    };
}

namespace tifa::impl {
    template<typename T, bool is_const, bool is_volatile>
    using qualified_t = typename qualify<T, is_const, is_volatile>::type;
}

namespace tifa::impl {
    template<typename from_t, typename to_t>
    struct copy_qualifiers {
        using qualified_dst_t = qualified_t<
            to_t,
            detect_qualifiers<from_t>::is_const,
            detect_qualifiers<from_t>::is_volatile
        >;
    };
}

namespace tifa::impl {
    template<typename src_t, typename dst_t>
    using copy_qualifiers_t = typename copy_qualifiers<src_t, dst_t>::qualified_dst_t;
}

namespace tifa::impl {
    template<typename T, typename U>
    concept smaller_or_equal_size_ct = sizeof(T) <= sizeof(U);

    template<typename T, typename U>
    concept greater_or_equal_size_ct = sizeof(T) >= sizeof(U);

    template<typename T, typename U>
    concept smaller_size_ct = sizeof(T) < sizeof(U);

    template<typename T, typename U>
    concept greater_size_ct = sizeof(T) > sizeof(U);
}
