#include "RPythonReferences.h"
#include "RPythonConverters.h"

/*
 These routines are used to store and access
 the reference manager which is an R object
 that maintains a named list of objects
 that it exports to Python.
 */
static SEXP defaultMethodHandler = NULL;

/*
 Registers an S object as the reference manager handler.
 This is then accessed when a call from Python
 involving an RForeignReference object is involved.
 */

USER_OBJECT_
RPython(setDefaultHandlerFunction)(USER_OBJECT_ handler)
{
 extern void R_PreserveObject(SEXP);
 USER_OBJECT_ old = NULL_USER_OBJECT;
  if(defaultMethodHandler)
    old = defaultMethodHandler;   

   R_PreserveObject(handler);
   defaultMethodHandler = handler;

 return(old);     
}    

/*
  Get the current reference manager object.
  
 */
USER_OBJECT_
RPython(defaultHandlerFunction)()
{
   if(defaultMethodHandler == NULL) {
     USER_OBJECT_ e, fun, val;
     int errorOccurred = 0;
       PROTECT(fun = Rf_findFun(Rf_install((char *)"referenceManager"),  R_GlobalEnv));
       PROTECT(e = allocVector(LANGSXP,1));
       SETCAR(e, fun);
       PROTECT(val = R_tryEval(e, R_GlobalEnv, &errorOccurred));
       if(errorOccurred) {
          RSPython_raiseException();
	  return(NULL_USER_OBJECT);
       }
       RPython(setDefaultHandlerFunction)(val);
       UNPROTECT(3);
       PROBLEM "the Python callback manager has been registered implicitly using the defaults (referenceManager())!"
       WARN;
    }
   return(defaultMethodHandler);
}



/*
  This registers the R object as an exported reference object.
 */
USER_OBJECT_
RPython(createRAnonymousReference)(USER_OBJECT_ obj)
{
  USER_OBJECT_ expr, value;
  USER_OBJECT_ handler = RPython(defaultHandlerFunction)();
  USER_OBJECT_ createFun;
  int errorOccurred = 0;

       /* The create reference method had better be the first element in the list! */
     PROTECT(expr = allocVector(LANGSXP, 2));
     createFun = VECTOR_ELT(handler, 1);

       SETCAR(expr, createFun);
       SETCAR(CDR(expr), obj);

       value = R_tryEval(expr, R_GlobalEnv, &errorOccurred);
       if(errorOccurred) {
         RSPython_raiseException();
	 return(NULL_USER_OBJECT);
       }

     UNPROTECT(1);
  
 return(value);
}

enum {CALL_HANDLER, CREATE_REFERENCE, ADD_REFERENCE, REMOVE_REFERENCE, GET_REFERENCE, REFERENCE_LIST, TOTAL_COUNT};

USER_OBJECT_
RPython(resolveRForeignReference)(PyObject *val)
{
 USER_OBJECT_ value, e;
 PyObject *tmp;
 int errorOccurred = 0;
 USER_OBJECT_ handler = RPython(defaultHandlerFunction)();

    
 tmp = PyObject_GetAttrString(val, "name");
 PROTECT(e =  allocVector(LANGSXP, 2));
   /* The getReference method had better be the 5th element in the handler!
      Will be more robust in the future.
    */
 SETCAR(e, VECTOR_ELT(handler, GET_REFERENCE));
 SETCAR(CDR(e), fromPython(NULL, tmp));

   value = R_tryEval(e, R_GlobalEnv, &errorOccurred);
   if(errorOccurred) {
      RSPython_raiseException();
      return(NULL_USER_OBJECT);
   }

 UNPROTECT(1);

 return(value);  
}

/*
 Same as the resolveRForeignReference above, but calls a different
 function in the handler. These could and should be consolidated.
 */

USER_OBJECT_
RPython(removeRForeignReference)(PyObject *val)
{
 USER_OBJECT_ value, e;
 PyObject *tmp;
 int errorOccurred = 0;
 USER_OBJECT_ handler = RPython(defaultHandlerFunction)();

    
 tmp = PyObject_GetAttrString(val, "name");
 PROTECT(e =  allocVector(LANGSXP, 2));
   /* The getReference method had better be the 5th element in the handler!
      Will be more robust in the future.
    */
 SETCAR(e, VECTOR_ELT(handler, REMOVE_REFERENCE));
 SETCAR(CDR(e), fromPython(NULL, tmp));

   value = R_tryEval(e, R_GlobalEnv, &errorOccurred);
   if(errorOccurred) {
      RSPython_raiseException();
      return(NULL_USER_OBJECT);
   }

 UNPROTECT(1);

 return(value);  
}


/*
 Creates a Python object of class RForeignReference
 that represents the R object `val' within Python.
 */
PyObject *
createRPythonReference(USER_OBJECT_ val, PyObject *func)
{
 PyObject *ref;
 PyObject *args;
 PyObject *s;
 USER_OBJECT_ classes;
 PyObject *pyClasses;
 int i, numClasses;

 if(!func || func == Py_None) {
   const char *  pyClassName = NULL;
     /* 
        Check what Python class we should be looking for.
        
      */
   USER_OBJECT_ targetClasses = VECTOR_ELT(val, 3);
   if(GET_LENGTH(targetClasses) > 0) {
     pyClassName = CHAR_DEREF(STRING_ELT(targetClasses, 0));
   }
   func = getPythonReferenceConstructor(pyClassName);
 }

  args = PyTuple_New(3);
   /* The id.  This is just a string and we should handle it
      here to avoid the numerous calls to toPython().
    */
  s = PyString_FromString(CHAR_DEREF(STRING_ELT(VECTOR_ELT(val, 0), 0)));
  PyTuple_SetItem(args, 0, s);
   /* The typeof() of the value */
  s = PyString_FromString(CHAR_DEREF(STRING_ELT(VECTOR_ELT(val, 4), 0)));
  PyTuple_SetItem(args, 1, s);
   /* The class() of the value */
  classes = GET_CLASS(VECTOR_ELT(val,1));
  numClasses = GET_LENGTH(classes);
  pyClasses = PyTuple_New(numClasses);
  for(i = 0; i < numClasses; i++) {
    s = PyString_FromString(CHAR_DEREF(STRING_ELT(classes, i)));
    PyTuple_SetItem(pyClasses, i, s);
  }
  PyTuple_SetItem(args, 2, pyClasses);

  ref = PyEval_CallObject(func, args);

  RS_checkError();

 return(ref);
}

/*
  Gets the Python constructor function for
  creating an RReference object.
 */
PyObject *
getPythonReferenceConstructor(const char* className)
{
 PyObject *module, *func, *d;

 if(className == NULL)
   className = "RForeignReference";

 module = PyImport_ImportModule("RReference");
  if(!module) {
    PROBLEM "Put the directory containg RReference.py in your PYTHONPATH environment variable"
    ERROR;
  }
  d = PyModule_GetDict(module);
  func = PyMapping_GetItemString(d, (char*) className);
  
  return(func);
}
