#ifndef RS_PYTHON_CONVERTERS_H
#define RS_PYTHON_CONVERTERS_H

#include "RPythonModule.h"


PyTypeObject *getListElementType(PyObject *pyobj);
PyObject *RPython(createPythonDictionary)(USER_OBJECT_ val, USER_OBJECT_ names);

USER_OBJECT_ fromPython(PyObject *self, PyObject *pyobj);
PyObject *toPython(USER_OBJECT_ val);

Rboolean isSPrimitive(USER_OBJECT_ val);

#endif

