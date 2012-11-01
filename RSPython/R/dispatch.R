"$.PythonReference" <-
#
# 
#
function(obj,name)
{
 function(...) {
  .PythonMethod(as.character(name),..., ref=obj)
 }
}

"[[.PythonReference" <-
function(x, i, ..., .convert = TRUE)
{
  .Call("RPy_getAttr", x, as.character(i), as.logical(.convert))
}  

if(FALSE) {
"print.PythonReference" <-
function(x, ...)
{
  txt = paste("<", class(x)[1], "(", unclass(x)$class, "): ", unclass(x)$id, ">", sep = "")
  print(txt)
}  
}
