#include "PythonReflectance.h"
#include "PythonReferences.h"

#include <compile.h>       /* PyRun interfaces   */


USER_OBJECT_
RS_Python_describeFunction(USER_OBJECT_ fun)
{
  PyObject *pyFun = RS_INTERNAL_getPythonFunction(fun);

  if(pyFun && PyFunction_Check(pyFun)) {
    return(RS_INTERNAL_PythonDescribeFunction(pyFun));
  } else {
    PROBLEM "No such function"
    ERROR;
  }

  return(NULL_USER_OBJECT);
}


PyObject *
RS_INTERNAL_getPythonFunction(USER_OBJECT_ fun)
{
  PyObject *val;
  if(IS_CHARACTER(fun)) {
   PyObject *module, *d;
   char *moduleName = "__main__";
   int n = GET_LENGTH(fun);
    if(n > 1) { 
      moduleName = CHAR_DEREF(STRING_ELT(fun, n-1));
    }

    module = PyImport_ImportModule(moduleName);
    if(module == NULL) { 
     PROBLEM "No such module %s", moduleName
     ERROR;
    }
    
   d = PyModule_GetDict(module);
   val = PyMapping_GetItemString(d, CHAR_DEREF(STRING_ELT(fun, 0)));    
  } else {
   val = getPythonReference(fun);
  }

   if(val == NULL || !PyFunction_Check(val)) {
     PROBLEM "Python reference doesn't identify valid Python Function Object"
     ERROR;
   }

 return(val);
}

enum {FUN_NAME_SLOT, FUN_CODE_SLOT, FUN_DEFAULTS_SLOT, FUN_DOC_SLOT};

/*

 */
USER_OBJECT_
RS_INTERNAL_PythonDescribeFunction(PyObject *val)
{
 int i;
 USER_OBJECT_ ans, names, tmp;
 int numSlots;
 char *slotNames[] = {"name", "code", "defaults", "doc"};
 PyFunctionObject *fun = (PyFunctionObject *)val;
 PyObject *pyTmp;

 if(val == Py_None)
   return(NULL_USER_OBJECT);

 numSlots = sizeof(slotNames)/sizeof(slotNames[0]);
 PROTECT(ans = NEW_LIST(numSlots));

 if(fun->ob_type->tp_getattr)
     pyTmp = fun->ob_type->tp_getattr(val, "__name__");
 else
     pyTmp = fun->ob_type->tp_getattro(val, PyString_FromString("__name__")); 
 SET_VECTOR_ELT(ans, FUN_NAME_SLOT, tmp = NEW_CHARACTER(1));
 if(pyTmp)
   SET_STRING_ELT(tmp, 0, COPY_TO_USER_STRING(PyString_AsString(pyTmp)));

 if(fun->ob_type->tp_getattr)
     pyTmp = fun->ob_type->tp_getattr(val, "__doc__");
 else
     pyTmp = fun->ob_type->tp_getattro(val, PyString_FromString("__doc__")); 
 SET_VECTOR_ELT(ans, FUN_DOC_SLOT, tmp = NEW_CHARACTER(1));
 if(pyTmp) {
  char *text = PyString_AsString(pyTmp);
  if(text)
  SET_STRING_ELT(tmp, 0, COPY_TO_USER_STRING(text));
 }

 pyTmp = PyFunction_GetDefaults(val);

#if 0
 pyTmp = PyFunction_GetGlobals(val);
 if(pyTmp) {
   SET_VECTOR_ELT(ans, FUN_GLOBALS_SLOT, RS_INTERNAL_GetMappingKeys(pyTmp));
 }
#endif

 pyTmp = PyFunction_GetCode(val);
 if(pyTmp) {
   SET_VECTOR_ELT(ans,FUN_CODE_SLOT, RS_INTERNAL_PythonCodeDescription(pyTmp));
 } 

  PROTECT(names = NEW_CHARACTER(numSlots));
  for(i = 0; i < numSlots; i++)
    SET_STRING_ELT(names, i, COPY_TO_USER_STRING(slotNames[i]));
  SET_NAMES(ans, names);

/*
  SET_CLASS(ans, RS_getPythonReferenceClass((PyObject*)fun));
*/
  PROTECT(tmp = NEW_CHARACTER(1));
  SET_STRING_ELT(tmp, 0, COPY_TO_USER_STRING("PythonFunctionDescription"));
  SET_CLASS(ans, tmp);


 UNPROTECT(3);

 return(ans);
}



