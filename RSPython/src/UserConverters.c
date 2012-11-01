/*

  This file provides the routines that handle the user-level
  or extensible converter mechanism which allows users of the
  package/module to register their own converters to 
  translate an S object to Python or in the other direction.

  See also GeneralConverters.c
 */

#include "UserConverters.h"
#include "RPythonReferences.h"
#include "RPythonConverters.h" /* getListElementType() */

/* The match and converter function for handling S functions.
   These identify a function object and convert it to a Python object of class
   RForeignFunction.
 */
Rboolean isRSFunctionMatch(USER_OBJECT_ obj, PyClassObject *pyClass, char *signature, Rboolean isArray, int arrayLen, RSToPythonConverter *converter);
PyObject *RSFunctionConverter(USER_OBJECT_ obj, PyClassObject *pyClass, char *signature, Rboolean isArray, int arrayLen, RSToPythonConverter *converter);



Rboolean RS_isPyMapping(PyObject *obj, PyClassObject *pyClass, RSFromPythonConverter *converter);
USER_OBJECT_ RSconvertPyMapping(PyObject *obj, PyClassObject *pyClass, RSFromPythonConverter *converter);

/*
  Registers the converters that we all likely want.
 */
void
RPython(registerDefaultConverters)()
{
    /* Converting R functions to RForeignFunction Python objects. */
  addToPythonConverterInfo(isRSFunctionMatch, RSFunctionConverter, FALSE, NULL, "Converts a S function to an RForeignFunction", NULL);

  addFromPythonConverterInfo(RS_isPyMapping,RSconvertPyMapping, FALSE, NULL, "Convert a Mapping object from Python to an R (named) list", NULL);
}

/*
 Iterates over the registered converters for translating from
 Python objects to S objects.
 If any of these succeed, then set ok to be TRUE and return
 the value. Otherwise, return NULL and leave it to the default
 converters.
*/

USER_OBJECT_
userLevelFromPythonConversion(PyObject *val, Rboolean *ok)
{
  PyClassObject *pyClass = NULL;
  RSFromPythonConverter *el = FromPythonConverters;

  if(PyInstance_Check(val)) {
    pyClass = ((PyInstanceObject*)val)->in_class;
  }
  while(el) {
    USER_OBJECT_ result;
    if(el->match(val, pyClass,  el)) {
      result = el->converter(val, pyClass, el);
      *ok = TRUE;
      return(result);
    }
    el = el->next;
  }

  *ok = FALSE;
  return(NULL_USER_OBJECT);
}


PyObject *
userLevelToPythonConversion(USER_OBJECT_ val, Rboolean *ok)
{
  int arrayLen;
  Rboolean isArray;
  RSToPythonConverter *el = ToPythonConverters;
  PyObject *result;
  PyClassObject *pyClass = (PyClassObject *)Py_None;

  isArray = GET_LENGTH(val) > 1;
  if(isArray)
    arrayLen = GET_LENGTH(val);
  else
    arrayLen = 0;

  while(el) {
    if(el->match(val, pyClass, NULL, isArray, arrayLen, el)) {
      result = el->converter(val, pyClass, NULL, isArray, arrayLen, el);
      *ok = TRUE;
      return(result);
    }
    el = el->next;
  }

  *ok = FALSE;
  return(Py_None);
}



Rboolean
isRSFunctionMatch(USER_OBJECT_ obj, PyClassObject *pyClass, char *signature, Rboolean isArray, int arrayLen, RSToPythonConverter *converter)
{
  return(TYPEOF(obj) == CLOSXP);
}


/*
 Calls a Python function to convert an S object to Python. 
 */
PyObject *
RSFunctionConverter(USER_OBJECT_ obj, PyClassObject *pyClass, char *signature, Rboolean isArray, int arrayLen, RSToPythonConverter *converter)
{
    /* Put this away in the referenceManager */
  obj = RPython(createRAnonymousReference)(obj);
    /* Now create an object of class RForeignFunction. */

 return(createRPythonReference(obj, getPythonReferenceConstructor("RForeignFunction")));  
}


Rboolean
RS_isPyMapping(PyObject *obj, PyClassObject *pyClass, RSFromPythonConverter *converter)
{
#if 1
    return(PyMapping_Check(obj) ? PyMapping_Values(obj) != NULL : FALSE);
#else
    return(FALSE);
#endif
}

USER_OBJECT_
RSconvertPyMapping(PyObject *obj, PyClassObject *pyClass, RSFromPythonConverter *converter)
{

    PyObject *keys;
    PyObject *values;

    PyObject *el;
    PyTypeObject *type;

    USER_OBJECT_ ans;
    int n, i;

      /* Get the values and the keys for this mapping.
         The keys might be NULL. */

    values = PyMapping_Values(obj);

    keys = PyMapping_Keys(obj);
    n = PyMapping_Length(obj);

    /* Now check whether the elements in the values list are homogeneous
       and if so, what type do they have. This allows us to map to a vector
       in R if the type is a primitive. */
    type = getListElementType(values);
    if(type == NULL)
      type = &PyList_Type;



    if(type == &PyInt_Type) {
     ans = NEW_INTEGER(n);
     for(i = 0; i < n; i++) {
	 el = PyList_GET_ITEM(values, i);
	 INTEGER_DATA(ans)[i] = PyInt_AsLong(el);
     }
    } else if(type == &PyString_Type) {
     PROTECT(ans = NEW_CHARACTER(n));
     for(i = 0; i < n; i++) {
	 el = PyList_GET_ITEM(values, i);
	 SET_STRING_ELT(ans, i, COPY_TO_USER_STRING(PyString_AsString(el)));
     }
     UNPROTECT(1);
    } else if(type == &PyFloat_Type) {
     PROTECT(ans = NEW_NUMERIC(n));
     for(i = 0; i < n; i++) {
	 el = PyList_GET_ITEM(values, i);
	 NUMERIC_DATA(ans)[i] = PyFloat_AsDouble(el);
     }
     UNPROTECT(1);
    } else {
     PROTECT(ans = NEW_LIST(n));
     for(i = 0; i < n; i++) {
	 el = PyList_GET_ITEM(values, i);
	 SET_VECTOR_ELT(ans, i, fromPython(NULL, el));
     }
     UNPROTECT(1);
    }



      /* Now if this is dictionary (i.e. with keys), use those. */
    if(n > 0 && keys && keys != Py_None) {
	USER_OBJECT_ names;
	PyObject *key;
	PROTECT(ans);
	PROTECT(names = NEW_CHARACTER(n));
	for(i = 0; i < n ; i++) {
	    key = PyList_GET_ITEM(keys, i);
	    if(key != NULL && key != Py_None && PyString_Check(key))
  	      SET_STRING_ELT(names, i, COPY_TO_USER_STRING(PyString_AsString(key)));
            else {
		   PROBLEM "Problems in converting python object to S. Usually signs of a problem in the Python code."
		   ERROR;
	    }

	}
        SET_NAMES(ans, names);
        UNPROTECT(2);
    }

    return(ans);
}
