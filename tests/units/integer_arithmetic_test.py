import os
from itertools import product
from .integer_test_helper import *
from .helper import StaticTestsGenerator

description = "integer<> arithmetic operators conversion test"

def declare_tifa(name, type_obj, value, is_constexpr = True, maybe_unused = False):
    return declare_variable(type_obj.tifa_t_str(), name + "t", value, is_constexpr, maybe_unused)

def declare_primitive(name, type_obj, value, is_constexpr = True, maybe_unused = False):
    return declare_variable(type_obj.primitive_t_str(), name + "p", value, is_constexpr, maybe_unused)

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

def append_test(generator, expression, should_compile):
    preamble = "void test() {\n"
    epilogue = "\n}"

    generator.append_test(preamble + expression + epilogue, should_compile)

def testing_expr_good(var1, var2, operator, expected_type, expected_value):
    expression = (var1 + " " + operator + " " + var2)
    is_type_ok = "static_assert(is_same_ct<decltype(" + expression + "), " + expected_type + ">);\n"
    is_value_ok = "static_assert((" + expression + ") == " + expected_value + ");"
    return is_type_ok + is_value_ok

def testing_expr_bad(var1, var2, operator):
    return "(void) (" + var1 + " " + operator + " " + var2 + ");"

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
                append_test(generator, v1t + v2t + testing_expr_bad("v1t", "v2t", operator), False)
                append_test(generator, v1p + v2t + testing_expr_bad("v1p", "v2t", operator), False)
                append_test(generator, v1t + v2p + testing_expr_bad("v1t", "v2p", operator), False)
            else:
                restype_str = restype.tifa_t_str()
                result = do_arithm_op(v1, v2, operator, restype)
                append_test(generator, v1t + v2t + testing_expr_good("v1t", "v2t", operator, restype_str, result.get_string()), True)
                append_test(generator, v1t + v2p + testing_expr_good("v1t", "v2p", operator, restype_str, result.get_string()), True)
                append_test(generator, v1p + v2t + testing_expr_good("v1p", "v2t", operator, restype_str, result.get_string()), True)

    preamble = """#include <stddef.h>
#include <integer.hpp>
#include <type_traits.hpp>
using namespace tifa;

"""
    path=".generated/integer_arithmetic_test.cpp"

    generator.write_to_file(preamble, path)
    generator.add_to_tester(description)
