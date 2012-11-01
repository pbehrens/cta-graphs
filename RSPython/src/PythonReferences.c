#include "PythonReferences.h"
#include "PythonReflectance.h"
#include "RCall.h"

#include "compile.h"

#include <unistd.h>

#define USE_NEW_REFERENCES 1

/*
 A Python dictionary for holding the foreign references
 exported to S.
 */
PyObject *ReferenceTable;


/*
 This stores a Python object in the foreign
 reference table and returns an S object that
 identifies it and that can be used in subsequent
 calls.
 */
USER_OBJECT_
registerPythonReference(PyObject *val, PyObject *table)
{
  USER_OBJECT_ sref;
  char buf[10];

#ifndef USE_NEW_REFERENCES
  if(table == NULL) {
    table = ReferenceTable;
    if(table == NULL) {
      table = ReferenceTable = PyDict_New();
    }
  }

  sprintf(buf, "%d", PyDict_Size(table));
  PyDict_SetItemString(table, buf, val);
#endif

  sref = RS_createPythonReference(buf, val);

 return(sref);
}


enum {REF_ID_SLOT, REF_PID_SLOT, REF_CLASS_SLOT};


PyObject *
getPythonReference(USER_OBJECT_ sref)
{
  char *id;
  PyObject *table = ReferenceTable, *val;

#ifdef USE_NEW_REFERENCES
  if(R_ExternalPtrTag(sref) != Rf_install("PythonReference")) {
    PROBLEM "Invalid tag name in external pointer when expecting PythonReference"
    ERROR;
  }
  val = (PyObject *)R_ExternalPtrAddr(sref);
  if(!val) {
	  PROBLEM "NULL value in external pointer when expecting value Python object"
	  WARN;
  }
	

#else
  id = CHAR_DEREF(STRING_ELT(VECTOR_ELT(sref, REF_ID_SLOT), 0));
  val = PyDict_GetItemString(table, id);
#endif

 return(val);
}

void
Rpy_decref(SEXP s)
{
   PyObject *val;
   val = (PyObject *) R_ExternalPtrAddr(s);
   if(val) {
      fprintf(stderr, "finalizing reference %p\n", (void *)val);
      Py_DecRef(val);
#if 0
/*      Py_DecRef(val);*/
      PyGC_Collect(); /* just used for testing */
#endif
   }

}

USER_OBJECT_
RS_createPythonReference(char *name, PyObject *val)
{
 USER_OBJECT_ sref, names, tmp;
#ifdef USE_NEW_REFERENCES

  USER_OBJECT_ classes;
  PROTECT(classes = RS_getPythonReferenceClass(val));
  Py_IncRef(val);
  PROTECT(sref =  R_MakeExternalPtr((void *) val, Rf_install("PythonReference"), R_NilValue));
  R_RegisterCFinalizer(sref, Rpy_decref);
  SET_CLASS(sref, classes);
  UNPROTECT(2);

#else
 char *slotNames[] = {"id", "pid", "class"};
 int numSlots = sizeof(slotNames)/sizeof(slotNames[0]);
 int i;

  PROTECT(sref = NEW_LIST(numSlots));

  SET_VECTOR_ELT(sref, REF_ID_SLOT, tmp = NEW_CHARACTER(1));
   SET_STRING_ELT(tmp, 0, COPY_TO_USER_STRING(name));

  SET_VECTOR_ELT(sref, REF_PID_SLOT, tmp = NEW_NUMERIC(1));
   NUMERIC_DATA(tmp)[0] = getpid();

  SET_VECTOR_ELT(sref, REF_CLASS_SLOT, tmp = NEW_CHARACTER(1));
   SET_STRING_ELT(tmp, 0, COPY_TO_USER_STRING(val->ob_type->tp_name));


  PROTECT(names = NEW_CHARACTER(numSlots));
  for(i = 0; i < numSlots; i++)
    SET_STRING_ELT(names, i, COPY_TO_USER_STRING(slotNames[i]));
  SET_NAMES(sref, names);

  SET_CLASS(sref, RS_getPythonReferenceClass(val));

  UNPROTECT(2);
#endif
  return(sref);
}

USER_OBJECT_
RS_getPythonReferenceClass(PyObject *val)
{
 USER_OBJECT_ klass;
 int  n = 2;
 char *className;

  if(PyInstance_Check(val)) {
    className = PyString_AsString( ((PyInstanceObject*)val)->in_class->cl_name);
  } else if(PyClass_Check(val)) {
    className = "Class";
  } else if(PyType_Check(val)) {
    className = "Type";
  } else if(PyTuple_Check(val)) {
    className = "Tuple";
  } else if(PyList_Check(val)) {
    className = "List";
  } else if(PyInt_Check(val)) {
    className = "Int";
  } else if(PyFloat_Check(val)) {
    className = "Float";
  } else if(PyString_Check(val)) {
    className = "String";
  } else if(PyCode_Check(val)) {
    className = "Code";
  } else if(PyFunction_Check(val)) {
    className = "Function";
  } else
    className = "?";
 
  PROTECT(klass = NEW_CHARACTER(n));
  SET_STRING_ELT(klass, 0, COPY_TO_USER_STRING(className));
  SET_STRING_ELT(klass, 1, COPY_TO_USER_STRING("PythonReference"));
  UNPROTECT(1);

 return(klass);
}


/*

 */
USER_OBJECT_
RS_clearPythonReferences(USER_OBJECT_ names)
{
  USER_OBJECT_ ans = NULL_USER_OBJECT;
  PyObject *table = ReferenceTable;
  PyDict_Clear(table);

 return(ans);
}


USER_OBJECT_
RS_getPythonReferenceIDs()
{
  USER_OBJECT_ ans;
  PyObject *table = ReferenceTable;

  if(table == NULL)
    return(NULL_USER_OBJECT);

  ans = RS_INTERNAL_GetMappingKeys(table);

 return(ans);
}
