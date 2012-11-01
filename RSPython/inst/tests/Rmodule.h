#ifndef RMODULE_H
#define RMODULE_H

#include "Python.h"

#ifdef __cplusplus
extern "C" {
#endif

PyObject *PyR_rnorm(PyObject *self, PyObject *args);
PyObject *PyR_test(PyObject *self, PyObject *args);
PyObject *PyR_date(PyObject *self, PyObject *args);


PyObject *PyR_call(PyObject *self, PyObject *args);

#ifdef __cplusplus
}
#endif

#endif
