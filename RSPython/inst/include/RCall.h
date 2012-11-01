#ifndef RS_PYTHON_CALL_H
#define RS_PYTHON_CALL_H

#include "RPythonModule.h"

extern PyObject *PythonInterpreter;

USER_OBJECT_ RPy_callPython(USER_OBJECT_ func, USER_OBJECT_ args, USER_OBJECT_ names, USER_OBJECT_ module, USER_OBJECT_ convert, USER_OBJECT_ isConstructor);
PyObject *RPy_getFunction(USER_OBJECT_ func, PyObject *module, Rboolean *checkBuiltins);
USER_OBJECT_ RS_PythonNewObject(USER_OBJECT_ className, USER_OBJECT_ module, USER_OBJECT_ args, USER_OBJECT_ argNames, USER_OBJECT_ convert);

USER_OBJECT_ RPy_callMethod(USER_OBJECT_ sref, USER_OBJECT_ methodName, USER_OBJECT_ args, USER_OBJECT_ argNames, USER_OBJECT_ convert);

USER_OBJECT_ RS_PythonEval(USER_OBJECT_ cmd, USER_OBJECT_ values, USER_OBJECT_ names, USER_OBJECT_ module, USER_OBJECT_ convert, USER_OBJECT_ isFile);

Rboolean RS_checkError();
PyObject *RS_pythonArgList(USER_OBJECT_ args, USER_OBJECT_ argNames);

#endif
