import RSInternal

def  call(name, *args, **other):
     "call an R function, possibly giving ordered and named arguments"
     convert = other.get('convert',1)
     ref = other.get('ref',None)
     isNested = other.get('_nested',None)     
     try:  
        del other['convert']
     except: pass
     try:  
        del other['ref']
     except: pass
     try:  
        del other['_nested']
     except: pass

     if isNested:
          args = args[0]
          other = other['_nested']
     return RSInternal.call(name, args, other, convert, ref)

def refCall(ref, name, *args, **namedArgs):
     "Call a method named 'name' "
     convert = other.get('convert',1)
     ref = other.get('ref',None)
     isNested = other.get('_nested',None)     
     try:  
        del other['convert']
     except: pass
     try:  
        del other['ref']
     except: pass
     try:  
        del other['_nested']
     except: pass
     if isNested:
          args = args[0]
          other = other['_nested']          
     RSInternal.refCall(ref, name, args, namedArgs)


def get(name):
    return(RSInternal.get(name))

def setConverter(converter, match, description=''):
    "Register a Python function for converting an S object to a Python object"
    idx = RSInternal.registerConverter(converter, match, description)
    return(idx)

def getConverterDescriptions(toPython=1):
    "Get a list of the converters between R and Python"
    x = call("getPythonConverterDescriptions",toPython)
    return(x[0])

def library(name):
     "Loads the R package/library named name into the R session"
     return(call("library", name));

def search():
     "Returns the search path of the R interpreter"
     return(call("search"));


def __getattribute__(name):
     def wrapper(*args, **keywordargs):
           return RS.call(name, *args, **keywordargs)
     return wrapper


class NoSuchFunctionException(Exception):
     "Exception indicating that no S function was found bound to a particular name"


library("RSPython")
call("referenceManager")
print("Initialized R-Python interface package.")
