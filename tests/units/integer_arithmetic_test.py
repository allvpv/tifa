import os
from itertools import product
from .integer_test_helper import *
from .helper import StaticTestsGenerator

description = "integer<> arithmetic operators conversion test"
operators_arithmetic = ["+", "-", "*", "/", "%"]

def do_arithm_op(v1, v2, operator, restype):
    sumval = None

    match operator:
        case '+':
            sumval = v1.val + v2.val
        case '-':
            sumval = v1.val - v2.val
        case '/':
            sumval = abs(v1.val) // abs(v2.val)
            if (v1.val < 0 or v2.val < 0) and not (v1.val < 0 and v2.val < 0):
                sumval *= -1
        case '*':
            sumval = v1.val * v2.val
        case '%':
            sumval = abs(v1.val) % abs(v2.val) * (-1 if v1.val < 0 else 1)

    return Value(restype, sumval)

def arithop_type(type1, type2):
    if type1.is_signed == type2.is_signed:
        if type1.bits >= type2.bits:
            return type1
        else:
            return type2
    elif type1.is_signed and not type2.is_signed:
        if type1.bits > type2.bits:
            return type1
    elif not type1.is_signed and type2.is_signed:
        if type2.bits > type1.bits:
            return type2

    return None

def register():
    generator = StaticTestsGenerator("INTEGER_ARITHMETIC_TEST")

    for operator in operators_arithmetic:
        for (type1, type2) in product(types, types):
            v1 = Value(type1)
            v2 = Value(type2)

            v1t = declare_tifa("v1", type1, v1.val)
            v2t = declare_tifa("v2", type2, v2.val)
            v1p = declare_primitive("v1", type1, v1.val)
            v2p = declare_primitive("v2", type2, v2.val)

            restype = arithop_type(type1, type2)

            if restype == None:
                append_wrap_test(generator, v1t + v2t + operators_expr_bad("v1t", "v2t", operator), False)
                append_wrap_test(generator, v1p + v2t + operators_expr_bad("v1p", "v2t", operator), False)
                append_wrap_test(generator, v1t + v2p + operators_expr_bad("v1t", "v2p", operator), False)
            else:
                restype_str = restype.tifa_t_str()
                result = do_arithm_op(v1, v2, operator, restype)
                append_wrap_test(generator, v1t + v2t + operators_expr_good("v1t", "v2t", operator, restype_str, result.get_string()), True)
                append_wrap_test(generator, v1t + v2p + operators_expr_good("v1t", "v2p", operator, restype_str, result.get_string()), True)
                append_wrap_test(generator, v1p + v2t + operators_expr_good("v1p", "v2t", operator, restype_str, result.get_string()), True)

    preamble = """#include <stddef.h>
#include <integer.hpp>
#include <type_traits.hpp>
using namespace tifa;

"""
    path=".generated/integer_bitwise_test.cpp"

    generator.write_to_file(preamble, path)
    generator.add_to_tester(description)
