# Run this from within a python session.
# Need to have the relevant environment variables set to find RSPython
# and be able to start an embedded R session .
#
# Also, we need libRGtk.so since we use code from that. Install RGtk
# with --clean to create that library that this one can link to.
# and make certain to add <R-library-directionr>/RGtk/libs to LD_LIBRARY_PATH
#
# See GNUmakefile to change the variables needed for building gtkConversion.so
#

import RS
from R import R

R.source("gtkConversion.R")

import pygtk
from gtk import *
import _gtk
w = new(GtkWindow)

# This is the piece that tests whether the converters work
R.ident(w)

# Now that we have this working, we can create a GtkDrawingArea
# and then pass this to
#   R.asGtkDevice(myCanvas)
#
#


b  = new (GtkButton, label = "Bob")
w.add(b)
w.show_all()
mainloop()

