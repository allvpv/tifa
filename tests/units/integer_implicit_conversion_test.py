import os
from itertools import product
from .helper import StaticTestsGenerator
from .integer_test_helper import types, declare_variable

description = "integer<> implicit conversions test"

def assign_expr(var1, var2):
    return var1 + " = " + var2 + ";\n"

def create_tests_for_types_conversion(generator, tp1, tp2, idx):
    preamble = "void test() {\n"
    epilogue = "}"

    varsuffix = "var" + str(idx)

    t1_decl = declare_variable(tp1.tifa_t_str(), "t1" + varsuffix, 0, False, True)
    t2_decl = declare_variable(tp2.tifa_t_str(), "t2" + varsuffix, 0, False, True)

    p1_decl = declare_variable(tp1.primitive_t_str(), "p1" + varsuffix, 0, False, True)
    p2_decl = declare_variable(tp2.primitive_t_str(), "p2" + varsuffix, 0, False, True)

    should_compile = tp2.is_convertible_to(tp1)

    generator.append_test(preamble + t1_decl + t2_decl + assign_expr("t1" + varsuffix, "t2" + varsuffix) + epilogue, should_compile)
    generator.append_test(preamble + t1_decl + p2_decl + assign_expr("t1" + varsuffix, "p2" + varsuffix) + epilogue, should_compile)
    generator.append_test(preamble + p1_decl + t2_decl + assign_expr("p1" + varsuffix, "t2" + varsuffix) + epilogue, should_compile)
    generator.append_test(preamble + p1_decl + p2_decl + assign_expr("p1" + varsuffix, "p2" + varsuffix) + epilogue, True)

def register():
    generator = StaticTestsGenerator("INTEGER_IMPLICIT_CONVERSION_TEST")
    idx = 0

    for (t1, t2) in product(types, types):
        create_tests_for_types_conversion(generator, t1, t2, idx)
        idx += 1

    preamble = """#include <stddef.h>
#include <integer.hpp>
using namespace tifa;

"""
    path=".generated/integer_implicit_conversion_test.cpp"

    generator.write_to_file(preamble, path)
    generator.add_to_tester(description)

