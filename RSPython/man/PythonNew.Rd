\name{.PythonNew}
\alias{.PythonNew}
\title{Create an instance of a Python class}
\description{
 This function creates an instance of a Python class.
Generally, this returns a reference to that object
so that it can be used in subsequent Python calls 
from R.
}
\usage{
.PythonNew(className, ..., .module=NULL, .convert=F)
}
\arguments{
  \item{className}{the name of the class of which an  new instance is to be created}
  \item{\dots}{any arguments to the \code{\_\_init\_\_} method of the class, including
   named arguments}
  \item{.module}{the ``name'' of the module in which the class is defined. This is
  usually a single string, but for nested modules is a character vector whose elements
  are in the top-level to sub-module order; e.g. if a class C is defined in the module B
  which is in A (i.e. A.B.C in Python terms), the \code{.module} is given as
   \code{c("A","B")}}
  \item{.convert}{a logical value indicating whether to attempt to convert the newly created object
    or not. This is usually only \code{FALSE} when creating the object has a useful side-effect.}
}
\value{
 If \code{.convert} is \code{FALSE}, a reference to the 
 newly created object is returned. This is an object of
 class \code{}. Otherwise, an attempt to convert the newly created
 object using the built-in and the current user-specified converters
 is performed. The result depends on what these return. If no converter
 is found, a reference to the newly created object is  returned, just
  as if \code{.convert} was specified as \code{FALSE}.
}
\references{\url{http://www.omegahat.org/RSPython},
\url{http://www.python.org}}
\author{Duncan Temple Lang}

\seealso{
\code{\link{.PythonMethod}}
\code{\link{.Python}}
}

\examples{
   # Get a class that we know exists!
 e <- .PythonNew("ArithmeticError")
 e <- .PythonNew("ArithmeticError", 'a simple messge string')
 e$"__str__"()

\dontrun{
  u <- .PythonNew("urlopen", "http://www.omegahat.org/index.html", .module="urllib")
  .PythonMethod(u, "geturl")  
  txt <- u$read()
  u$geturl()
  u$close()
}
}
\keyword{programming}
\keyword{interface}
\concept{Inter-system Interface}
\concept{Python}

