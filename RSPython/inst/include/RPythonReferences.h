#ifndef RPYTHON_REFERENCES_H
#define RPYTHON_REFERENCES_H

#include "RPythonModule.h"

USER_OBJECT_ RPython(defaultHandlerFunction)();
USER_OBJECT_ RPython(setDefaultHandlerFunction)(USER_OBJECT_ handler);

USER_OBJECT_ RPython(createRAnonymousReference)(USER_OBJECT_ obj);

PyObject *createRPythonReference(USER_OBJECT_ val, PyObject *func);
PyObject *getPythonReferenceConstructor(const char * className);

USER_OBJECT_ RPython(resolveRForeignReference)(PyObject *val);
USER_OBJECT_ RPython(removeRForeignReference)(PyObject *val);

#endif
