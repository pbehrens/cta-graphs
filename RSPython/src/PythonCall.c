#include "RPythonReferences.h"
#include "PythonFunctionConverters.h"

PyObject *PyR_namedCall(PyObject *self, PyObject *args);

PyObject *PyR_refCall(PyObject *self, PyObject *args);
Rboolean isSForeignReference(PyObject *val);
PyObject *PyR_get(PyObject *self, PyObject *args);


PyObject *PyR_registerConverter(PyObject *self, PyObject *args);


PyObject *RPy_NoSuchFunctionError(char *funcName);

extern void init_R();

PyObject *
RSPython_raiseException()
{
   USER_OBJECT_ val, e;
   char buf[1024];
   PROTECT(e = allocVector(LANGSXP, 1));

   SETCAR(e, Rf_install("geterrmessage"));
   val = R_tryEval(e, R_GlobalEnv, NULL);
   sprintf(buf, "error in calling R: %s", CHAR_DEREF(STRING_ELT(val, 0)));
   PyErr_SetString(PyExc_RuntimeError, buf);
   UNPROTECT(1);

   return NULL;
}


/*

 This handles calling R from Python.


 This code is quickly thrown together for the purposes
 of a) learning about the Python internals and C API,
 and b) to illustrate to others how one might embed
 R in Python or other applications and programming environments.

 There is a lot more to come, specifically the ability
 to be able to pass Python objects to R by "reference"
 and have R operate on these by calling methods in those
 objects that result in calls to Python functions/methods.

 */

static PyMethodDef RMethods[] = {
  {"call", PyR_call, METH_VARARGS},
  {"get", PyR_get, METH_VARARGS},
  {"namedCall", PyR_namedCall, METH_VARARGS},
  {"registerConverter", PyR_registerConverter, METH_VARARGS},
  {NULL, NULL}
};


PyObject *RSPyException;

/*
 Initialize R when running it from within
 Python.
 */
void
initRSInternal(void)
{
 PyObject *m, *dict;
#ifdef RSPYTHON_DEBUG
 fprintf(stderr, "Python API Version: %d, %s\n", PYTHON_API_VERSION, PYTHON_API_STRING);
#endif

 if(R_GlobalEnv == NULL) {
#ifdef RSPYTHON_DEBUG
   fprintf(stderr,"Initializing R\n");fflush(stderr);
#endif
   init_R();
 }
 RPython(registerDefaultConverters)(); 
 m = Py_InitModule("RSInternal", RMethods);

 dict = PyModule_GetDict(m);
 RSPyException = PyErr_NewException("RSInternal.SError", NULL, NULL);
 PyDict_SetItemString(dict, "SError", RSPyException);
}


/*
  This is the routine that implements Python calling
  an S function with a simple, ordered list of arguments
  (i.e.  no named S arguments, etc.).
  This converts the Python arguments into S objects.

  This gets 4 arguments:
    1) the name of the function to call
    2) the un-named arguments as a Tuple
    3) the named arguments (that do not use ``reserved'' words)
    4) a convert argument.
 */
