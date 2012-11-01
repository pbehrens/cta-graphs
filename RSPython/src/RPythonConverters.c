#include "RPythonConverters.h"
#include "UserConverters.h"
#include "RPythonReferences.h"

#include "PythonCall.h"



USER_OBJECT_
ConvertPyDictToR(PyObject *pyobj, PyObject *self)
{
  PyObject *keys = PyDict_Keys(pyobj);
  PyObject *items = PyDict_Items(pyobj);
  int n, i;
  SEXP ans, names;

  PROTECT(ans = NEW_LIST(n));
  PROTECT(names = NEW_CHARACTER(n));
  n = PyDict_Size(pyobj);
  for(i = 0; i < n ; i++) {
      char *str;
      SET_VECTOR_ELT(ans, i, fromPython(self, PyList_GET_ITEM(items, i)));

      str = PyString_AsString(PyList_GET_ITEM(keys, i));
      SET_STRING_ELT(names, i, COPY_TO_USER_STRING(str));
  }

  SET_NAMES(ans, names);
  UNPROTECT(2);

  return(ans);
}


/* 
 This converts a Python object to an S object.

 This covers enough converters to get simple
examples working - see Docs/overview.nw.

*/
USER_OBJECT_
fromPython(PyObject *self, PyObject *pyobj)
{
  USER_OBJECT_ arg = NULL_USER_OBJECT;
  int d;

  if(pyobj == Py_None)
    return(NULL_USER_OBJECT);

  if(isSForeignReference(pyobj)) {
#ifdef RSPYTHON_DEBUG
    fprintf(stderr, "Resolving python reference to S object\n");
#endif
    return(RPython(resolveRForeignReference)(pyobj));
  }


  if(FromPythonConverters) {
    Rboolean ok;
    arg = userLevelFromPythonConversion(pyobj, &ok);
    if(ok)
      return(arg);
  }  


    /* If we have a list, operate on it element-wise. */
  if(PyList_Check(pyobj) || PyTuple_Check(pyobj)) {
    PyObject *el;
    PyTypeObject *type;
    int n, i;
    Rboolean isList = PyList_Check(pyobj);

    n = isList ? PyList_Size(pyobj) : PyTuple_Size(pyobj);
#ifdef RSPYTHON_DEBUG
    fprintf(stderr, "Converting from Python %s\n", (isList ? "list" : "tuple"));
#endif

      /* See if the elements in the list all have the same
         type and are primitives.
       */
    type = getListElementType(pyobj);
    if(type == NULL)
      type = &PyList_Type;
   
    if(type == &PyInt_Type) {
     arg = NEW_INTEGER(n);
     for(i = 0; i < n; i++) {
       el = isList ? PyList_GET_ITEM(pyobj, i) :  PyTuple_GetItem(pyobj, i);
      INTEGER_DATA(arg)[i] = PyInt_AsLong(el);
     }
    } else if(type == &PyLong_Type) {
     arg = NEW_NUMERIC(n);
     for(i = 0; i < n; i++) {
       el = isList ? PyList_GET_ITEM(pyobj, i) :  PyTuple_GetItem(pyobj, i);
      NUMERIC_DATA(arg)[i] = PyLong_AsLong(el);
     }
    } else if(type == &PyString_Type) {
     PROTECT(arg = NEW_CHARACTER(n));
     for(i = 0; i < n; i++) {
       char *str;
       el = isList ? PyList_GET_ITEM(pyobj, i) :  PyTuple_GetItem(pyobj, i);
       str = PyString_AsString(el);
       if(str)
	   SET_STRING_ELT(arg, i, COPY_TO_USER_STRING(str));
     }
     UNPROTECT(1);
    } else if(type == &PyFloat_Type) {
     PROTECT(arg = NEW_NUMERIC(n));
     for(i = 0; i < n; i++) {
       el = isList ? PyList_GET_ITEM(pyobj, i) :  PyTuple_GetItem(pyobj, i);
       NUMERIC_DATA(arg)[i] = PyFloat_AsDouble(el);
     }
     UNPROTECT(1);
    } else {
     PROTECT(arg = NEW_LIST(n));
     for(i = 0; i < n; i++) {
       el = isList ? PyList_GET_ITEM(pyobj, i) :  PyTuple_GetItem(pyobj, i);
       SET_VECTOR_ELT(arg, i, fromPython(self, el));
     }
     UNPROTECT(1);
    }
  } else if(PyInt_Check(pyobj)) {
      /* Otherwise, no list, and just a single integer. */ 
    PROTECT(arg = NEW_INTEGER(1));
    INTEGER_DATA(arg)[0] = PyInt_AsLong(pyobj);
    UNPROTECT(1);
  } else if(PyFloat_Check(pyobj)) {
      /* Otherwise, no list, and just a single integer. */ 
    PROTECT(arg = NEW_NUMERIC(1));
    NUMERIC_DATA(arg)[0] = PyFloat_AsDouble(pyobj);
    UNPROTECT(1);
  } else if(PyLong_Check(pyobj)) {
      /* Otherwise, no list, and just a single integer. */ 
    PROTECT(arg = NEW_NUMERIC(1));
    NUMERIC_DATA(arg)[0] = (double) PyLong_AsLong(pyobj);
    UNPROTECT(1);
  } else if(PyString_Check(pyobj)) {
     PROTECT(arg = NEW_CHARACTER(1));
     { 
       char *str = PyString_AsString(pyobj);
       if(str)
	   SET_STRING_ELT(arg, 0, COPY_TO_USER_STRING(str));
     }
     UNPROTECT(1);
  } else if(PyDict_Check(pyobj)) {
	  arg = ConvertPyDictToR(pyobj, self);
  } else if(PyInstance_Check(pyobj)) {
         arg = registerPythonReference(pyobj, NULL);
  } else {
	  fprintf(stderr, "Failing in conversion from Python object: Unhandled case!\n");
  }

 return(arg);
}


