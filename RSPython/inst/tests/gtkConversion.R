library(RSPython)

dyn.load("gtkConversion.so")
addFromPythonConverter("R_pygtk_match", "R_pygtk_converter",
                   "Convert a PyGtk object to an R reference to a Gtk widget")


ident =
  function(...) {
     print(list(...))
     TRUE
  }

# Not really needed except for computing  the class hierarchy interactively from a python call.
library(RGtk)



