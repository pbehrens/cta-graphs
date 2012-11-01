#
# init methods
# Handle passing * and ** args as values of other * and **args
#  and avoid the nesting.
#
#
#

__version__=0,1,2
__doc__= """\

This class is used to generate a new Python class

from hierarchy import *  # in tests/
g = SClassGenerator(hierarchy.A)
print(g.defineClass())


def ex(el, klass):
    return(el.__name__ == "foo3")
print(g.defineClass(exclude=ex))
"""
from types import  *

from Signature import *

# Signature.py can be obtained from
#   http://www.sff.net/people/neelk/open-source/
#
class SClassGenerator:
     "Create a new class extending an existing Python, implementing the methods with calls to S functions."
     def __init__(self, baseClass, name=None, exclude=None):
        ""
        if type(baseClass) == ClassType:
          baseClass = [baseClass]
        self.baseClass = baseClass         
        if name == None:
             # compute the name of the new class from the base
             # class
           name = "SForeignReference"+ baseClass[0].__name__
        self.name = name
          # a function that can be called to
          # determine if a method should be added.
        self.exclude = exclude
          # stores the definition when processed.
        self.classDef = None
          # a list containing the names of methods
          # that have already been defined in this new class
          # and used to avoid re-defining an inherited method.
        self.already = []

     def defineClass(self, exclude=None, force = 0):
        "Create the definition for the new class, allowing one to specify the filter for excluding different methods from being implemented by calling the S system, and also allow forcing the definition to be recomputed"
        if force:
             self.reset()
             
        if self.classDef != None:
             return(self.classDef)
        
        if exclude != None:
             self.exclude = exclude
        tmp = self.processClassMethods()
        baseClassNames = "SForeignReference, "
        ctr = 1
        for x in self.baseClass:
             if ctr < len(self.baseClass):
                  sep = ", "
             else:
                  sep = ""
             baseClassNames = baseClassNames + x.__name__ + sep
             ctr = ctr + 1
        txt = "class " + self.name + "(" + baseClassNames + "):\n"
        txt = txt + tmp
        self.classDef = txt
        return(txt)
   
     def processClassMethods(self):
        "Iterate over all of the base classes of the new class, and all of their base classses, etc. to create definitions for all methods defined for the new class."
        txt = ""
        for x in self.baseClass:
          tmp = self.showClassMethods(x)
          if len(tmp) > 0:
             txt =  txt + tmp + "\n"

        for x in self.baseClass:             
          for k in x.__bases__:
             tmp=  self.showClassMethods(k)
             if len(tmp) > 0:
               txt = " " +  tmp + "\n"                     

        return(txt)
       
     def showClassMethods(self, klass, recursive = 1):
        ""
        txt = ""
        for x in klass.__dict__.keys():
           el = klass.__dict__.get(x)
           if isinstance(el, MethodType) | isinstance(el, FunctionType):
             if not el.__name__ in self.already and (self.exclude == None or self.exclude(el, klass) == 0):
                 txt = txt + " " +  self.getMethodDef(el, klass) + "\n"
                 self.already.append(el.__name__)

        if recursive:
             for x in klass.__bases__:
                  tmp = self.showClassMethods(x)
                  if len(tmp) > 0:
                     txt = " " +  tmp + "\n"                     
        return(txt)
        
     def getMethodDef(self, m, klass):
          ""
          s = Signature(m)
          els = self.getMethodSignature(m, s)
          txt = "  " + "def " + els[0] + ":\n\t"
          if m.__doc__ != None:
             txt = txt + "\"" + m.__doc__ + "\"\n\t"
          if len(els[1]) > 0:
                sep = ", "
          else:
                sep=""
          txt = txt + "return( RS.refCall(self, \"" +  s.name + "\"" + sep + els[1] + ") )"
          return(txt)
          
     def getMethodSignature(self, m, s = None):
          "Creates the signature for defining the method and the argument list for the call to RS.refcall to implement the method."
          if(s == None):
               s = Signature(m)
          defTxt = s.name + "("
          callArgs = ""
          ctr = 1
          numArgs = len(s.ordinary_args())
          for x in s.ordinary_args():
               defTxt = defTxt + x
               callArgs = callArgs + x               
               if x in s.defaults().keys():
                  defTxt = defTxt + (" = " + repr(s.defaults().get(x)))
               if ctr < numArgs:
                     defTxt = defTxt + ", "
                     callArgs = callArgs + ", "
               ctr = ctr + 1
          sp = s.special_args()
          if 'positional' in sp.keys():
              defTxt = defTxt + ", *"+ sp.get('positional')
              callArgs = callArgs + ", "+ sp.get('positional')              
          if 'keyword' in sp.keys():
              defTxt = defTxt + ", **"+ sp.get('keyword')
              callArgs = callArgs + ", "+ sp.get('keyword')                            
          defTxt = defTxt + ")"
          return([defTxt, callArgs])

     def getCode(self):
         "Creates the complete code for the new class being defined."
         txt = "from RReference import *\n"
         txt = txt + self.defineClass()
         return(txt)
     
     def writeCode(self, con = None):
        "Writes the code for the new class to the specified connection/stream, or creates one using the name of the new class"          
        if con == None:
             con = self.name + ".py"

        if type(con) == StringType:
             fd = open(con, "w")
        else:
             fd = con
        
        fd.write(self.getCode())
        if type(con) == StringType:
            fd.close()
        return(1)
   

     def reset(self):
        "Discard any previous computations for defining the class"
        self.classDef = None
        self.already = []
