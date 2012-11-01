
def a(x,*args, **namedArgs):
    b(x, args, _named=namedArgs)

def b(x, *args, **namedArgs):
    print(len(args), type(args))
    print(len(namedArgs), type(namedArgs))    
    print(args)
    print(namedArgs)
    
