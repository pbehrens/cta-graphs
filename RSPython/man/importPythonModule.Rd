\name{importPythonModule}
\alias{importPythonModule}
\title{Loads a Python Module}
\description{
 This loads the Python module into the Python interpreter,
 making its functions and classes available to the Python, and hence R,
 session. This is equivalent to the Python command
 \code{from <name> import *}. 
}
\usage{
importPythonModule(name, all=T)
}
\arguments{
  \item{name}{}
  \item{all}{}
}
\value{

}
\references{\url{http://www.omegahat.org/RSPython},
\url{http://www.python.org}}
\author{Duncan Temple Lang}

\seealso{
\code{\link{pythonModuleTypes}}
}

\examples{
  importPythonModule("sys")
  .PythonEval("version")

  importPythonModule("sys", all=F)
  .PythonEval("sys.version")

\dontrun{
  importPythonModule("urllib", all=F)
  pythonModuleTypes("urllib")
}
}
\keyword{programming}
\keyword{interface}
\concept{Inter-system Interface}
\concept{Python}
