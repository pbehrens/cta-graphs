\name{.PythonEvalFile}
\alias{.PythonEvalFile}
\alias{.PythonEval}
\title{Evaluate a Python script from a string or file.}
\description{
 These two functions allow one to evaluate Python scripts
which are given in the Python syntax. These can be specified
as a string or as a file containing the Python code.
This approach to evaluating Python commands is less
flexible than those offered by \code{\link{.Python}}
and related functions.
}
\usage{
.PythonEvalFile(cmd, ..., .module=NULL, .convert=T)
.PythonEval(cmd, ..., .module=NULL, .convert=T, .isFile = FALSE)
}
\arguments{
  \item{cmd}{the Python expression to parse and evaluate.}
  \item{\dots}{Any number of name-value pairs that are converted to a Python
  dictionary or context which will be used when evaluating the expression. }
  \item{.module}{the name of the module whose dictionary is used to provide the name
  space for resolving symbols in the Python expressions.}
  \item{.convert}{a logical value indicating whether the result of evaluating the Python
object should be converted to a regular R object, if possible, or explicitly left
within Python and returned as a \code{PythonReference} object.}
  \item{.isFile}{indicates whether the command `cmd' identifies a file
    by name whose contents should be evaluated, or is a Python expression.}
}
\details{

}
\value{
  The value returned from evaluating the Python expression(s),
converted to R using the standard conversion mechanism, including
any user-level converters currently registered.
}
\references{\url{http://www.omegahat.org/RSPython},
\url{http://www.python.org}}
\author{Duncan Temple Lang}

\seealso{
\code{\link{.Python}}
\code{\link{.PythonMethod}}
\code{\link{.PythonNew}}
}

\examples{
.PythonEval("import sys; sys.version")

  #
# .PythonEval("from sys import *")
 .PythonEval("sys.version")
}
\keyword{programming}
\keyword{interface}
\concept{Inter-system Interface}
\concept{Python}

