import RS
import FunctionConverters;
RS.setConverter(FunctionConverters.convertSMatrix, FunctionConverters.isMatrix, "A test for matrices")
m = RS.call("matrix", RS.call("rnorm",12), 3,4)
