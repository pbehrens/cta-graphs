#
# An example of registering Python functions
# (rather than C routines) to perform conversion
# from an R object.
# See FunctionConvertersTest.py in this directory
# for how these are used.


import RS

class SimpleMatrix:
    "A trivial matrix representation, avoiding the need for NumPy."
    def __init__(self, dims, data):
      "Constructor for SimpleMatrix, expecting the dimensions of the matrix and the data as a numerc vector"
      self.dims = dims
      self.data = data

def isMatrix(ref, klasses):
   "Determines if the S object ref is a matrix"
   # This could look in the klasses tuple which contains the
   # names of the R classes that the object referenced by `ref'
   # has. In other words, it is the value of
   #   class(ref$value)
   # However, we can also invoke R functions using the ref as a value.
   # In this case, we will call is.matrix().
   # This is slightly different than looking at the class names as it
   # handles "inheritance" more appropriately.
   ans = RS.call("is.matrix", ref)
   print("Is matrix? %d" % ans)
   return(ans)

def convertSMatrix(ref, klasses):
    "Converts an S matrix reference to a Python object"
    print("In convertSMatrix")
    dims = RS.call("dim", ref)
    print("Nrow %d" % dims[0])
    print("Ncol %d" % dims[1])
    print(RS.call("as.vector", ref))
    m = SimpleMatrix(dims, RS.call("as.vector", ref))
    print("We're done")
    return(m)

