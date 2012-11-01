\name{setPythonHandler}
\alias{setPythonHandler}
\alias{getPythonHandler}
\title{Get or set the R to Python foreign reference handler.}
\description{
The R-Python reference manager/handler is responsible for caching and
resolving R objects that are exported to Python as references to R
objects. The manager accepts these R objects and stores them with a
given unique name, and returns an object that identifies it. When the
object is used in Python, the reference is passed back to R and
resolved by this handler.  This handler is implemented as a collection
of basic R functions with different functions handling the storing and
resolving of references, calling functions on that reference.  Usually
this is a closure that shares access to the pool of objects being
exported.  These two functions allow one to get the currently
registered handler and also to set the handler.
}
\usage{
setPythonHandler(handler)
getPythonHandler(handler)
}
\arguments{
  \item{handler}{the handler to be registered as the active one.}
}
\details{
Currently, the form of the handler is quite restricted.
It should be a list with the same length and functions 
in the same order as those in the default handler
returned by \code{\link{referenceManager}}.
}
\value{
 Both functions return the reference handler that was registered
before the function was called. In the case of the
\code{setPythonHandler}, this is the value that is being replaced.
}
\references{\url{http://www.omegahat.org/RSPython},
\url{http://www.python.org}}
\author{Duncan Temple Lang}

\seealso{
\code{\link{referenceManager}}
}

\examples{

}
\keyword{programming}
\keyword{interface}
\concept{Inter-system Interface}
\concept{Python}

