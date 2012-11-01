\name{.PythonPath}
\alias{.PythonPath}
\title{Create Python Load Path}
\description{
 This function handles creating a suitable value for
the \code{PYTHONPATH} environment variable which controls
how the Python interpreter finds its modules.
}
\usage{
.PythonPath(path=NULL, merge=T)
}
\arguments{
 \item{path}{if specified, this should be a string specifying the directories
 in which to find Python modules. The directory names should be separated by the appropriate
 directory separator (":" on Unix). If this is missing, the current value fo 
 the \code{PYTHONPATH} environment variable is used.}
 \item{merge}{a logical value indicating whether the basic elements needed
 to locate the Python modules used to implement this package should be appended
 to the value of the system variable \code{PYTHONPATH}}
}
\details{
The following is the algorithm used to determine the
path.
 If there is no PYTHONPATH set, then we set our version so that
 one can find the RS modules.

 If there is a PYTHONPATH already set, then we take it and append
 our value to it. This can be inhibited by specifying `merge = F'

 Finally, if there is a non-null value for the path argument, then 
 we use that as the value for PYTHONPATH. Again, if merge = T, we 
 append the RS directories to it.

 If merge is T, then we take whatever value for path is given
 and append it to the current value of PYTHONPATH
}
\value{
 A string (i.e. character vector of length 1) giving the 
appropriate setting for the \code{PYTHONPATH} environment variable.
}

\references{\url{http://www.omegahat.org/RSPython},
\url{http://www.python.org}}
\author{Duncan Temple Lang}

\seealso{\code{\link{.PythonInit}}}

\examples{
 .PythonPath(system.file("tests", pkg="RSPython"))

 .PythonPath(c(.PythonPath(), system.file("tests", pkg="RSPython")))

\dontrun{
 .PythonInit(system.file("tests", pkg="RSPython"))
}
}
\keyword{programming}
\keyword{interface}
\concept{Inter-system Interface}
\concept{Python}
