import os
from itertools import product
from .integer_test_helper import *
from .helper import StaticTestsGenerator

description = "integer<> bitwise operators conversion test"
operators_bitwise = ["^", "&", "|"]

def do_binop(v1, v2, operator, restype):
    sumval = None

    match operator:
        case '^':
            sumval = v1.val ^ v2.val
        case '|':
            sumval = v1.val | v2.val
        case '&':
            sumval = v1.val & v2.val

    return Value(restype, sumval)

def binop_type(type1, type2):
    if type1.bits == type2.bits:
        if not type2.is_signed:
            return type2
        else:
            return type1

    return None

def register():
    generator = StaticTestsGenerator("INTEGER_BITWISE_TEST")

    for operator in operators_bitwise:
        for (type1, type2) in product(types, types):
            v1 = Value(type1)
            v2 = Value(type2)

            v1t = declare_tifa("v1", type1, v1.val)
            v2t = declare_tifa("v2", type2, v2.val)
            v1p = declare_primitive("v1", type1, v1.val)
            v2p = declare_primitive("v2", type2, v2.val)

            restype = binop_type(type1, type2)

            if restype == None:
                append_wrap_test(generator, v1t + v2t + operators_expr_bad("v1t", "v2t", operator), False)
                append_wrap_test(generator, v1p + v2t + operators_expr_bad("v1p", "v2t", operator), False)
                append_wrap_test(generator, v1t + v2p + operators_expr_bad("v1t", "v2p", operator), False)
            else:
                restype_str = restype.tifa_t_str()
                result = do_binop(v1, v2, operator, restype)
                append_wrap_test(generator, v1t + v2t + operators_expr_good("v1t", "v2t", operator, restype_str, result.get_string()), True)
                append_wrap_test(generator, v1t + v2p + operators_expr_good("v1t", "v2p", operator, restype_str, result.get_string()), True)
                append_wrap_test(generator, v1p + v2t + operators_expr_good("v1p", "v2t", operator, restype_str, result.get_string()), True)

    preamble = """#include <stddef.h>
#include <integer.hpp>
#include <type_traits.hpp>
using namespace tifa;

"""
    path=".generated/integer_arithmetic_test.cpp"

    generator.write_to_file(preamble, path)
    generator.add_to_tester(description)
