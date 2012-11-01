importPythonModule <-
function(name, all = TRUE)
{
 if(as.logical(all))
   cmd <- paste("from", name, "import *")
 else
   cmd <- paste("import", name)
 .PythonEval(cmd)
}

pythonModuleNames <-
function(name)
{
 .Call("RS_PythonGetModuleNames", as.character(name))
}

pythonModuleTypes <-
function(name)
{
 .Call("RS_PythonGetModuleTypes", as.character(name))
}
