#
# A class for testing some basic functionality
# from within R.
#

class RSTest:
    def __init__(self): pass 
    def test(self, x, y):
	"This is a test of calling a Python method from R"
	return 10
    def namedTest(self, x=1, y="a string"):
	"This is a test with default arguments"
	return 11
    def varTest(self, x = 1, **args):
	"This is a test method variable number of arguments"
	return 12




class ExtendedRSTest(RSTest):
     def __init__(self): pass

     def newTest():
	"A new method in the extended class"
	return("Value")
     def namedTest(self, x=1, y="a string"):
	"Overriddend version of namedTest"
	return("Overriding the namedTest method")