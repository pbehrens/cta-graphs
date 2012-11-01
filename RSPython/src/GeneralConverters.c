#include "UserConverters.h"

typedef RSFromPythonConverter RSFromTargetConverter;
typedef RSToPythonConverter   RSToTargetConverter;

RSFromTargetConverter *FromTargetConverters;
RSToTargetConverter   *ToTargetConverters;

int addToTargetConverter(RSToTargetConverter *cvt);

#define fromPythonConverterDescription fromTargetConverterDescription
char *fromTargetConverterDescription(RSFromTargetConverter *cvt);

RSFromTargetConverter *
removeFromTargetConverterByIndex(int which)
{
 int ctr = 0;
 RSFromTargetConverter *tmp, *prev = NULL;
   tmp = FromTargetConverters;

   if(which == 0) {
       tmp = FromTargetConverters;
       FromTargetConverters = FromTargetConverters->next;
       return(tmp);
   }
 
   while(tmp != NULL && ctr++ < which) {
      prev = tmp;
      tmp = tmp->next;
   }

   if(tmp == NULL)
      return((RSFromTargetConverter*) NULL);

     /* Now drop this element. */
   prev->next = tmp->next;

 return(tmp);
}




RSToTargetConverter *
removeToTargetConverterByIndex(int which)
{
 int ctr = 0;
 RSToTargetConverter *tmp, *prev = NULL;
   tmp = ToTargetConverters;

   if(which == 0) {
       tmp = ToTargetConverters;
       ToTargetConverters = ToTargetConverters->next;
       return(tmp);
   }
 
   while(tmp != NULL && ctr++ < which) {
      prev = tmp;
      tmp = tmp->next;
   }

   if(tmp == NULL)
      return((RSToTargetConverter*) NULL);

     /* Now drop this element. */
   prev->next = tmp->next;

 return(tmp);
}


RSFromTargetConverter *
removeFromTargetConverterByDescription(char *desc, int *which)
{
 int ctr = 0;
 RSFromTargetConverter *tmp, *prev = NULL;
 char *tmpDesc; 
   tmp = FromTargetConverters;

   while(tmp != NULL) {
      tmpDesc = fromPythonConverterDescription(tmp);
      if(tmpDesc != NULL && strcmp(tmpDesc, desc) ==0) {
             /* Now drop this element. */
        if(prev)
          prev->next = tmp->next;
        else
          FromTargetConverters = tmp->next;

        if(which)
          *which = ctr;
        return(tmp);
      }
      prev = tmp;
      tmp = tmp->next;
      ctr++;
   }

    return((RSFromTargetConverter*) NULL);
}


USER_OBJECT_ 
RPython(removeConverter)(USER_OBJECT_ id, USER_OBJECT_ fromPython)
{
  USER_OBJECT_ ans;
  int ok = -1, i;
  int *which;
  char *desc = NULL;
  char *descString;
  USER_OBJECT_ names;
  int n = GET_LENGTH(id);

  which = INTEGER_DATA(id);
  
  PROTECT(ans = NEW_INTEGER(n));  
  PROTECT(names = NEW_CHARACTER(n));

  for(i = 0; i < n; i++) { 
	  ok = -1;
	  if(LOGICAL_DATA(fromPython)[0] != 0) {
	      RSFromTargetConverter *el;
	      el = removeFromTargetConverterByIndex(which[i]);

	      if(el != NULL) {
		 descString = fromPythonConverterDescription(el);
		 if(descString != NULL)
		    SET_STRING_ELT(names, i, COPY_TO_USER_STRING(descString));
		 free(el); /* Need to free all of its elements also. */
		 ok = which[i];
	      }
	  } else {
	      RSToTargetConverter *el;
	      el = removeToTargetConverterByIndex(which[i]);
	      if(el != NULL) {
    	          descString = el->description;
		  if(descString != NULL)
	    	     SET_STRING_ELT(names, i, COPY_TO_USER_STRING(descString));
		  free(el);
		  ok = which[i];       
	      }
	  }
	  INTEGER_DATA(ans)[i] = ok;
  }
  SET_NAMES(ans, names);

  UNPROTECT(2);
  
  return(ans);
}


int
getNumConverters(int which)
{
 int ctr = 0;
 if(which == 0) {
      RSFromTargetConverter *tmp = FromTargetConverters;
      while(tmp) {
        ctr++;
        tmp = tmp->next;
      }
 } else {
      RSToTargetConverter *tmp = ToTargetConverters;
      while(tmp) {
        ctr++;
        tmp = tmp->next;
      }
 }

 return(ctr);
}

