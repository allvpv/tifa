#!/usr/bin/env python3

# Forgive me this code, I'm not a pythoner

import argparse
import os
import pathlib

import units.helper
import units.compiler_test as compiler_test
import units.integer_template_test as integer_template_test

def get_arguments():
    default_flags = "-Wall -Wextra --std=c++20 -nodefaultlibs -fno-rtti -lc -I.."
    parser = argparse.ArgumentParser(description='Test tifa')

    parser.add_argument('--verbose', help="print executed commands", action="store_true")
    parser.add_argument('--flags', help="compile with flags (default: " + default_flags + ")",
            default = default_flags)

    parser.add_argument('--compiler', help="compiler binary (default g++)", default = "g++")
    
    args = parser.parse_args()

    return args

if __name__ == '__main__':
    units.helper.arguments = get_arguments()

    compiler_test.register()
    integer_template_test.register()

    units.helper.Tester.runTests()



    
