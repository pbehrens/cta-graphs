\name{getPythonReferenceIDs}
\alias{getPythonReferenceIDs}
\title{Python Foreign Reference Information}
\description{
This returns the names of the currently active
foreign Python reference objects being managed by 
the Python reference manager.
}
\usage{
getPythonReferenceIDs()
}
\details{
This probes the reference manager in which the 
exported Python objects are actually stored
and gets information about each element.
}
\value{
  A character vector giving the identifiers (i.e. names) of
the currently active foreign references exported from the Python
session.
}
\references{\url{http://www.omegahat.org/RSPython},
\url{http://www.python.org}}
\author{Duncan Temple Lang}

\seealso{\code{\link{.Python}},
\code{\link{.PythonNew}}
}

\examples{
\dontrun{
  before <- getPythonReferenceIDs()
   # do something that creates Python reference objects
  after <-  getPythonReferenceIDs()
   # Now cleanup the new ones.
}
}
\keyword{programming}
\keyword{interface}
\concept{Inter-system Interface}
\concept{Python}

