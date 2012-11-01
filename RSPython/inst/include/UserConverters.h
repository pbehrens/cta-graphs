#ifndef RSPYTHON_USER_CONVERTERS_H
#define RSPYTHON_USER_CONVERTERS_H

#include "RPythonModule.h"
/*
  The following are related to User-level or extensible converters.
  They are modelled on and copied from the Converters.cweb file in the 
  RS-Java package and follow the same basic principles.
 */

typedef struct _RSFromPythonConverter RSFromPythonConverter;

typedef Rboolean (*FromPythonConverterMatch)(PyObject *obj, PyClassObject *pyClass, RSFromPythonConverter *converter);

typedef USER_OBJECT_ (*FromPythonConverter)(PyObject *obj, PyClassObject *pyClass, RSFromPythonConverter *converter);

typedef FromPythonConverterMatch FromTargetConverterMatch;
typedef FromPythonConverter      FromTargetConverter;


typedef struct _RSToPythonConverter RSToPythonConverter;

struct _RSFromPythonConverter {
  FromPythonConverterMatch match;
  FromPythonConverter      converter;
  Rboolean                 autoArray;
  void                     *userData;
  char                     *description;
  RSFromPythonConverter    *next;
};
 
/*
In RS-Java, have a java_type here.
 */
typedef Rboolean (*ToPythonConverterMatch)(USER_OBJECT_ obj, PyClassObject *pyClass, char *signature, Rboolean isArray, int arrayLen, RSToPythonConverter *);

typedef PyObject* (*ToPythonConverter)(USER_OBJECT_ obj, PyClassObject *pyClass, char *signature, Rboolean isArray, int arrayLen, RSToPythonConverter *);

typedef ToPythonConverterMatch ToTargetConverterMatch;
typedef ToPythonConverter      ToTargetConverter;


struct _RSToPythonConverter {

  ToPythonConverterMatch match;
  ToPythonConverter      converter;
  Rboolean               autoArray;   
  void                  *userData;

  char                  *description;  

  RSToPythonConverter *next;
};    


#define ToPythonConverters   ToTargetConverters
#define FromPythonConverters FromTargetConverters

extern RSFromPythonConverter *FromPythonConverters;
extern RSToPythonConverter   *ToPythonConverters;


USER_OBJECT_ userLevelFromPythonConversion(PyObject *, Rboolean *ok);
PyObject *userLevelToPythonConversion(USER_OBJECT_ val, Rboolean *ok);


#define addFromPythonConverter addFromTargetConverter
#define addFromPythonConverterInfo addFromTargetConverterInfo

#define addToPythonConverter addToTargetConverter
#define addToPythonConverterInfo addToTargetConverterInfo

int addFromPythonConverter(RSFromPythonConverter *);
RSFromPythonConverter * addFromPythonConverterInfo(FromPythonConverterMatch match, FromPythonConverter converter, Rboolean autoArray, void *userData, char *description, int *index);

/* Done. */
int addToPythonConverter(RSToPythonConverter *);
RSToPythonConverter * addToPythonConverterInfo(ToPythonConverterMatch match, ToPythonConverter converter, Rboolean autoArray, void *userData, char *description, int *index);

RSFromPythonConverter *removeFromPythonConverterByIndex(int which);
RSFromPythonConverter *removeFromPythonConverterByDescription(char *desc, int *which);

USER_OBJECT_ RPython(removeConverter)(USER_OBJECT_ id, USER_OBJECT_ fromPython);


int getNumConverters(int which);
USER_OBJECT_ RPython(getNumConverters)(USER_OBJECT_ which);
USER_OBJECT_ RPython(getConverterDescriptions)(USER_OBJECT_ which);
char *fromPythonConverterDescription(RSFromPythonConverter *cvt);

/*
 The basic matching functions.
 ExactClassMatch, Derived From.
 */
void RPython(registerDefaultConverters)();

#endif
