#pragma once

namespace tifa::impl {
    template<bool v>
    struct constraint {
        static constexpr bool test = v;
        using type = constraint;
    };
}

namespace tifa::impl {
    template<class T, class U>
    struct is_same : constraint<false> {};

    template<class T>
    struct is_same<T, T> : constraint<true> {};

    template<class T, class U>
    concept same_partial_ct = is_same<T, U>::test;
}

namespace tifa::impl {
    /* https://stackoverflow.com/a/58511321 */
    template<class T, class U>
    concept is_same_ct = same_partial_ct<T, U> && same_partial_ct<U, T>;
}

namespace tifa::impl {
    template<class T>
    struct dequalify {
        using type = T;
    };

    template<class T>
    struct dequalify<const T> {
        using type = T;
    };

    template<class T>
    struct dequalify<volatile T> {
        using type = T;
    };

    template<class T>
    struct dequalify<const volatile T> {
        using type = T;
    };
}

namespace tifa::impl {
    template<class T>
    using dequalified_t = typename dequalify<T>::type;
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
