import random
import math
from itertools import product

class IntType:
    bits = None
    is_signed = None
    maxval = None
    litsuf = None

    def __init__(self, bits, is_signed):
        self.bits = bits
        self.is_signed = is_signed
        self.maxval = (2**bits-1 if not is_signed else 2**(bits-1)-1)
        self.litsuf = ("_s" if is_signed else "_u") + str(bits)

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

types = [
    IntType(bits, is_signed)
    for (is_signed, bits) in product([True, False], [8, 16, 32, 64])
]

class Value:
    tp = None
    val = None

    def __init__(self, tp, val = None):
        self.tp = tp

        if val == None:
            # Small value to prevent overflow
            self.val = random.randint(1, int(math.sqrt(tp.maxval)) - 1)

            if tp.is_signed and random.randint(0, 1) == 1:
                self.val *= -1
        else:
            self.val = val

    def get_string(self):
        prepend = ""
        append = ""

        if self.val < 0 and self.tp.is_signed:
            prepend = "-"
            append = ""
        elif self.val < 0 and not self.tp.is_signed:
            prepend = "complement("
            append = ")"

        match random.randint(1, 4):
            case 1:
                return prepend + "0b" + "{0:b}".format(abs(self.val)) + self.tp.litsuf + append
            case 2:
                return prepend + "0x" + "{0:x}".format(abs(self.val)) + self.tp.litsuf + append
            case 3:
                return prepend + "0" + "{0:o}".format(abs(self.val)) + self.tp.litsuf + append
            case 4:
                return prepend + str(abs(self.val)) + self.tp.litsuf + append

def declare_variable(typename, varname, value, is_constexpr = False, maybe_unused = False):
    return ("[[maybe_unused]] " if maybe_unused else "") + \
           ("constexpr " if is_constexpr else "") + \
           typename + " " + varname + " = " + typename + "(" + str(value) + ");\n"

operators_arithmetic = ["+", "-", "*", "/", "%"]
operators_logic = ["==", "!=", "<", ">", "<=", ">="]
operators_bitwise = ["^", "&", "|"]
operators_arithmetic_assignment = ["+=", "-=", "*=", "/=", "%="]
operators_bitwise_assignment = ["^=", "&=", "|="]

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
