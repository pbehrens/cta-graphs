#include "RCall.h"
#include "PythonReferences.h"
#include "RPythonConverters.h"

#include "PythonReflectance.h" /* For RSPy_getPythonTypeName() */


PyObject *PythonInterpreter;

/*
  Call Python from R.
  This is also used to instantiate Python objects by calling the 
 */
USER_OBJECT_
RPy_callPython(USER_OBJECT_ func, USER_OBJECT_ args, USER_OBJECT_ names, USER_OBJECT_ module, USER_OBJECT_ convert, USER_OBJECT_ isConstructor)
{
  PyObject *result;
  PyObject *pyModule, *pyFunc;
  USER_OBJECT_ ans = NULL_USER_OBJECT;
  char *moduleName;
  Rboolean isBuiltIn = FALSE;

  if(GET_LENGTH(module)) 
    moduleName = CHAR_DEREF(STRING_ELT(module, 0));
  else
    moduleName = "__main__";

  pyModule = PyImport_ImportModule(moduleName);
  if (pyModule == NULL) {
      PROBLEM "No such module"
      ERROR;
  }

  
  pyFunc = RPy_getFunction(func, pyModule, &isBuiltIn);
  if(pyFunc == NULL) {
    PROBLEM "No object named %s in %s",
      CHAR(STRING_ELT(func, 0)), GET_LENGTH(module) ? CHAR(STRING_ELT(module, 0)) : "__main__"
    ERROR;
  }

  if(LOGICAL_DATA(isConstructor)[0]) {
    if(!PyClass_Check(pyFunc) && !PyType_Check(pyFunc)) {
      PROBLEM "Non-class object used as constructor (actually of type `%s')",
                   	RSPy_getPythonTypeName(pyFunc)
      ERROR;
    }
  }


    /*  If this is a constructor call (i.e. from .PythonNew), then
        call PyInstance_New()
     */
  if((LOGICAL_DATA(isConstructor)[0] && PyClass_Check(pyFunc)) || PyFunction_Check(pyFunc) || PyCFunction_Check(pyFunc)) {
    PyObject *pyArgs;
    pyArgs = RS_pythonArgList(args, names);
    if(LOGICAL_DATA(isConstructor)[0]) {
     PyObject * d = PyDict_New();
     if(PyClass_Check(pyFunc))
	 result = PyInstance_New(pyFunc, pyArgs, d);
     else 
	 result = PyType_GenericNew(pyFunc, pyArgs, d);
    } else {
      if(!isBuiltIn)
        result =  PyEval_CallObject(pyFunc, pyArgs);
      else {
        result =  PyEval_CallObjectWithKeywords(pyFunc, pyArgs, NULL);
      }
    }

/* Don't we need t increment the reference count for result at this point! */
      /*  Py_DECREF(pyArgs);  */
    RS_checkError();
  } else {
       /* Not a function, so just return the value itself. */
    if(GET_LENGTH(args) > 0) {
      PROBLEM "ignoring %d arguments to Python call %s", GET_LENGTH(args),
                                        pyFunc->ob_type->tp_name
      WARN;
    }
    result = pyFunc;
  }

  if(result) {
    if(LOGICAL_DATA(convert)[0])
       ans = fromPython(PythonInterpreter, result);
    else
       ans = registerPythonReference(result, NULL);
  }

  Py_DecRef(result);
  result = NULL;

 return(ans);
}

PyObject *
RPy_getFunction(USER_OBJECT_ func, PyObject *module, Rboolean *checkBuiltIns)
{
  PyObject *function = NULL;
  PyObject *d;

  if(module == NULL)
    module = PyImport_AddModule("__main__");

  d = PyModule_GetDict(module);
  function = PyMapping_GetItemString(d, CHAR(STRING_ELT(func, 0)));

  if(function == NULL && checkBuiltIns) {
    PyErr_Clear(); 
    module = PyImport_AddModule("__builtin__");
    d = PyModule_GetDict(module);
    function = PyMapping_GetItemString(d, CHAR(STRING_ELT(func, 0)));
    if(function && function != Py_None)
      *checkBuiltIns = TRUE;
  }

  return(function);
}



USER_OBJECT_
InitPython()
{
  Py_Initialize();
  RS_checkError();
    /* Force the loading of the RS and hence RSInternal 
       modules. 
     */
  PyImport_ImportModule("RS");
  RS_checkError();
 return(NULL_USER_OBJECT);
}

USER_OBJECT_
TerminatePython()
{
  Py_Finalize();
 return(NULL_USER_OBJECT);
}

USER_OBJECT_
RS_PythonIsInitialized()
{
 USER_OBJECT_ ans =  NEW_LOGICAL(1);
 
 LOGICAL_DATA(ans)[0] = Py_IsInitialized();

 return(ans);
}


/*

 */
