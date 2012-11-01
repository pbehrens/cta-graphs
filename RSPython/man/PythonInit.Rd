\name{.PythonInit}
\alias{.PythonInit}
\alias{.PythonTerminate}
\title{Initialize the Python interpreter.}
\description{
 These functions allow one to initializes the Python engine and creates an interpreter
 and also to discard it and its contents.
 The \code{.PythonInit} function  must be called before invoking a Python function or method.
 One can start, stop and restart the Python interpreter any number of times.
}
\usage{
.PythonInit(path=NULL, merge=T, warn = FALSE, force=TRUE)
.PythonTerminate()
}
\arguments{
 \item{path}{if specified, this should be a string specifying the directories
 in which to find Python modules. The directory names should be separated by the appropriate
 directory separator (":" on Unix). If this is missing, the current value fo 
 the \code{PYTHONPATH} environment variable is used.}
 \item{merge}{a logical value indicating whether the basic elements needed
 to locate the Python modules used to implement this package should be appended
 to the value of the system variable \code{PYTHONPATH}}
 \item{warn}{if \code{TRUE}, issue a warning about the implicit initialization}
 \item{force}{if \code{FALSE} and the Python interpreter has already
   been initialized, we just issue a warning and return. However, if
   this is \code{TRUE}, we attempt to perform another initialization.} 
}
\details{
 \code{.PythonInit} passes its arguments
to \code{\link{.PythonPath}} to determine the
appropriate value for the \code{PYTHONPATH} environment
variable. It then uses the returned value to set
this environment variable and then initializes the python
interpreter.
}
\value{
 NULL or an error occurs.
}
\references{\url{http://www.omegahat.org/RSPython},
\url{http://www.python.org}}
\author{Duncan Temple Lang}
\note{Very bad things will happen if you stop the interpreter and then access a
Python object, function, method, etc. Python will terminate.}

\seealso{\code{\link{.PythonPath}}}
\examples{
\dontrun{
  .PythonInit()
}
}
\keyword{programming}
\keyword{interface}
\concept{Inter-system Interface}
\concept{Python}

