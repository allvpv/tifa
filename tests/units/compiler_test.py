from . import helper

description = "simple C++20 program"

def execute():
    return helper.Tester.dry_compile("units/src/compiler_test.cpp")

def register():
    helper.Tester.addTest(description, execute)

