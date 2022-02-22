#pragma once 
#include "integer.hpp"

/* We have to write literal parser to get the value of literal as a constant
 * expression (-_-) C++ never fails to surprise me
 */

namespace tifa::impl {
    /* Universal literal (operator""_ literal) is not yet fully implemented. */
    template <unsigned long long val, bool negative = false>
    struct integer_literal {
        consteval bool is_negative() const { return negative; }
        consteval unsigned long long get() const { return val; }

        consteval auto operator-() const {
            return integer_literal<val, negative^1>();
        }
    };

    template<int base>
    consteval int parse_digit(char digit);

    template<>
    consteval int parse_digit<16>(char digit) {
        switch (digit) {
            case '0': return 0;
            case '1': return 1;
            case '2': return 2;
            case '3': return 3;
            case '4': return 4;
            case '5': return 5;
            case '6': return 6;
            case '7': return 7;
            case '8': return 8;
            case '9': return 9;
            case 'A': return 10;
            case 'B': return 11;
            case 'C': return 12;
            case 'D': return 13;
            case 'E': return 14;
            case 'F': return 15;
            default:  return -1;
        }
    }

    template<>
    consteval int parse_digit<10>(char digit) {
        switch (digit) {
            case '0': return 0;
            case '1': return 1;
            case '2': return 2;
            case '3': return 3;
            case '4': return 4;
            case '5': return 5;
            case '6': return 6;
            case '7': return 7;
            case '8': return 8;
            case '9': return 9;
            default:  return -1;
        }
    }

    template<>
    consteval int parse_digit<8>(char digit) {
        switch (digit) {
            case '0': return 0;
            case '1': return 1;
            case '2': return 2;
            case '3': return 3;
            case '4': return 4;
            case '5': return 5;
            case '6': return 6;
            case '7': return 7;
            default:  return -1;
        }
    }

    template<>
    consteval int parse_digit<2>(char digit) {
        switch (digit) {
            case '0': return 0;
            case '1': return 1;
            default:  return -1;
        }
    }

    template <unsigned long long val, int base>
    consteval auto unpack_and_parse() {
        return integer_literal<val>();
    }

    // declaration is required... 
    template <unsigned long long val, int base, char digit, char... number>
    requires (digit != '\'')
    consteval auto unpack_and_parse();


    template <unsigned long long val, int base, char digit, char... number>
    requires (digit == '\'') // digit separator, passing 
    consteval auto unpack_and_parse() {
        return unpack_and_parse<val, base, number...>(); // ... ^ calls function declared above
    }

    template <unsigned long long val, int base, char digit, char... number>
    requires (digit != '\'')
    consteval auto unpack_and_parse() {
        constexpr int parsed_digit = parse_digit<base>(digit);

        static_assert(parsed_digit != -1,
                "Wrong symbol in a literal. This should not happen. "
                "Fill out bug request at: https://.../");

        return unpack_and_parse<val * base + parsed_digit, base, number...>();
    }

    namespace hex_literal {
        template<char> consteval bool check() { return false; }

        template<char _0, char _x, char... tail>
        consteval bool check() {
            return sizeof...(tail) != 0 && _0 == '0' && (_x == 'x' || _x == 'X');
        }

        template<char _0, char _x, char... tail>
        consteval auto parse() {
            return unpack_and_parse<0, 16, tail...>();
        }
    }

    namespace bin_literal {
        template<char> consteval bool check() { return false; }

        template<char _0, char _b, char... tail>
        consteval bool check() {
            return sizeof...(tail) != 0 && _0 == '0' && (_b == 'b' || _b == 'B');
        }

        template<char _0, char _b, char... tail>
        consteval auto parse() {
            return unpack_and_parse<0, 2, tail...>();
        }
    }

    namespace oct_literal {
        template<char>
        consteval bool check() { return false; }

        template<char _0, char digit, char... tail>
        consteval bool check() {
            return _0 == '0' && parse_digit<8>(digit) != -1;
        }

        template<char _0, char... tail>
        consteval auto parse() {
            return unpack_and_parse<0, 8, tail...>();
        }
    }

    namespace dec_literal {
        template<char... tail>
        consteval auto parse() {
            return unpack_and_parse<0, 10, tail...>();
        }
    }

    template<char... number>
    concept is_hex_literal = hex_literal::check<number...>();

    template<char... number>
    concept is_oct_literal = oct_literal::check<number...>();

    template<char... number>
    concept is_bin_literal = bin_literal::check<number...>();
};

namespace tifa::impl {
    template <char... number>
    requires is_hex_literal<number...> 
    consteval auto operator""_() {
        static_assert(!is_bin_literal<number...>, "This should not happen.");
        static_assert(!is_oct_literal<number...>, "This should not happen.");
        return hex_literal::parse<number...>();
    }

    template <char... number>
    requires is_oct_literal<number...> 
    consteval auto parse_literal() {
        static_assert(!is_bin_literal<number...>, "This should not happen.");
        static_assert(!is_hex_literal<number...>, "This should not happen.");
        return oct_literal::parse<number...>();
    }

    template <char... number>
    requires is_bin_literal<number...> 
    consteval auto parse_literal() {
        static_assert(!is_oct_literal<number...>, "This should not happen.");
        static_assert(!is_hex_literal<number...>, "This should not happen.");
        return bin_literal::parse<number...>();
    }

    template <char... number>
    consteval auto parse_literal() {
        static_assert(!is_oct_literal<number...>, "This should not happen.");
        static_assert(!is_hex_literal<number...>, "This should not happen.");
        static_assert(!is_bin_literal<number...>, "This should not happen.");
        return dec_literal::parse<number...>();
    }

    template <unsigned long long value, typename T>
    concept is_below_type_upper_limit_ct = 
        int_ct<T> &&
        value <= u64(limit<T>::max).value;
};

