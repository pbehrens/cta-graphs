\name{pythonFunction}
\alias{pythonFunction}
\title{Python Method Information}
\description{
This returns information describing a Python
method or function within a Python module.
It gives reflectance information allowing one to determine
the number and types of arguments, which are keywords, 
and what the default values are, if any.
Additionally, this returns the documentation string of the object.
}
\usage{
pythonFunction(obj, module=NULL)
}
\arguments{
  \item{obj}{the name of the function or method being queried}
  \item{module}{the name of the module in which the function/method is to be found. This can be
 a character vector of one or more names identifying the module.}
}
\details{

}
\value{
An object of classes
\code{Function} and  \code{PythonReference}.
It contains
  \item{name}{Description of `comp1'}
  \item{code}{a description of the function definition (i.e. ``signature'' and body)
       giving the names of the arguments and giving a count of how many local and external
       values 
   }
  \item{defaults}{a list of the default values of the different arguments. This is a named list
  whose names correspond to the argument names.}
  \item{doc}{the documentation string associated with the Python function/method}
}
\references{\url{http://www.omegahat.org/RSPython},
\url{http://www.python.org}}
\author{Duncan Temple Lang}

\seealso{
 \code{\link{pythonModuleTypes}}
}

\examples{
 pythonFunction("call", module="RS")

 els <- pythonModuleTypes("RS")
 RS.functionNames <- names(els[els == "function"])
 RS.functionDescriptions <- lapply(RS.functionNames, function(x) pythonFunction(x, module="RS"))
 names(RS.functionDescriptions) <- RS.functionNames

 # Just get the documentation string.
 RS.docs <- lapply(RS.functionNames, function(x) pythonFunction(x, module="RS")[["doc"]])
 names(RS.docs) <- RS.functionNames
}
\keyword{programming}
\keyword{interface}
\concept{Inter-system Interface}
\concept{Python}

