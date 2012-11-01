getNumPythonConverters <-
#
# Returns the number of converters
# registered internally in a C list/table
# for performing the conversion between R and Java. 
#   F represents from Java to R,
#   T represents from Java to R.
# This is vectorized in the type or direction of converter.

# By default, it returns both.

function(which = c(fromPython=FALSE, toPython=TRUE))
{
 ans <- integer(0)
 for(i in which)
  ans <- c(ans, .Call("RPython_getNumConverters",as.logical(i))) 

 names(ans) <- names(which)

 ans
}


getPythonConverterDescriptions <-
function(which = c(fromPython = FALSE, toPython = TRUE))
{
 ans <- vector("list", length(which))
 for(i in 1:length(which)) {
  ans[[i]] <- .Call("RPython_getConverterDescriptions", as.logical(which[i]))
 }

 names(ans) <- names(which)

 ans
}


removePythonConverter =
function(id,  fromPython)
{
  if(is.character(id)) {
     tmp = c(fromPython = FALSE, toPython = FALSE)
     direction = ifelse(fromPython, "fromPython", "toPython")
     tmp[direction] <- TRUE
     desc = getPythonConverterDescriptions(which = tmp)[[direction]]
     id = pmatch(id, desc)
     if(is.na(id))
        stop("No description matching ", id, " in: ", paste("'", desc, "'", sep = "", collapse  = ", "))
  } else
     id = as.integer(as.numeric(id) - 1)
  
  ans = .Call("RPython_removeConverter", id, as.logical(fromPython), PACKAGE  = "RSPython")
  which = ans  < 0
  if(any(which))
    ans[which] <- NA

  ans
}

setOldClass("NativeSymbolInfo")
setOldClass("NativeSymbol")
setAs("NativeSymbolInfo", "NativeSymbol", function(from) from$address)
setAs("character", "NativeSymbol", function(from) as(getNativeSymbolInfo(from), "NativeSymbol"))

addFromPythonConverter =
function(match, converter, description, autoArray = FALSE, index = -1)
{
   # Doesn't handle R functions yet.  Only a matter of time to get to it.
  match = as(match, "NativeSymbol")
  converter = as(converter, "NativeSymbol")  
  
  .Call("R_addFromPythonConverter", match, converter, as.logical(autoArray),
                   as.character(description), as.integer(index),
                    PACKAGE = "RSPython")
}  
