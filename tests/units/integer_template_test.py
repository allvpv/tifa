import os
from . import helper

description = "integer<> template test"

def expression(tp, variable_name):
    return "inline constexpr integer<" + tp + "> " + variable_name + " = (" + tp + ") 0;"

def register():
    def qualify_types(types):
        qualified = []
        for qualifier in ["const ", "volatile ", "const volatile "]:
            qualified += [qualifier + t for t in types]

        return qualified

    def pointerize_types(types):
        return [t + "*" for t in types]

    primitive_signed = ["short", "int", "long", "long long"]
    primitive_unsigned = ["unsigned " + s for s in primitive_signed]

    primitive_signed.append("signed char")
    primitive_unsigned.append("unsigned char")

    primitive_another = ["char"]

    aliased_signed = ["int" + str(bits) + "_t" for bits in [8, 16, 32, 64]]
    aliased_unsigned = ["u" + s for s in aliased_signed]
        
    supported_signed = primitive_signed + aliased_signed
    supported_unsigned = primitive_unsigned + aliased_unsigned
    supported = supported_signed + supported_unsigned + primitive_another

    unsupported = ["nullptr_t", "void", "bool", "wchar_t", "char8_t",   \
            "char16_t", "char32_t", "double", "float", "long double"] + \
            qualify_types(supported) +  \
            pointerize_types(supported)

    varnum = 0
    expressions_good = []
    expressions_bad = []

    for good_type in supported:
        expressions_good.append(expression(good_type, "var" + str(varnum)))
        varnum += 1

    for bad_type in unsupported:
        expressions_bad.append(expression(bad_type, "var" + str(varnum)))
        varnum += 1

    generator = helper.StaticTestsGenerator("INTEGER_TEMPLATE_TEST")
    generator.append_test('\n'.join(expressions_good), True)

    for bad in expressions_bad:
        generator.append_test(bad, False)

    preamble = """#include <stddef.h>
#include <integer.hpp>
using namespace tifa::impl;

"""
    path=".generated/integer_template_test.cpp"

    generator.write_to_file(preamble, path)
    generator.add_to_tester(description)