USER_OBJECT_
RS_PythonEval(USER_OBJECT_ cmd, USER_OBJECT_ values, USER_OBJECT_ names, USER_OBJECT_ module, USER_OBJECT_ convert, USER_OBJECT_ isFile)
{
  PyObject *pyModule, *result;
  PyObject *d, *locals;
  USER_OBJECT_ ans = NULL_USER_OBJECT;
  if(GET_LENGTH(module)) {
    pyModule = PyImport_ImportModule(CHAR_DEREF(STRING_ELT(module, 0)));
    if (pyModule == NULL) {
      PROBLEM "No such Python module %s", CHAR_DEREF(STRING_ELT(module, 0))
      ERROR;
    }
  } else
    pyModule = PyImport_ImportModule("__main__");

  d = PyModule_GetDict(pyModule);

  if(GET_LENGTH(values)) {
    locals = RPython(createPythonDictionary)(values, names);
  } else
    locals = d;

  if(LOGICAL_DATA(isFile)[0]) {
    FILE *fp;
    char *name = CHAR_DEREF(STRING_ELT(cmd, 0));
    fp = fopen(name, "r"); 
    if(fp == NULL) {
      PROBLEM "Cannot open file %s to evaluate in Python", 
               name
     ERROR;
    }
    result = PyRun_File(fp, name,  Py_eval_input, d, locals);
    fclose(fp);
  }
  else {
      /* Use single_input rather than eval_input*/
    result = PyRun_String(CHAR_DEREF(STRING_ELT(cmd, 0)), Py_single_input, d, locals);
  }
  RS_checkError();

  if(result) {
    if(LOGICAL_DATA(convert)[0])
       ans = fromPython(PythonInterpreter, result);
    else
       ans = registerPythonReference(result, NULL);
  }
 return(ans);
}


Rboolean
RS_checkError() 
{
  if(PyErr_Occurred()) {
      int clear = 1;
      char buf[1000];
      PyObject *exception, *v, *tb;
      PyErr_Fetch(&exception, &v, &tb);
      PyErr_NormalizeException(&exception, &v, &tb);

        /* Handle the old-style exceptions which are just simple strings. */
      if(PyString_Check(exception)) {
	 sprintf(buf,"%s", PyString_AsString(exception));
      } else {
	  /* Otherwise, we have a real exception and we can get its
             error message via __str__()
             Are all exceptions guaranteed to have this method?
           */
	PyObject *m = PyObject_GetAttrString(v, "__str__");
	PyObject *args = PyTuple_New(0);
        PyObject *str = PyEval_CallObject(m, args);

	if(str && PyString_Check(str)) {
	    sprintf(buf, "%s", PyString_AsString(str));
	} else
	    sprintf(buf, "%s", "A Python Exception Occurred.");
      }
      PROBLEM "Error in Python call: %s", buf
	  ERROR;
    return(TRUE);
  }

 return(FALSE);
}


USER_OBJECT_
RPy_getAttr(USER_OBJECT_ sref, USER_OBJECT_ name, USER_OBJECT_ convert)
{
 PyObject *pinst, *result;
 USER_OBJECT_ ans = NULL_USER_OBJECT;

    pinst = getPythonReference(sref);
    if(!pinst) {
      PROBLEM "Python Reference is no longer valid"
      ERROR;
    }
    result = PyObject_GetAttrString(pinst, CHAR_DEREF(STRING_ELT(name, 0)));

    if(result) {
	if(LOGICAL_DATA(convert)[0])
	    ans = fromPython(PythonInterpreter, result);
        else
	    ans = registerPythonReference(result, NULL);
    }

    return(ans);
}

/*
  Call a method of a Python object and return the result as an R object.
 */
USER_OBJECT_
RPy_callMethod(USER_OBJECT_ sref, USER_OBJECT_ methodName, USER_OBJECT_ args, USER_OBJECT_ argNames, USER_OBJECT_ convert)
{
 PyObject *pinst;
 PyObject *pmethod, *pargs, *result;
 USER_OBJECT_ ans = NULL_USER_OBJECT;

  pinst = getPythonReference(sref);
  pmethod = PyObject_GetAttrString(pinst, CHAR_DEREF(STRING_ELT(methodName, 0)));
  if(pmethod == NULL) {
    PROBLEM "No such method or attribute %s in Python object",
        CHAR_DEREF(STRING_ELT(methodName, 0))
    ERROR;
  }

  pargs = RS_pythonArgList(args, argNames);
  result = PyEval_CallObject(pmethod, pargs);

  if(result) {
    if(LOGICAL_DATA(convert)[0])
       ans = fromPython(PythonInterpreter, result);
    else
       ans = registerPythonReference(result, NULL);
  }

 return(ans);
}


/*
  Create a Python list containing the elements
  of the S list, having converted each to its
  corresponding Python type.
 */
PyObject *
RS_pythonArgList(USER_OBJECT_ args, USER_OBJECT_ argNames)
{
 PyObject *pyArgs = NULL;
 int i, numArgs;
    numArgs = GET_LENGTH(args);
    pyArgs = PyTuple_New(numArgs);
     /*  Py_INCREF(pyArgs);  */
    for(i = 0; i < numArgs; i++) {
      PyTuple_SetItem(pyArgs, i, toPython(VECTOR_ELT(args, i)));  
    }

 return(pyArgs);
}

