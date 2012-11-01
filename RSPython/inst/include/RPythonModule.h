#ifndef RPYTHON_MODULE_H
#define RPYTHONMODULE_H

#include <Python.h>

#include "RSCommon.h"

#define RPython(a) RPython_##a

#ifdef __cplusplus
extern "C" {
#endif

PyObject *toPython(SEXP val);
SEXP fromPython(PyObject *self, PyObject *args);

PyObject *PyR_call(PyObject *self, PyObject *args);

Rboolean RS_checkError();

#ifdef __cplusplus
}
#endif

#endif

