#ifndef PYTHONFUNCTION_CONVERTERS_H
#define PYTHONFUNCTION_CONVERTERS_H

#include "UserConverters.h"

typedef struct _ToPythonCurrentFunctionConverter {
  USER_OBJECT_  sref;
  PyObject     *sclassNames;
  PyObject     *pyRef;

    /* Alow recursion by having a last in, first element style
       stack.
     */
  struct _ToPythonCurrentFunctionConverter *next;
} ToPythonCurrentFunctionConverter;


typedef struct {
 
  PyObject     *match;
  PyObject     *converter;
  ToPythonCurrentFunctionConverter *data;

} ToPythonFunctionConverter;


Rboolean ToPythonConverterFunctionMatch(USER_OBJECT_ obj, PyClassObject *pyClass, char *signature, Rboolean isArray, int arrayLen, RSToPythonConverter *converter);
PyObject *ToPythonConverterFunction(USER_OBJECT_ obj, PyClassObject *pyClass, char *signature, Rboolean isArray, int arrayLen, RSToPythonConverter *converter);

void releaseToPythonFunctionConverterData(ToPythonFunctionConverter *el);


#endif
