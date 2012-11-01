.First.lib <-
function(libname, pkgname) {
 library.dynam("RSPython", pkgname, libname)
 .PythonInit()
 setPythonHandler(referenceManager())
}
