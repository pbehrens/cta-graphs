import RS
class SForeignReference:
        "A Python class for representing references to S objects"
	def __init__(self, id, type=None, classes=None):
           self.name = id
           self.classes = classes
           self.type = type
           return
        def setName(self, id):
           self.name = id
           return self.name
        def getName(self):
           return(self.name)

        def __callS__(self,funcName, *args, **namedArgs):
           print("In __callS__")
           return RS.call(funcName, args, namedArgs, convert=1, ref=self)


class RForeignReference(SForeignReference):
        "A Python class for representing references to R objects"

class RForeignFunction(RForeignReference):
        "A Python class representing an R function"
        def __call__(self, *args, **namedArgs):
            return RS.call(self, args, namedArgs)

class RForeignList(RForeignReference):
       "A Python class representing an R list, providing overloaded operator access"
       def __getitem__(self, name, convert=1):
           RS.get(self, name, convert)
       def __getattr__(self, attrname):
           "Get an element of an S list"	
	   return(RS.call("$",self, attrname))