PyObject *
PyR_call(PyObject *self, PyObject *args)
{
 int numArgs;
 USER_OBJECT_ fun, e, val;
 PyObject *result, *name, *targetObject;
 PyObject *orderedArgs = NULL, *namedArgs = NULL;
 char *funcName;
 Rboolean convert;
 int numProtect = 0;
 int errorOccurred = 0;

    orderedArgs = PyTuple_GET_ITEM(args, 1);
    namedArgs = PyTuple_GET_ITEM(args, 2);
    numArgs = PyTuple_GET_SIZE(orderedArgs) + PyDict_Size(namedArgs);

      /* Treat the 4th argument indicating whether to convert the
         result of the call to a Python object or leave as an S
         reference. */
    convert = PyInt_AsLong(PyTuple_GET_ITEM(args, 3));
      /* Treat the 4th argument as a boolean. */
    targetObject = PyTuple_GET_ITEM(args, 4);
   

#ifdef RSPYTHON_DEBUG
fprintf(stderr, "Num arguments %d, convert = %d\n", numArgs, convert);
#endif

   PROTECT(e = allocVector(LANGSXP, numArgs + 1));
   numProtect++;

   if(targetObject == Py_None) {
      name = PyTuple_GET_ITEM(args, 0);
      if(PyString_Check(name)) {
        funcName = PyString_AsString(name);    
	fun = Rf_install((char *)funcName );
#if 0
        fun = Rf_findFun(Rf_install((char *)funcName ),  R_GlobalEnv);
        if(fun == NULL_USER_OBJECT || GET_LENGTH(fun) < 1) {
          RPy_NoSuchFunctionError(funcName);
          return(NULL);
	}
#endif
        SETCAR(e, fun);
      } else if(PyInstance_Check(name)) {
        USER_OBJECT_ sobj;
          sobj = RPython(resolveRForeignReference)(name);
          if(IS_FUNCTION(sobj)) {
             SETCAR(e, sobj);
	  }
      } else {
    	   /* Throw an exception. */
        return(Py_None);
      }
   } else {
     if(isSForeignReference(targetObject)) {
       USER_OBJECT_ sobj, names;
       int i, n;
        sobj = RPython(resolveRForeignReference)(targetObject);
       name = PyTuple_GET_ITEM(args, 0);     
       funcName = PyString_AsString(name);
       names = GET_NAMES(sobj);
       n = GET_LENGTH(names);
       fun = NULL_USER_OBJECT;
       for(i = 0 ; i < n; i++) {
         if(strcmp(CHAR_DEREF(STRING_ELT(names, i)), funcName) == 0) {
           fun = VECTOR_ELT(sobj, i);
           break;
	 }
       }
       SETCAR(e, fun);
     } else {
       return(Py_None);
     }
   }

  if(numArgs > 0) {
    /*
        Add the ordered and named arguments to the
       expression.
     */
    int i, n;
    char *keyName;
    PyObject *arg;
    PyObject *keys, *key, *argVal;

    USER_OBJECT_ tmp = CDR(e);

    n = PyTuple_GET_SIZE(orderedArgs);
    for(i = 0; i < n; i++) {
      arg = PyTuple_GET_ITEM(orderedArgs, i);
      SETCAR(tmp, fromPython(self, arg));
      tmp = CDR(tmp);
    }

    n = PyDict_Size(namedArgs);
    keys = PyMapping_Keys(namedArgs);
    for(i = 0; i < n; i++) {
      key = PyList_GetItem(keys, i);
      keyName = PyString_AsString(key);
      argVal = PyMapping_GetItemString(namedArgs, keyName);

      SETCAR(tmp, fromPython(self, argVal));
      SET_TAG(tmp, mkChar(keyName));
      tmp = CDR(tmp);
    }
  }

 
  /* 
     The way we create the expression means that it is not
    printable and bad things happen. Got to fix this, but
    for the moment, named arguments screw things up.
    Rf_PrintValue(e); 
   */

      /* Evaluate the call to the R function.
         Ignore the return value.
       */
    val = R_tryEval(e, R_GlobalEnv, &errorOccurred);
    if(errorOccurred) {
       UNPROTECT(numProtect);
       return(RSPython_raiseException());
    }
    PROTECT(val);
    numProtect++;

    if(convert) {
      result = toPython(val);
    } else {
      val = RPython(createRAnonymousReference)(val);      
      result = createRPythonReference(val, NULL);     
    }

   UNPROTECT(numProtect);   

  return(result); 
}



/*
  This routine implements the call from Python
  to an S function
 */
PyObject *
PyR_namedCall(PyObject *self, PyObject *args)
{
 int numArgs;
 USER_OBJECT_ fun, e, val;
 PyObject *result, *name, *namedArgs;
 char *funcName;
 int errorOccurred = 0;

    name = PyTuple_GET_ITEM(args, 0);
    namedArgs = PyTuple_GET_ITEM(args, 1);
    numArgs = PyMapping_Length(namedArgs);

   funcName = PyString_AsString(name);    

    fun = Rf_findFun(Rf_install((char *)funcName ),  R_GlobalEnv);
    PROTECT(fun);

    e = allocVector(LANGSXP, numArgs+1);
    PROTECT(e);
    SETCAR(e, fun);

  if(numArgs > 0) {
    int i;
    PyObject *arg, *argVal;
    USER_OBJECT_ tmp = CDR(e);
    char *keyName;
    PyObject *keys = PyMapping_Keys(namedArgs);
    for(i = 0; i < numArgs; i++) {
      arg = PyList_GetItem(keys, i);
      keyName = PyString_AsString(arg);
      argVal = PyMapping_GetItemString(namedArgs, keyName);

      SETCAR(tmp, fromPython(self, argVal));
      SET_TAG(tmp, mkChar(keyName));
      tmp = CDR(tmp);
    }
  }


      /* Evaluate the call to the R function.
         Ignore the return value.
       */
    val = R_tryEval(e, R_GlobalEnv, &errorOccurred);
    if(errorOccurred) {
       return(RSPython_raiseException());
    }
    PROTECT(val);
    result = toPython(val);

   UNPROTECT(2);   

  return(result); 
}

PyObject*
RPy_get(const char *name, const char *mod)
{
  PyObject *module;
  module = PyImport_ImportModule(mod);
  module = PyModule_GetDict(module);
  return(PyDict_GetItemString(module, name));
}


