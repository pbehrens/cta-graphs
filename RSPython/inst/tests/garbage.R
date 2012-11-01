# This is used to test whether the garbage collection works
# correctly when we release an object in R and have the destructor
# called in Python.

library(RSPython)
importPythonModule("hierarchy")
z = .PythonNew("C", .module = "hierarchy")
z$foo(1, 2)
rm(z)
gc()


importPythonModule("gc")
.Python("collect", .module = "gc")

