#include "PythonFunctionConverters.h"
#include "RPythonReferences.h"

/*
 Here we define a converter from R to Python
 that takes a Python function object and calls it with the
 reference to the R object. It can then extract the information
 it requires from that R object by invoking the RS.call()
 routines and then creating a Python object which it returns.

 As with the C converters, one specifies a pair of Python
 functions: one is the converter that takes a reference to an appropriate R object
 and creates the correspondingPython object, and the second is 
 a matching function that is used to determine if the R object 
 can be handled by the converter function.

 Each of these functions should expect 2 arguments:
   a) a reference to the S object, usually with class
      derived from SForeignReference.
   b) a Python tuple containing the names of the classes
      this S object has. (This is currently the S3 style classes.)

 For "efficiency" reasons, we compute the arguments to these functions
 just once and store them. Then, if the match function returns FALSE
 indicating that the converter cannot handle the S object, then we discard
 these (releaseToPythonFunctionConverterData).
 Otherwise, we store them in the user data of the converter (along with the
 references to the Python match and converter functions) and use
 them in the call to the Python converter function that has been registered.
 When that call returns, we release these arguments.

 Note that it is possible that the Python function will be called
 recursively. For example, when converting an object of class A
 the function might extract an element of class B and then the 
 converters will be called again. Eventhough the converter
 may not be called for this function, the matcher may. Hence, we
 have to store the currently pending state for class A
 and use the ones for B. To do this, we use a stack and move the new
 instance of the arguments on top of the linked list.
*/


Rboolean
ToPythonConverterFunctionMatch(USER_OBJECT_ obj, PyClassObject *pyClass, char *signature, Rboolean isArray, int arrayLen, RSToPythonConverter *converter)
{
  PyObject *pyArgs, *result, *ref;
  PyObject *pyFunc, *pyClassNames;
  USER_OBJECT_ sref;
  int numArgs;
  int i, numClasses;
  Rboolean ans;
  ToPythonFunctionConverter *pyConverter;

#ifdef RSPYTHON_DEBUG
fprintf(stderr,"In converter function match\n");fflush(stderr);
#endif

   pyConverter = (ToPythonFunctionConverter*)converter->userData;

   sref = RPython(createRAnonymousReference)(obj);
   ref = createRPythonReference(sref, NULL);     

   
   numArgs = 2;
   pyArgs = PyTuple_New(numArgs);
   PyTuple_SetItem(pyArgs, 0, ref);  

   /* Now get the class names as a Python tuple. */
   sref = GET_CLASS(obj);
   numClasses = GET_LENGTH(sref);
   pyClassNames = PyTuple_New(numClasses);
   for(i = 0; i < numClasses; i++) {
     PyTuple_SetItem(pyClassNames, 0, PyString_FromString(CHAR(STRING_ELT(sref,i))));
   }
   PyTuple_SetItem(pyArgs, 1, pyClassNames);  

    pyFunc = pyConverter->match;

    result = PyEval_CallObject(pyFunc, pyArgs);  
    if(PyErr_Occurred()) {
       PyErr_Print();
       PyErr_Clear();
       return(FALSE);
    }
    if(!PyInt_Check(result)) {
      fprintf(stderr, "converter match function returned %s\n", result->ob_type->tp_name);
      PROBLEM "converter match function did not return an integer" 
      WARN;
      return(FALSE);
    }
   ans = PyInt_AsLong(result) ? TRUE : FALSE;

   if(ans) {
     ToPythonCurrentFunctionConverter* tmp;
     tmp = (ToPythonCurrentFunctionConverter*) calloc(1, sizeof(ToPythonCurrentFunctionConverter));
     tmp->sref= sref;
     tmp->pyRef = ref;
     tmp->sclassNames = pyClassNames;
     Py_INCREF(tmp->pyRef);
     Py_INCREF(tmp->sclassNames);
     if(pyConverter->data) {
       tmp->next = pyConverter->data;
     }
       pyConverter->data = tmp;

   } else {
     releaseToPythonFunctionConverterData(pyConverter);
   }

#ifdef RSPYTHON_DEBUG
fprintf(stderr,"Finished converter function match %d\n", ans);fflush(stderr);
#endif

  return(ans);
}

/*
 This calls the Python function registered as the one that will convert
 the S object into a Python value, giving it a reference to the S object
 and the names of the classes that it professes to "implement".
 */
PyObject *
ToPythonConverterFunction(USER_OBJECT_ obj, PyClassObject *pyClass, char *signature, Rboolean isArray, int arrayLen, RSToPythonConverter *converter)
{
  PyObject *pyArgs, *result, *ref;
  PyObject *pyFunc;
  int numArgs;
  ToPythonFunctionConverter *pyConverter;
  ToPythonCurrentFunctionConverter *current;

   pyConverter = (ToPythonFunctionConverter*)converter->userData;
   current = pyConverter->data;

   ref = current->pyRef;
   numArgs = 2;
   pyArgs = PyTuple_New(numArgs);
   PyTuple_SetItem(pyArgs, 0, ref);  
   PyTuple_SetItem(pyArgs, 1, current->sclassNames);  

    pyFunc = pyConverter->converter;

    result =  PyEval_CallObject(pyFunc, pyArgs);  
    if(PyErr_Occurred()) {
       PyErr_Print();
       PyErr_Clear();
       return(FALSE);
    }  

    /* Need to free the current data */
  releaseToPythonFunctionConverterData(pyConverter);
   
  return(result);
}

/*
 This is used to pop the arguments off the stack
 */
void
releaseToPythonFunctionConverterData(ToPythonFunctionConverter *el)
{
  ToPythonCurrentFunctionConverter *current;
  current = el->data;
  /*
  RPython(removeRForeignReference)(current->pyRef);
  Py_DECREF(current->sclassNames);
  Py_DECREF(current->pyRef);
  */
  el->data = current ? current->next : NULL;
  free(current);
}
