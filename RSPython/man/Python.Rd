\name{.Python}
\alias{.Python}
\alias{.PythonMethod}
\title{Invoke a Python function or instance method}
\description{
 These are the functions that invoke a Python function
or a method of a Python object.
They transfer the S arguments to their corresponding
Python values and invoke the identified function object.
The resulting Python value is transferred back as an 
S object using the default converters.
}
\usage{
.Python(func, ..., .module=NULL, .convert=T)
.PythonMethod(ref, method, ..., .convert=T)
}
\arguments{
  \item{func}{usually, a string specifying the name of the Python function to invoke.
This can also be a \code{PythonReference} object which refers to a previously
``computed'' function object.}
  \item{ref}{The Python object whose instance method is to be invoked. This is the 
\code{self} object in Python terms.}
  \item{method}{the name of the instance method to be invoked.}
  \item{\dots}{the arguments to the Python function or method, given as S objects
and converted to Python using the default converters.}
  \item{.module}{the name of the module in which to find to find and resolve the
 Python function}
  \item{.convert}{a logical value indicating whether an attempt to convert 
the result from the Python function or method to a regular R value
should be made, or whether the object should be left on the Python side of
the interface and a reference to it returned. Avoiding conversion can be useful
when the value will be used in subsequent calls Python functions or methods.}
}
\details{
 These functions use the standard conversion mechanism for
translating R objects to Python values and vice versa.
These can be augmented by 
}
\value{
 The return value resulting from calling the Python function
or method. This is converted to an R object or 
returned as a reference  to the Python object
in the form of an object \code{PythonReference}.
}
\references{\url{http://www.omegahat.org/RSPython},
\url{http://www.python.org}}
\author{Duncan Temple Lang}

\seealso{ 
\code{\link{.PythonEval}}
\code{\link{.PythonEvalFile}}
\code{\link[RSPerl]{.Perl}}
\code{\link[SJava]{.Java}}
\code{\link[CORBA]{.CORBA}}
}

\examples{
 .Python("version", .module="sys")
 .Python("path", .module="sys")
}
\keyword{programming}
\keyword{interface}
\concept{Inter-system Interface}
\concept{Python}
