import os
from itertools import product
from .integer_test_helper import *
from .helper import StaticTestsGenerator

description = "integer<> logic operators conversion test"
operators_logic = ["==", "!=", "<", ">", "<=", ">="]

def do_logop(v1, v2, operator):
    match operator:
        case '==':
            return (v1.val == v2.val)
        case '!=':
            return (v1.val != v2.val)
        case '<':
            return (v1.val < v2.val)
        case '>':
            return (v1.val > v2.val)
        case '<=':
            return (v1.val <= v2.val)
        case '>=':
            return (v1.val >= v2.val)

def logop_type(type1, type2):
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

def logic_expr(var1, var2, operator, expected_value):
    expected_str = "true" if expected_value else "false"
    expression = var1 + " " + operator + " " + var2

    return "static_assert((" + expression + ") == " + expected_str + ");"

def register():
    generator = StaticTestsGenerator("INTEGER_LOGIC_TEST")

    for operator in operators_logic:
        for (type1, type2) in product(types, types):
            v1 = Value(type1)
            v2 = Value(type2)

            v1t = declare_tifa("v1", type1, v1.val)
            v2t = declare_tifa("v2", type2, v2.val)
            v1p = declare_primitive("v1", type1, v1.val)
            v2p = declare_primitive("v2", type2, v2.val)

            should_compile = (logop_type(type1, type2) != None)
            expected_val = do_logop(v1, v2, operator)

            append_wrap_test(generator, v1t + v2t + logic_expr("v1t", "v2t", operator, expected_val), should_compile)
            append_wrap_test(generator, v1t + v2p + logic_expr("v1t", "v2p", operator, expected_val), should_compile)
            append_wrap_test(generator, v1p + v2t + logic_expr("v1p", "v2t", operator, expected_val), should_compile)


    preamble = """#include <stddef.h>
#include <integer.hpp>
#include <type_traits.hpp>
using namespace tifa;

"""
    path=".generated/integer_logic_test.cpp"

    generator.write_to_file(preamble, path)
    generator.add_to_tester(description)