char **
computeCommandLineArgs(int *argc)
{
  char **argv;
  int n, i;
  PyObject *pyArgs;
  pyArgs = RPy_get("__RPythonInitArgs", "__main__");
  
  if(!pyArgs || pyArgs == Py_None) {
      argv = (char **) malloc(sizeof(char *) * 2);
      argv[0] = strdup("Rpython");
      argv[1] = strdup("--silent");
      *argc = 2;
  } else if(PyString_Check(pyArgs)) {
    argv = (char **) malloc( 2 * sizeof(char *));
    *argc = 2;
    argv[0] = strdup("Rpython");
    argv[1] = strdup(PyString_AsString(pyArgs));
  } else if(PyList_Check(pyArgs)) {
    int ctr = 1;
    n = PyList_Size(pyArgs);
    argv = (char **) malloc( (n+1) * sizeof(char *));
    argv[0] = strdup("Rpython");
    *argc = n + 1;
    for(i = 0; i < n; i++) {
	PyObject *el = PyList_GetItem(pyArgs, i);
	if(PyString_Check(el))
	    argv[ctr++] = strdup(PyString_AsString(el));
    }
  }

  return(argv);
}

void
init_R()
{
  char **argv;
  int argc = sizeof(argv)/sizeof(argv[0]);

  argv = computeCommandLineArgs(&argc);

#ifndef WIN32
  Rf_initEmbeddedR(argc, argv);
#else
  PROBLEM "Support for initializing embedded R in Python on Windows not yet available"
      ERROR;
#endif
}


PyObject *RForeignReferenceClass = NULL;

Rboolean
isSForeignReference(PyObject *val)
{
 int ans = 0;

 if(!PyInstance_Check(val))
   return(FALSE);
 

 if(RForeignReferenceClass == NULL) {
  PyObject *module, *d;
  module = PyImport_ImportModule("RReference");
  d = PyModule_GetDict(module);
  RForeignReferenceClass = PyDict_GetItemString(d, "RForeignReference");
 }
 ans = PyClass_IsSubclass((PyObject*) ((PyInstanceObject*)val)->in_class, RForeignReferenceClass);
 return((Rboolean) ans);
}


PyObject *
PyR_get(PyObject *self, PyObject *args)
{
  char *name;
  USER_OBJECT_ e, tmp, val, getFun;
  PyObject *result;
  Rboolean convert = TRUE;
  int errorOccurred = 0;

  PROTECT(e = allocVector(LANGSXP, 2));
  getFun = Rf_findFun(Rf_install("get"),  R_GlobalEnv);
  SETCAR(e, getFun);
  PROTECT(tmp = NEW_CHARACTER(1));
  name = PyString_AsString(PyTuple_GET_ITEM(args, 0));
   SET_STRING_ELT(tmp, 0, COPY_TO_USER_STRING(name));
  SETCAR(CDR(e), tmp);

  PROTECT(val = R_tryEval(e, R_GlobalEnv, &errorOccurred));
  if(errorOccurred) {
     return(RSPython_raiseException());
  }
  if(convert) {
      result = toPython(val);
  } else {
      val = RPython(createRAnonymousReference)(val);      
      result = createRPythonReference(val, NULL);     
  }
  UNPROTECT(2);

  return(result);
}


PyObject *
PyR_registerConverter(PyObject *self, PyObject *args)
{
 ToPythonFunctionConverter *data;
 PyObject *tmp;
 int index;
 char *description = NULL;

  data = (ToPythonFunctionConverter *) calloc(1, sizeof(ToPythonFunctionConverter));

  tmp = PyTuple_GET_ITEM(args, 0);
  data->converter = tmp;
  tmp = PyTuple_GET_ITEM(args, 1);
  data->match = tmp;
 
  tmp = PyTuple_GET_ITEM(args, 2);
  if(tmp != Py_None)
    description = strdup(PyString_AsString(tmp));

  addToPythonConverterInfo(ToPythonConverterFunctionMatch, ToPythonConverterFunction, TRUE, data, description, &index);

  return(PyInt_FromLong((long)index));
}

PyObject *
RPy_NoSuchFunctionError(char *funcName)
{
  PyObject *exc;

  exc = PyErr_NewException("RS.NoSuchFunctionException", (PyObject*) NULL, (PyObject*) NULL);

  PyErr_Format(exc, "No such S function: %s", funcName);

  return(exc);
}

#if 0
int 
jump_now(void)
{
  PyObject *exc;
  fprintf(stderr, "In the local error handler\n");
  exc = PyErr_NewException("RS.NoSuchFunctionException", (PyObject*) NULL, (PyObject*) NULL);
  PyErr_Format(exc, "R error: %x", "Hi there from the R-Python error handler");

  return(-1);
}
#endif

