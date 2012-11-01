#ifndef RPYTHON_REFLECTANCE_H
#define RPYTHON_REFLECTANCE_H

#include "RPythonModule.h"

USER_OBJECT_ RS_INTERNAL_PythonDescribeFunction(PyObject *fun);
USER_OBJECT_ RS_INTERNAL_PythonCodeDescription(PyObject *val);
USER_OBJECT_ RS_INTERNAL_PythonGetClassMethods(PyObject *klass);
USER_OBJECT_ RS_INTERNAL_GetMappingKeys(PyObject *map);

USER_OBJECT_ RS_PygetSuperClasses(USER_OBJECT_ sref);
USER_OBJECT_ RS_Python_getSuperClasses(PyClassObject *k);

PyClassObject *RS_INTERNAL_getPythonClass(USER_OBJECT_ name);

char *RSPy_getPythonTypeName(PyObject *el);
USER_OBJECT_ RSPy_GetMappingTypes(PyObject *map);

PyObject *RS_INTERNAL_getPythonFunction(USER_OBJECT_ fun);
#endif
