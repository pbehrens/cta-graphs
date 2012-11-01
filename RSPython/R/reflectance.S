
getSuperClasses <-
function(obj, module=NULL)
{
 if(!missing(module))
  obj <- c(obj, module)

 .Call("RS_PygetSuperClasses", obj)
}

# e.g.
#  getSuperClasses("addinfourl", "urllib")
# u <- .PythonNew("urlopen", "http://www.omegahat.org/index.html", .module="urllib")
#
# If the PYTHONPATH environment variable includes system.file("", pkg="RSPython")
#    getSuperClasses("A", "hierarchy")



pythonMethods <- 
#
# Get the methods in a Python object or class
# all  whether to query the base classes of this class
#      or just do a shallow query.
function(obj, all = FALSE)
{
  .Call("RS_PythonGetMethods", obj, as.logical(all))
}


pythonFunction <-
function(obj, module=NULL)
{
 if(!missing(module))
  obj <- c(obj, module)

 .Call("RS_Python_describeFunction", obj)
}

