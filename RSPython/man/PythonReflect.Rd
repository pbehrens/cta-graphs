\name{Python Reflectance}
\alias{pythonMethods}
\alias{getSuperClasses}
\alias{pythonModuleNames}
\title{Information about Python classes and methods}
\description{
  These functions return information about a python class
  and its methods or the contents of module.
}
\usage{
pythonMethods(obj, all=F)
getSuperClasses(obj, module)
pythonModuleNames(name)
}
%- maybe also `usage' for other objects documented here.
\arguments{
  \item{obj}{a character vector giving the name of a Python class,
    given in the form \code{c("className", "moduleName")}}
  \item{module}{the name of the module in which to find the class given
    by \code{obj}}
  \item{all}{controls how inherited methods and attributes are
    processed. If \code{TRUE} is specified, the inherited methods are
    also reported. Otherwise, these are ignored and only those methods
    defined within the class are returned.}
  \item{name}{the name of the module for which we are to get the
    names of the elements in that module.}
}
\details{

}
\value{

}
\references{\url{http://www.omegahat.org/RSPython},
\url{http://www.python.org}}
\author{Duncan Temple Lang}

\note{This will be implemented differently in the future
and will make use of the Signature module for Python}
\seealso{}

\examples{

 pythonModuleNames("__main__")
 pythonModuleNames("RS")
}
\keyword{programming}
\keyword{interface}
\concept{Inter-system Interface}
\concept{Python}

