from RReference import *
class SForeignReferenceA(SForeignReference, A):
   def foo3(x, y = 1, **namedArgs):
	"Standard, default and keyword arguments"
	return( RS.call("foo3", x, y, namedArgs) )
   def foo1(x, y = 1, *other):
	"Standard, default and positional arguments"
	return( RS.call("foo1", x, y, other) )
   def foo(x, y, z = 1, b = 2):
	"Standard and named arguments"
	return( RS.call("foo", x, y, z, b) )
   def foobar(x, y = 1, *other, **namedOthers):
	"Standard, default and positional and keyword arguments"
	return( RS.call("foobar", x, y, other, namedOthers) )
   def bar(x, y, *other, **namedOthers):
	"Standard and positional and keyword arguments"
	return( RS.call("bar", x, y, other, namedOthers) )

