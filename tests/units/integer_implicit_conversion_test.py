import os
from itertools import product
from . import helper

description = "integer<> implicit conversions test"

class IntType:
    bits = None
    is_signed = None

    def __init__(self, bits, is_signed):
        self.bits = bits
        self.is_signed = is_signed

    def is_convertible_to(self, another):
        if self.bits <= another.bits and self.is_signed == another.is_signed:
            return True
        if self.bits < another.bits and (not self.is_signed) and another.is_signed:
            return True
        else:
            return False

    def tifa_t_str(self):
        return ("s" if self.is_signed else "u") + str(self.bits)

    def primitive_t_str(self):
        return ("" if self.is_signed else "u") + "int" + str(self.bits) + "_t"

def declare_var(typename, varname, value, is_constexpr=False):
    return ("constexpr " if is_constexpr else "") + typename + " " + varname + \
            " = " + typename + "(" + str(value) + ");\n"

def assign_expr(var1, var2):
    return var1 + " = " + var2 + ";\n"

def create_tests_for_types_conversion(generator, type1, type2, idx):
    preamble = "void test() {\n"
    epilogue = "}"

    varsuffix = "var" + str(idx)

    t1_decl = declare_var(type1.tifa_t_str(), "t1" + varsuffix, 0)
    t2_decl = declare_var(type2.tifa_t_str(), "t2" + varsuffix, 0)

    p1_decl = declare_var(type1.primitive_t_str(), "p1" + varsuffix, 0)
    p2_decl = declare_var(type2.primitive_t_str(), "p2" + varsuffix, 0)

    should_compile = type2.is_convertible_to(type1)

    generator.append_test(preamble + t1_decl + t2_decl + assign_expr("t1" + varsuffix, "t2" + varsuffix) + epilogue, should_compile)
    generator.append_test(preamble + t1_decl + p2_decl + assign_expr("t1" + varsuffix, "p2" + varsuffix) + epilogue, should_compile)
    generator.append_test(preamble + p1_decl + t2_decl + assign_expr("p1" + varsuffix, "t2" + varsuffix) + epilogue, should_compile)
    generator.append_test(preamble + p1_decl + p2_decl + assign_expr("p1" + varsuffix, "p2" + varsuffix) + epilogue, True)


def register():
    types = []

    for (is_signed, bits) in product([True, False], [8, 16, 32, 64]):
        types.append(IntType(bits, is_signed))

    generator = helper.StaticTestsGenerator("INTEGER_IMPLICIT_CONVERSION_TEST")
    idx = 0

    for (t1, t2) in product(types, types):
        create_tests_for_types_conversion(generator, t1, t2, idx)
        idx += 1

    preamble = """#include <stddef.h>
#include <integer.hpp>
using namespace tifa::impl;

"""
    path=".generated/integer_implicit_conversion_test.cpp"

    generator.write_to_file(preamble, path)
    generator.add_to_tester(description)

