/*
 Illustration of how to register a converter for
 transforming Python pygtk objects to R objects related to 
 the RGtk package.

 See Rpyggtk.py for more information about how to run this code.
*/
#include "pygtk.h"
#include "Rinternals.h"
#include "Rdefines.h"

#include "UserConverters.h"
#include "gtkUtils.h"


/*
  Given the PyObject, we need to find the underlying reference to the
  GtkObject. It is in the python object's dictionary as '_o'.
  Then we can access the C-level structur which is a PyObject with
  a GtkObject field below the basic PyObject data.
*/
GtkObject *
Rpy_getGtkObject(PyObject *obj)
{
  PyObject *d, *ans;
  GtkObject *gobj;
  d = PyObject_GetAttrString(obj, "__dict__");
  ans = PyDict_GetItemString(d, "_o");
  gobj = PyGtk_Get(ans);
  return(gobj);
}


/*
 Determine if the PyObject is a pygtk instance of a GtkObject.
*/
Rboolean 
R_pygtk_match(PyObject *obj, PyClassObject *pyClass, RSFromPythonConverter *converter)
{
   char *tmp;
   GtkObject *gobj;
   if(!pyClass)
       return(FALSE);
   tmp = PyString_AsString(pyClass->cl_name);
   return(strncmp("Gtk", tmp, 3) == 0);
}

/*
 Create the actual R object to represent the underlying GtkObject.
*/
SEXP
R_pygtk_converter(PyObject *obj, PyClassObject *pyClass, RSFromPythonConverter *converter)
{
   char *tmp;
   SEXP ans;
   PyGtk_Object *gtk_obj = (PyGtk_Object *)obj;
   GtkObject *gobj;
   gobj = Rpy_getGtkObject(obj);

   tmp = PyString_AsString(pyClass->cl_name);
#if 0
   fprintf(stderr, "Convering PyGtk object %p (class = %s) -> %p to an R Gtk reference\n", (void*) obj, tmp, gobj);fflush(stderr);
#endif

   ans = toRPointer((void *) gobj, tmp);
#if 0
   fprintf(stderr, "Getting class hierarchy for %p\n", (void *) gobj);
#endif
   SET_CLASS(ans, R_internal_getTypeHierarchy(GTK_OBJECT_TYPE(gobj))); 

   return(ans);
}

