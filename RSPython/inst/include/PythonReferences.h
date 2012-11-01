#ifndef RS_PYTHON_REFERENCES_H
#define RS_PYTHON_REFERENCES_H


#include "RPythonModule.h"

USER_OBJECT_ RS_getPythonReferenceClass();
USER_OBJECT_ RS_createPythonReference(char *name, PyObject *val);
USER_OBJECT_ registerPythonReference(PyObject *val, PyObject *table);

PyObject *getPythonReference(USER_OBJECT_ sref);
USER_OBJECT_ RS_getPythonReferenceClass(PyObject *val);

#endif

