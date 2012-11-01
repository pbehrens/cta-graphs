\name{pythonModuleTypes}
\alias{pythonModuleTypes}
\title{Python Module Elements and Types}
\description{
 This function provides reflective information about the
 contents of a module by returning a list of all the
 entries in that module and the type of object it (currently) is.
 This allows one to differentiate the functions or methods
 and the fields of a module  and to understand what the fields
 contain.
 Essentially it describes the elements of the module's dictionary
 (\code{\_\_dict\_\_})
}
\usage{
pythonModuleTypes(name)
}
\arguments{
  \item{name}{the name of the module in question. This is given as a single string or
a character vector containg the hierarchy of module names in a top-down or top-first form.}
}
\details{
 
}
\value{
 A named character/string vector whose name-value pairs
 give the name of each  entry in the module and its type.
 The types are 
}
\references{\url{http://www.omegahat.org/RSPython},
\url{http://www.python.org}}
\author{Duncan Temple Lang}

\seealso{
}

\examples{
 pythonModuleTypes("sys")
 pythonModuleTypes("RS")
}
\keyword{programming}
\keyword{interface}
\concept{Inter-system Interface}
\concept{Python}