/*
 This returns a description of a Python PyCodeObject
 by giving the number of arguments and local variables
 and the names of those arguments.

 More work needed for *name and **name arguments.
 */
enum { CODE_NUM_VARS_SLOT, CODE_VARNAMES_SLOT};

USER_OBJECT_
RS_INTERNAL_PythonCodeDescription(PyObject *val)
{ 
  PyCodeObject *code = (PyCodeObject *)val;
  PyObject *pyNames;
  USER_OBJECT_ tmp, ans, names, numNames;
  int numArgs, i;
  int numSlots;
  char *slotNames[] = {"numArgs", "names"};

  if(!PyCode_Check(val))
    return(NULL_USER_OBJECT);

  numSlots = sizeof(slotNames)/sizeof(slotNames[0]);  

  PROTECT(ans = NEW_LIST(numSlots));
  
  SET_VECTOR_ELT(ans,  CODE_NUM_VARS_SLOT, tmp = NEW_INTEGER(2));
  PROTECT(numNames = NEW_CHARACTER(2));
  INTEGER_DATA(tmp)[0] = code->co_argcount;
  INTEGER_DATA(tmp)[1] = code->co_nlocals;
  SET_STRING_ELT(numNames, 0, COPY_TO_USER_STRING("External"));
  SET_STRING_ELT(numNames, 1, COPY_TO_USER_STRING("Local"));
  SET_NAMES(tmp, numNames);

  pyNames = code->co_varnames;
  numArgs = PyTuple_Size(pyNames);
  SET_VECTOR_ELT(ans, CODE_VARNAMES_SLOT, tmp = NEW_CHARACTER(numArgs));
  for(i = 0 ; i < numArgs; i++) {
    char *text;
    PyObject *pyTmp = PyTuple_GET_ITEM(pyNames, i);
    if(pyTmp) {
      text = PyString_AsString(pyTmp);
      if(text) 
        SET_STRING_ELT(tmp, i, COPY_TO_USER_STRING(text));   
    }
  }

  PROTECT(names = NEW_CHARACTER(numSlots));
  for(i = 0; i < numSlots; i++)
    SET_STRING_ELT(names, i, COPY_TO_USER_STRING(slotNames[i]));
  SET_NAMES(ans, names);


  PROTECT(tmp = NEW_CHARACTER(1));
  SET_STRING_ELT(tmp, 0, COPY_TO_USER_STRING("PythonParameterListDescription"));
  SET_CLASS(ans, tmp);

  UNPROTECT(4);
 
 return(ans);
}


USER_OBJECT_
RS_INTERNAL_PythonGetClassMethods(PyObject *klass)
{
 USER_OBJECT_ ans, names;
 PyObject *d, *pyNames;
 int n, i;

 d  = PyObject_GetAttrString(klass, "__dict__");

 n = PyDict_Size(d);
 pyNames = PyDict_Keys(d);

 PROTECT(names = NEW_CHARACTER(n));
 PROTECT(ans = NEW_LIST(n));
 for(i = 0 ;  i < n; i++) {
  PyObject *fun, *el;
   el = PyList_GetItem(pyNames, i);
   fun = PyDict_GetItem(d, el);
   if(PyFunction_Check(fun))
     SET_VECTOR_ELT(ans, i, RS_INTERNAL_PythonDescribeFunction(fun));
   SET_STRING_ELT(names, i, COPY_TO_USER_STRING(PyString_AsString(el))); 
 }
 if(n > 0)
   SET_NAMES(ans, RS_INTERNAL_GetMappingKeys(d));
 UNPROTECT(2);

 return(ans);
}


/*
 Need to walk the inheritance hierarchy to get all the methods
 all is a logical value indicating whether this is desired.
 */
