\name{isPythonInitialized}
\alias{isPythonInitialized}
\title{Is Python Interpreter Initialized}
\description{
 This function tests whether the Python interpreter is in an initialized state
and optionally initializes it if it is not (using default settings).
}
\usage{
isPythonInitialized(init=F)
}
\arguments{
  \item{init}{a logical value indicating whether to perform the default initialization
 if the Python interpreter is not already initialized.
  }
}
\details{
 This checks the internal state of the Python library and queries if the
interpreter is already initialized. If not and \code{init} is \code{TRUE},
the function \code{\link{.PythonInit}} is called with no arguments.
}
\value{
 A logical value indicating what the current status of the Python interpreter
is. If \code{init} is \code{FALSE}, this is the state before the call.
Otherwise, the interpreter should have been started, except if an error occurs.
}
\references{\url{http://www.omegahat.org/RSPython},
\url{http://www.python.org}}
\author{Duncan Temple Lang}

\seealso{\code{\link{.PythonInit}}}

\examples{
 isPythonInitialized(init)

 if(isPythonInitialized())
}
\keyword{programming}
\keyword{interface}
\concept{Inter-system Interface}
\concept{Python}
