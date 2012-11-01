\name{PythonReference Element Access}
\alias{$.PythonReference}
\title{Syntactic convenience for calling a Python references's method}
\description{
This allows one to call a method of a Python object
from R in a simple form such as \code{pyObj$method(arg1, arg2, ...)}.
This provides an overloading of the \code{$} operator
for R objects of class \code{PythonReference} to return
a simplified call to \code{.PythonMethod}
}
\usage{
"$.PythonReference"(obj, name)
}
\arguments{
  \item{obj}{the \code{PythonReference} object whose method is to be called.}
  \item{name}{the name of the method in the Python object which is to be called.}
}
\details{
 When calling a Python object's method from R, one specifies the
reference to the Python object and the name of the method along with
the arguments to that method in a call to \code{\link{.PythonMethod}}. 
This function/method returns  function object containing sufficient
information to call the specified method on the given reference.
These two pieces of information are stored in an R closure instance
and this can then be called as a regular function whose
body calls \code{\link{.PythonMethod}}.
}
\value{
 A function closure instance whose body consists
of a call to \code{.PythonMethod} for the 
specified \code{PythonReference} object
and method name. 
}
\references{\url{http://www.omegahat.org/RSPython},
\url{http://www.python.org}}
\author{Duncan Temple Lang}

\seealso{
\code{\link{.PythonMethod}}
\code{\link{.Python}}
\code{\link{.PythonNew}}
\code{\link{.PythonEval}}
\code{\link{.PythonEvalFile}}
}

\examples{
\dontrun{
 x <- .PythonNew("ArithmeticError", "Some error message", .module="exceptions") 
 x$"__str__"()  # call the __str__() method, escaping the __ 
 x$args()       # 
}
}
\keyword{programming}
\keyword{interface}
\concept{Inter-system Interface}
\concept{Python}

