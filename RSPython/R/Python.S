.Python <- 
#
# Invoke a Python function.
#
#
function(func, ..., .module = NULL, .convert = TRUE)
{
 isPythonInitialized(TRUE)

 args <- list(...)
 .Call("RPy_callPython", as.character(func), args, names(args), as.character(.module), as.logical(.convert), FALSE)
}


.PythonNew <-
#
# Create a new Python object
#
# className - the name of the class of which we create an instance
# ... - arguments to the constructor (__init__ method)
# .module - the name of the module in which to find the class.
# .convert - indicates whether to attempt to convert the resulting object
#    to an R object or leave as a Python reference.
#
function(className, ..., .module=NULL, .convert = FALSE)
{
 isPythonInitialized(TRUE)

  args <- list(...)
 .Call("RPy_callPython", as.character(className),  args, names(args),
                           as.character(.module), as.logical(.convert), TRUE)

#  .Call("RS_PythonNewObject", as.character(className), as.character(.module),
#                     args, names(args), as.logical(.convert))
}


.PythonEvalFile <-
#
# Evaluate the contents of a file containing Python expressions.
#
function(cmd, ..., .module = NULL, .convert = TRUE)
{
 isPythonInitialized(TRUE)

 .PythonEval(cmd, ..., .module = .module, .convert = .convert, .isFile = TRUE)
}

.PythonEval <-
#
# Eval a Python expression as a string or a file.
#
function(cmd, ..., .module = NULL, .convert = TRUE, .isFile = FALSE)
{
 isPythonInitialized(TRUE)

 args <- list(...)
 .Call("RS_PythonEval", as.character(cmd), args, names(args), as.character(.module), as.logical(.convert), as.logical(.isFile))
}

.PythonMethod <-
#
# Call a method on Python object.
# ref - the reference to the Python object.
# method - the name of the method to invoke
# ...   - arguments to pass to the method, which  can be named and this
#         will be used when matching the arguments.
# .convert - logical value indicating whether to attempt to convert the result 
#           to an R object or leave as a Python reference object in R.
function(ref, method, ..., .convert = TRUE)
{
 isPythonInitialized(TRUE)

  args <- list(...)
 .Call("RPy_callMethod", ref, as.character(method), args, names(args), as.logical(.convert))
}


.PythonInit <-
#
# Initialize the Python interpreter.
#
# If there is no PYTHONPATH set, then we set our version so that
# one can find the RS modules.
#
# If there is a PYTHONPATH already set, then we take it and append
# our value to it. This can be inhibited by specifying `merge = F'
#
# Finally, if there is a non-null value for the path argument, then 
# we use that as the value for PYTHONPATH. Again, if merge = T, we 
# append the RS directories to it.

# If merge is T, then we take whatever value for path is given
# and append it to the current value of PYTHONPATH
#
function(path = NULL, merge = TRUE, warn = FALSE, force=TRUE) { 

 if(!force && isPythonInitialized(FALSE)) {
   warning("The Python interpreter was already initialized")
   return(TRUE)
 }

 path <- .PythonPath(path=path, merge=merge)

 if(!is.null(path) && path != "")
   Sys.putenv("PYTHONPATH"=path)

 status <- .Call("InitPython")
 if(warn) {
    warning("Implicit initialization of the Python interpreter with default settings")  
 }
 invisible(status)
}

.PythonPath <-
function(path=NULL, merge = TRUE)
{
 dirSep <- ifelse(R.version$os == "Win32", ";", ":",)

 if(length(path) > 1)
   path <- paste(path, collapse=dirSep)

 pyPath <- ifelse(is.null(path), Sys.getenv("PYTHONPATH") , path)

 ourPath <- paste(system.file("Python", package="RSPython"),
                  system.file("libs",   package="RSPython"), 
                  sep=dirSep)


 if(as.logical(merge)) {
  if(pyPath != "")
   pyPath <- paste(pyPath, ourPath, sep=dirSep)
  else
   pyPath <- ourPath
 }

 return(pyPath)
}


.PythonTerminate <- 
#
# Initialize the Python interpreter.
#
#
function() {
 isPythonInitialized()
  .Call("RS_PythonTerminatePython")
}

isPythonInitialized <-
function(init = FALSE)
{
 status <- .Call("RS_PythonIsInitialized")
 if(!status && init) {
   .PythonInit(warn = TRUE)
   status <- TRUE
 }
 return(status)
}