/*
 This iterates over the elements of Python list
 to determine whether the elements are of the same
 type. If so, it returns this type. Otherwise, it returns
 NULL.
  This is used to collapse Python lists to S vectors
  when we have homogeneous types in the list (and are 
  primitive.)

 Can we use PySequence protocol here.
 */
PyTypeObject *
getListElementType(PyObject *pyobj)
{
 int n, i;
 PyObject *el;
 PyTypeObject *type;

 if(!PySequence_Check(pyobj))
   return(NULL);
  
 n = PySequence_Size(pyobj);
 if(n < 1)
     return(NULL);

 el = PySequence_GetItem(pyobj, 0);
 type = el->ob_type;

 for(i = 1; i < n ; i++) {
     el = PySequence_GetItem(pyobj, i);
     if(el->ob_type != type)
	 return(NULL);
 }
 
 return(type); 
}

/*
 Converts an S object to a Python representation.
 */
PyObject *
toPython(USER_OBJECT_ val)
{
  PyObject *result;
  int n, i;
  USER_OBJECT_ names;
  n = GET_LENGTH(val);

  if(n == 0)
    return(Py_None);

 
 if(!isSPrimitive(val)) {
  if(ToPythonConverters) {
    Rboolean ok;
    result = userLevelToPythonConversion(val, &ok);
    if(ok)
      return(result);
  }
 }

    /* If it is an object with a class,
       then don't take it apart as a list.
     */
  if(GET_LENGTH(GET_CLASS(val)) > 0) {
     val = RPython(createRAnonymousReference)(val);
     return(createRPythonReference(val, NULL));     
  }
 
    /* Now, if it has names, then treat it as a dictionary.*/
  names = GET_NAMES(val);
  if(GET_LENGTH(names)) {
    /* Create a dictionary. */
    return(RPython(createPythonDictionary)(val, names));
  }

     /* Otherwise, just convert it  */
  if(IS_NUMERIC(val)) {
    if(n == 1) {
      result = PyFloat_FromDouble(NUMERIC_DATA(val)[0]);
    } else {
      result = PyList_New(n);
      Py_INCREF(result); 
      for(i = 0; i < n; i++) {
       PyList_SetItem(result, i, PyFloat_FromDouble(NUMERIC_DATA(val)[i])) ;
      }
      Py_DECREF(result);
    }
  } else if(IS_CHARACTER(val)) {
    if(n == 1) {
      result = PyString_FromString(CHAR(STRING_ELT(val,0)));
    } else {
      result = PyList_New(n);
      Py_INCREF(result); 
      for(i = 0; i < n; i++) {
       PyList_SetItem(result, i, PyString_FromString(CHAR(STRING_ELT(val,i)))) ;
      }
      Py_DECREF(result);
    }
  } else if(IS_INTEGER(val)) {
    if(n == 1) {
      result = PyInt_FromLong(INTEGER_DATA(val)[0]);
    } else {
      result = PyList_New(n);
      Py_INCREF(result); 
       for(i = 0; i < n; i++) {
        PyList_SetItem(result, i, PyInt_FromLong(INTEGER_DATA(val)[i])) ;
       }
      Py_DECREF(result);
    }
  } else if(IS_LOGICAL(val))  {
    if(n == 1) {
      result = PyInt_FromLong(LOGICAL_DATA(val)[0]);
    } else {
      result = PyList_New(n);
      Py_INCREF(result); 
       for(i = 0; i < n; i++) {
        PyList_SetItem(result, i, PyInt_FromLong(LOGICAL_DATA(val)[i])) ;
       }
      Py_DECREF(result);
    }
  } else if(IS_LIST(val)) {
      result = PyList_New(n);
      Py_INCREF(result); 
       for(i = 0; i < n; i++) {
        PyList_SetItem(result, i, toPython(VECTOR_ELT(val, i)));
       }
      Py_DECREF(result);
  } else {
     val = RPython(createRAnonymousReference)(val);
     result = createRPythonReference(val, NULL);
  }

  return(result);
}

/*
 Creates a python dictionary from an S object whose names
 are given as a second argument.
 */
PyObject *
RPython(createPythonDictionary)(USER_OBJECT_ val, USER_OBJECT_ names)
{
  int n = GET_LENGTH(names), i;
  PyObject *d, *el = NULL;

   d = PyDict_New();
   Py_INCREF(d);
   for(i = 0 ; i < n ; i++) {
     if(IS_NUMERIC(val)) {
       el = PyFloat_FromDouble(NUMERIC_DATA(val)[i]);
     } else if(IS_CHARACTER(val)) {
       el = PyString_FromString(CHAR_DEREF(STRING_ELT(val,i)));
     } else if(IS_INTEGER(val)) {
       el = PyInt_FromLong(INTEGER_DATA(val)[i]);
     }else if(IS_LOGICAL(val)) {
       el = PyInt_FromLong(INTEGER_DATA(val)[i]);
     } else if(IS_LIST(val))
       el = toPython(VECTOR_ELT(val, i));
     else {
	 el = NULL;
     }

     PyDict_SetItemString(d, CHAR_DEREF(STRING_ELT(names, i)), el);
   }

   Py_DECREF(d);

   return(d);
}


Rboolean
isSPrimitive(USER_OBJECT_ val)
{
 Rboolean ans;

  ans = IS_CHARACTER(val) || IS_NUMERIC(val) || IS_LOGICAL(val) || IS_INTEGER(val);

  if(ans) {
    ans = GET_LENGTH(GET_DIM(val)) == 0;
  }

  return(ans);
}
