import os
from copy import deepcopy

arguments=[]

class Tester:
    # Compile without generating .o file (useful for constexpr or templates
    # testing)
    @staticmethod
    def dry_compile(source_path, additional_flags=""):
        compile_commands = arguments.compiler + " " + arguments.flags + " " + \
                additional_flags + " " + source_path + " -S -o -"

        redirection = " > /dev/null 2> /dev/null"

        if arguments.verbose:
            print('\n' + compile_commands)

        if os.system(compile_commands + redirection) == 0:
            return True
        else:
            return False

    @staticmethod
    def compile(source_path, additional_flags="", object_path=None):

        (src_dir, src_file) = os.path.split(source_path)
        obj_file = os.path.splitext(src_file)[0] + ".o"

        if (object_path == None):
            object_path = src_dir + "/object/" + obj_file

        retcode = os.system("mkdir -p " + os.path.split(object_path)[0])
        assert(retcode == 0)

        return os.system(self.compiler_bin + " " + self.flags + " " +
                additional_flags + " " + source_path + " -o " + object_path)

    test_list=[] # (description, function); function shall return True if test
                 # is passed

    @staticmethod
    def addTest(description, function):
        Tester.test_list.append((description, function))

    @staticmethod
    def runTests():
        total = len(Tester.test_list)
        counter = arguments.resume

        for (description, fun) in Tester.test_list[arguments.resume:]:
            print("[" + str(counter) + "/" + str(total) + "] (" + description + ") ", end='')
            counter += 1
            if fun() == False:
                print("Failed")
                exit(-1)
            else:
                print("OK")

class StaticTest:
    number = None
    should_compile = None
    content = None

    def __init__(self, number, content, should_compile):
        self.number = deepcopy(number)
        self.should_compile = should_compile
        self.content = content

class StaticTestsGenerator(object):
    def __init__(self, guard_name):
        self.counter = 0
        self.guard_name = guard_name
        self.path = None
        self.test_list = []

    def append_test(self, expression, should_compile):
        comment = ""

        if should_compile:
            comment = "// Must compile\n"
        else:
            comment = "// Must not compile\n" 

        test_str = comment \
                 + "#ifdef " + self.guard_name + "_" + str(self.counter) \
                 + "\n" + expression + "\n" \
                 + "#endif\n"

        self.test_list.append(StaticTest(self.counter, test_str, should_compile))
        self.counter += 1

    def write_to_file(self, preamble, path):
        self.path = path

        with open(path, 'w', encoding = 'utf-8') as f:
            f.write(preamble)
            f.write('\n'.join(test.content for test in self.test_list))

    def __compile(self, num, should_compile):
        result = Tester.dry_compile(self.path, "-D" + self.guard_name + "_" + str(num))

        if should_compile:
            return result
        else:
            return (not result)

    def add_to_tester(self, description):
        for test in self.test_list:
            fun = lambda num=test.number, comp=test.should_compile : self.__compile(num, comp)
            Tester.addTest(description, fun)