USER_OBJECT_
RS_PythonGetMethods(USER_OBJECT_ sref, USER_OBJECT_ all)
{ 
 USER_OBJECT_ ans;
 PyObject *klass;

 klass =(PyObject *) RS_INTERNAL_getPythonClass(sref);

 if(LOGICAL_DATA(all)[0] == FALSE) 
    ans = RS_INTERNAL_PythonGetClassMethods(klass);
 else {
   int i, n;
   PyClassObject *k = (PyClassObject *)klass;
   PyObject *tmp;
   char *txt;
   USER_OBJECT_ names;

   n = PyTuple_Size(k->cl_bases);

   PROTECT(ans = NEW_LIST(n+1));
   PROTECT(names = NEW_CHARACTER(n+1));
   SET_VECTOR_ELT(ans, 0, RS_INTERNAL_PythonGetClassMethods(klass));
   txt = PyString_AsString(k->cl_name);
   SET_STRING_ELT(names, 0, COPY_TO_USER_STRING(txt));
   for(i=0; i < n; i++) {
     tmp = PyTuple_GET_ITEM(k->cl_bases, i);
     SET_VECTOR_ELT(ans, i+1, RS_INTERNAL_PythonGetClassMethods(tmp));
     txt = PyString_AsString(((PyClassObject*)tmp)->cl_name);
     SET_STRING_ELT(names, i+1, COPY_TO_USER_STRING(txt));
   }
   SET_NAMES(ans, names);
   UNPROTECT(2);
 }

 return(ans);
}

USER_OBJECT_
RS_PygetSuperClasses(USER_OBJECT_ sref)
{
 PyObject *klass;
 
 klass = (PyObject*)RS_INTERNAL_getPythonClass(sref);

 return(RS_Python_getSuperClasses((PyClassObject*) klass));
}


/*
  This traverses the super-classes, building a structure
  containing.

  Returns the name of the class or
  a list containing 1) the name of the class
  and 2) a list of super-classes processed in the same
  way. 

 */
USER_OBJECT_
RS_Python_getSuperClasses(PyClassObject *k)
{
 int i, n;
 char *txt;
 PyObject *pyTmp;
 USER_OBJECT_ ans, els, tmp, names;  

   n = PyTuple_Size(k->cl_bases);

   if(n == 0) {
     PROTECT(tmp = NEW_CHARACTER(1));
     txt = PyString_AsString(k->cl_name);   
     SET_STRING_ELT(tmp, 0, COPY_TO_USER_STRING(txt));
     UNPROTECT(1);
     return(tmp);
   }
 
   PROTECT(ans = NEW_LIST(2));
   SET_VECTOR_ELT(ans, 0, tmp = NEW_CHARACTER(1));
   txt = PyString_AsString(k->cl_name);   
   SET_STRING_ELT(tmp, 0, COPY_TO_USER_STRING(txt));



   PROTECT(els = NEW_LIST(n));
   PROTECT(names = NEW_CHARACTER(n));
   for(i=0; i < n; i++) {
     pyTmp = PyTuple_GET_ITEM(k->cl_bases, i);
     SET_VECTOR_ELT(els, i, RS_Python_getSuperClasses((PyClassObject*) pyTmp));
     txt = PyString_AsString(((PyClassObject*)pyTmp)->cl_name);
     SET_STRING_ELT(names, i, COPY_TO_USER_STRING(txt));
   }
   SET_NAMES(els, names);

   SET_VECTOR_ELT(ans, 1, els);

   PROTECT(names = NEW_CHARACTER(2));
   SET_STRING_ELT(names, 0, COPY_TO_USER_STRING("ClassName"));
   SET_STRING_ELT(names, 1, COPY_TO_USER_STRING("SuperClasses"));
   SET_NAMES(ans, names);

   UNPROTECT(4);
 return(ans);
}


/*
 We are trying to get the methods of an object.
 There are two reasons for this. 1) we can call these
 methods directly; 2) we can define a new class that extends
 that of the object and overrides its methods by calling R 
 functions to implement the methods.
 This allows us to use R objects.
 */
USER_OBJECT_
RS_PythonGetMethodNames(USER_OBJECT_ sref)
{ 
 USER_OBJECT_ ans;
 PyObject *d, *klass;
 PyObject *val = getPythonReference(sref);
 if(val == NULL)
   return(NULL_USER_OBJECT);

 klass = PyObject_GetAttrString(val, "__class__");
 d  = PyObject_GetAttrString(klass, "__dict__");

 ans = RS_INTERNAL_GetMappingKeys(d);
 return(ans);
}


