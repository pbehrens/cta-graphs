class F:
    "A Base class"
    def __init__(self):
       ""
       print("In __init__ for hierarchy.F")
class G:
    "Second Base class"

class B(F):
    "2nd generation"

class C(F, G):
    "2nd generation"
    def foo(x,y,z=1,b=2):
        return(1)

    def __del__(self):
        ""
        print("__del__ for class C")

class D(F):
    "2nd generation"
    def foo(x,y):
        return(1)
    
class A(B,C,D):
    "3nd generation"
    def __init__(self):
        ""
        print("In initialization for hierarchy.A")
    def foo(x, y, z=1, b=2):
        "Standard and named arguments"
        print(locals())
    def bar(x, y, *other, **namedOthers):
        "Standard and positional and keyword arguments"
        print(locals())
    def foobar(x, y=1, *other, **namedOthers):
        "Standard, default and positional and keyword arguments"
        print(locals())
    def foo1(x, y=1, *other):
        "Standard, default and positional arguments"        
        print(locals())
    def foo3(x, y=1, **namedArgs):
        "Standard, default and keyword arguments"        
        print(locals())                                