USER_OBJECT_
RPython(getNumConverters)(USER_OBJECT_ which)
{
 USER_OBJECT_ ans;
  ans = NEW_INTEGER(1);
 
  INTEGER_DATA(ans)[0] = getNumConverters(LOGICAL_DATA(which)[0]);
 return(ans);
}

USER_OBJECT_
RPython(getConverterDescriptions)(USER_OBJECT_ which)
{
  int type = LOGICAL_DATA(which)[0];
  int n, i;
  const char *d;
  USER_OBJECT_ ans;
  RSFromTargetConverter *tmp0 = FromTargetConverters;
  RSToTargetConverter *tmp1 = ToTargetConverters;
  
  n = getNumConverters(type);

  PROTECT(ans = NEW_CHARACTER(n));
   for(i = 0; i < n ; i++) {
    if(type == 0) {
      d = tmp0->description;
      if(!d) {
         d = fromTargetConverterDescription(tmp0);
      }
      tmp0 = tmp0->next;      
    } else {
      d = tmp1->description;
      tmp1 = tmp1->next;
    }
    
    if(d)    
     SET_STRING_ELT(ans, i, COPY_TO_USER_STRING(d));
   }

  UNPROTECT(1);

 return(ans); 
}    


char *
fromTargetConverterDescription(RSFromTargetConverter *cvt)
{
  char *ans = NULL;

  if(cvt->description)
    return(cvt->description);

  return(ans);
}    



/*
 registerConverter
 */


int
addFromTargetConverter(RSFromTargetConverter *el)
{
 int ctr = 0;
  el->next = NULL;
  if(FromTargetConverters == NULL) {
    FromTargetConverters = el;
  } else { 
    RSFromTargetConverter *tmp;
    tmp = FromTargetConverters;
    while(tmp->next) {
     ctr++;
     tmp = tmp->next;
    }

    tmp->next = el;
    ctr++;
  }

 return(ctr);
}


RSFromPythonConverter * 
addFromPythonConverterInfo(FromPythonConverterMatch match, FromPythonConverter converter, 
                            Rboolean autoArray, void *userData, char *description, int *index)
{
 int pos;
 RSFromTargetConverter *cvt = (RSFromTargetConverter*) malloc(sizeof(RSFromTargetConverter)); 
  cvt->match = match;
  cvt->converter = converter;
  cvt->autoArray = autoArray;
  cvt->userData = userData;
  cvt->description = description;
  cvt->next = NULL;

  pos = addFromTargetConverter(cvt);
  if(index)
    *index = pos;

 return(cvt);
}

SEXP
R_addFromPythonConverter(SEXP match, SEXP converter, SEXP autoArray, SEXP description, SEXP index)
{
   FromPythonConverterMatch *m;
   FromPythonConverter *cvt;

   int idx = INTEGER(index)[0];
   m = (FromPythonConverterMatch *) R_ExternalPtrAddr(match);
   cvt = (FromPythonConverter *) R_ExternalPtrAddr(converter);

   addFromPythonConverterInfo(m, cvt, LOGICAL(autoArray)[0], NULL, strdup(CHAR(STRING_ELT(description, 0))),
                               &idx);

   return(ScalarInteger(idx));
}



RSToTargetConverter *
addToTargetConverterInfo(ToTargetConverterMatch match, ToTargetConverter converter, Rboolean autoArray, void *userData, char *description, int *index)
{
 int pos;
 RSToTargetConverter *cvt = (RSToTargetConverter*) malloc(sizeof(RSToTargetConverter)); 
  cvt->match = match;
  cvt->converter = converter;
  cvt->autoArray = autoArray;
  cvt->userData = userData;
  cvt->description = description;
  cvt->next = NULL;

  pos = addToTargetConverter(cvt);
  if(index)
    *index = pos;

 return(cvt);
}

int
addToTargetConverter(RSToTargetConverter *cvt)
{
  int ctr = 0;
  RSToTargetConverter *tmp = ToTargetConverters;

  cvt->next = NULL;
  if(tmp == NULL) {
     ToTargetConverters = cvt;
  } else {
    while(tmp->next) {
       ctr++;
       tmp = tmp->next;
    }
    tmp->next = cvt;
  }      
  
  return(ctr);
}
