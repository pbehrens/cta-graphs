#
# Used to ensure that the PyMapping  is converted correctly
# Put this directory in PYTHONPATH. 
# library(RSPython)
# importPythonModule("map")
# .Python("getMap")
#
def getMap():
    "example of getting a map object back from a call to python"
    return({'abc': 1, 'myKey': 'a value'})

