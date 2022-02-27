import os
import sys
import subprocess

arguments = None

class Tester:
    # Compile without generating .o file (useful for constexpr or templates
    # testing)
    @staticmethod
    def dry_compile(source_path, additional_flags=""):
        flags = list(filter(lambda item : item != '', [
            *arguments.flags.split(' '),
            *additional_flags.split(' '),
            '-S', '-o' , '-'
        ]))

        compile_commands = [arguments.compiler, *flags, source_path]

        if arguments.verbose:
            print(' '.join(compile_commands))

        retcode = subprocess.run(
            compile_commands,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL
        ).returncode

        if retcode < 0:
            print("Compiler terminated by signal, exiting...")
            sys.exit(1)

        return not bool(retcode)

    test_list=[] # (description, function); function shall return True if test
                 # is passed

    @staticmethod
    def addTest(description, function):
        Tester.test_list.append((description, function))

    @staticmethod
    def runTests():
        total = len(Tester.test_list)
        counter = arguments.resume

        for (description, fun) in Tester.test_list[(arguments.resume - 1):]:
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
        self.number = number
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

