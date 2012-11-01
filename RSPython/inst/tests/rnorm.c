#include "Rmodule.h"

#include "R.h"
#include "Rinternals.h"
#include "Rdefines.h"


SEXP fromPythonTuple(PyObject *self, PyObject *args);


static PyMethodDef RMethods[] = {
  {"rnorm", PyR_rnorm, METH_VARARGS},
  {"test", PyR_test, METH_VARARGS},
  {"date", PyR_date, METH_VARARGS},
  {"call", PyR_call, METH_VARARGS},
  {NULL, NULL}
};


void
initRS(void)
{
 static void *PyR_API[1];
 PyObject *m, *d;
 void init_R();
 init_R();

 m = Py_InitModule("RS", RMethods);

#if 0
 PyR_API[0] = (void*) PyR_rnorm;

 c_api_object = PyCObject_FromVoidPtr((void *) PyR_API, NULL);

 d = PyModule_GetDict(m);
 PyDict_SetItemString(d, "_C_API", c_api_object);
#endif
}


PyObject *
PyR_rnorm(PyObject *self, PyObject *args)
{

 SEXP e;
 SEXP fun;
 SEXP arg;
 SEXP val;
 int i;
 int n, numArgs;
 PyObject *result;

    fun = Rf_findFun(Rf_install((char *)"rnorm"),  R_GlobalEnv);
    PROTECT(fun);
    arg = fromPythonTuple(self, args);
    if(arg == NULL)
	return(NULL);
    PROTECT(arg);

    numArgs = GET_LENGTH(arg);
    e = allocVector(LANGSXP, 1+numArgs);
    PROTECT(e);
    SETCAR(e, fun);
    if(numArgs > 0)
       SETCAR(CDR(e), arg);

      /* Evaluate the call to the R function.
         Ignore the return value.
       */
    val = eval(e, R_GlobalEnv);
    result = toPython(val);
    UNPROTECT(3);   
  return(result); 
}

/*
  Call the date function.
 */
PyObject *
PyR_date(PyObject *self, PyObject *args)
{

 SEXP e;
 SEXP fun, val;
 PyObject *result;

    fun = Rf_findFun(Rf_install((char *)"date"),  R_GlobalEnv);
    PROTECT(fun);
    e = allocVector(LANGSXP, 1);
    PROTECT(e);
    SETCAR(e, fun);
    val = eval(e, R_GlobalEnv);
    result = toPython(val);
    UNPROTECT(2);   
  return(result); 
}


PyObject *
PyR_test(PyObject *self, PyObject *args)
{
 int n = PyList_GET_SIZE(args);
 PyObject *result = Py_BuildValue("i", n);
 return(result);
}

SEXP
fromPythonTuple(PyObject *self, PyObject *args)
{
  SEXP arg;
  int d;
    PROTECT(arg = NEW_INTEGER(1));
    if(!PyArg_ParseTuple(args, "i", &d)) {
      return(NULL);
    }
    INTEGER_DATA(arg)[0] = d;
    UNPROTECT(1);
 return(arg);
}


