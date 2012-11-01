#
# Test from R with
#  .PythonEval("import except"
#  .PythonEval("except.bar()")
#
#


import exceptions
import sys

def bar():
    print "In bar\n"
    raise ArithmeticError, 'A dummy error string from bar()'


def x():
    try:
        bar()
    except ArithmeticError, e:
        print "Caught error:"
        print e.args
        print e.__str__()
        

def y():
    try:
        bar()
    except ArithmeticError:
        print "caught error"
        print sys.exc_type
        print sys.exc_value
        
