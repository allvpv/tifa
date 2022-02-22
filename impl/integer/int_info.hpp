#pragma once
#include <limits.h>
#include <impl/integer/primitive_int_traits.hpp>
#include <impl/integer/tifa_int_traits.hpp>

namespace tifa::impl {
    template<typename T>
    struct integer_info_helper;

    template<>
    struct integer_info_helper<signed char> {
        static constexpr signed char min = SCHAR_MIN;
        static constexpr signed char max = SCHAR_MAX;
    };

    template<>
    struct integer_info_helper<short> {
        static constexpr short min = SHRT_MIN;
        static constexpr short max = SHRT_MAX;
    };

    template<>
    struct integer_info_helper<int> {
        static constexpr int min = INT_MIN;
        static constexpr int max = INT_MAX;
    };

    template<>
    struct integer_info_helper<long> {
        static constexpr long min = LONG_MIN;
        static constexpr long max = LONG_MAX;
    };

    template<>
    struct integer_info_helper<long long> {
        static constexpr long long min = LLONG_MIN;
        static constexpr long long max = LLONG_MAX;
    };

    template<>
    struct integer_info_helper<unsigned char> {
        static constexpr unsigned char min = 0;
        static constexpr unsigned char max = UCHAR_MAX;
    };

    template<>
    struct integer_info_helper<unsigned short> {
        static constexpr unsigned short min = 0;
        static constexpr unsigned short max = USHRT_MAX;
    };

    template<>
    struct integer_info_helper<unsigned int> {
        static constexpr unsigned int min = 0;
        static constexpr unsigned int max = UINT_MAX;
    };

    template<>
    struct integer_info_helper<unsigned long> {
        static constexpr unsigned long min = 0;
        static constexpr unsigned long max = ULONG_MAX;
    };

    template<>
    struct integer_info_helper<unsigned long long> {
        static constexpr long long min = 0;
        static constexpr long long max = ULLONG_MAX;
    };

    template<>
    struct integer_info_helper<char> {
        static constexpr char min = CHAR_MIN;
        static constexpr char max = CHAR_MAX;
    };

    template<tifa_int_ct T>
    struct integer_info_helper<T> {
        static constexpr T min = integer_info_helper<typename T::underlying_t>::min;
        static constexpr T max = integer_info_helper<typename T::underlying_t>::max;
    };
};

namespace tifa::impl {
    template<int_ct T>
    struct limit {
        static constexpr T min = integer_info_helper<dequalified_t<T>>::min;
        static constexpr T max = integer_info_helper<dequalified_t<T>>::max;
    };
};