USER_OBJECT_
RS_INTERNAL_GetMappingKeys(PyObject *map)
{
 USER_OBJECT_ names;
 PyObject *pyNames, *el;
 int n, i;

 if(PyDict_Check(map)) {
    n = PyDict_Size(map);
    pyNames = PyDict_Keys(map);
 } else {
    n = PyMapping_Length(map);
    pyNames = PyMapping_Keys(map);
 }

 PROTECT(names = NEW_CHARACTER(n));
 for(i = 0; i < n ; i++) {
   el = PyList_GetItem(pyNames, i);
   SET_STRING_ELT(names, i, COPY_TO_USER_STRING(PyString_AsString(el))); 
 }

 UNPROTECT(1);
 return(names);
}


USER_OBJECT_
RS_PythonGetModuleNames(USER_OBJECT_ module)
{
  PyObject *m, *d;
   m = PyImport_ImportModule(CHAR_DEREF(STRING_ELT(module, 0)));
   if (m == NULL) {
     PROBLEM "no Python module named %s found",
                   CHAR_DEREF(STRING_ELT(module, 0))
     ERROR;
   }
   d = PyModule_GetDict(m); 
 return(RS_INTERNAL_GetMappingKeys(d));
}


USER_OBJECT_
RS_PythonGetModuleTypes(USER_OBJECT_ module)
{
  PyObject *m, *d;
   m = PyImport_ImportModule(CHAR(STRING_ELT(module, 0)));
   if (m == NULL) {
     PROBLEM  "Can't locate Python module `%s'", CHAR(STRING_ELT(module, 0))
     ERROR;
     return(NULL_USER_OBJECT);
   }

   d = PyModule_GetDict(m); 
 return(RSPy_GetMappingTypes(d));
}




PyClassObject *
RS_INTERNAL_getPythonClass(USER_OBJECT_ name)
{
  int n =  GET_LENGTH(name);
  PyObject *pyModule;
  PyObject *klass = NULL;
  PyObject *d;
  char *moduleName;

 if(!IS_CHARACTER(name)) {
   PyObject *val;
   val = getPythonReference(name);
   if(val == NULL) {
     PROBLEM "Python reference doesn't identify valid Python Object"
     ERROR;
   }

   if(!PyClass_Check(val)) {
     if(PyInstance_Check(val)) {
       klass = PyObject_GetAttrString(val, "__class__");
     }
   } else
     klass = val;
   return((PyClassObject*) klass);
 }


  if(n > 1) {
    moduleName = CHAR_DEREF(STRING_ELT(name, 1));
  } else {
    moduleName = "__main__";
  }
   pyModule = PyImport_ImportModule(moduleName);
   if (pyModule == NULL) {
      PROBLEM "No such module %s", moduleName
      ERROR;
   }

  d = PyModule_GetDict(pyModule);
  klass = PyMapping_GetItemString(d, CHAR(STRING_ELT(name, 0)));

   if(!klass) {
     PROBLEM "No such class"
     ERROR;
   }

  return((PyClassObject *) klass);
}




USER_OBJECT_
RSPy_GetMappingTypes(PyObject *map)
{
 USER_OBJECT_ names, types = NULL_USER_OBJECT;
 PyObject *pyNames, *el;
 int n, i;
 Rboolean isDict = PyDict_Check(map);

 if(isDict) {
    n = PyDict_Size(map);
    pyNames = PyDict_Keys(map);
 } else {
    n = PyMapping_Length(map);
    pyNames = PyMapping_Keys(map);
 }

 if(n > 0) {
   PROTECT(types = NEW_CHARACTER(n));
   PROTECT(names = NEW_CHARACTER(n));
   for(i = 0; i < n ; i++) {
     el = PyList_GetItem(pyNames, i);
     SET_STRING_ELT(names, i, COPY_TO_USER_STRING(PyString_AsString(el))); 
     if(isDict)
       el = PyDict_GetItem(map, el);
     else {
       el = PyMapping_GetItemString(map, PyString_AsString(el));
     }
     SET_STRING_ELT(types, i, COPY_TO_USER_STRING(RSPy_getPythonTypeName(el))); 
   }
   
   SET_NAMES(types, names);
   UNPROTECT(2);
 }

 return(types);
}

char *
RSPy_getPythonTypeName(PyObject *el)
{
  return(el->ob_type->tp_name);
}
