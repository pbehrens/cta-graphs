\name{referenceManager}
\alias{referenceManager}
\title{Create instance of the standard Reference Manager}
\description{

}
\usage{
referenceManager()
}

\details{
 If no reference manager is available when it is first needed
by a call from Python to R, this function is called to create
a \code{referenceManager} instance and is then registered
at the C level.
}
\value{
 A closure that stores R objects and returns
references to them. The closure provides a variety of different
functions to operate on these R objects:
  \item{handler}{}
  \item{createReference}{}
  \item{addReference}{}
  \item{remove}{}
  \item{getReference}{}
  \item{references}{}
  \item{total}{}
}
\references{\url{http://www.omegahat.org/RSPython},
\url{http://www.python.org}}
\author{Duncan Temple Lang}


\seealso{
\code{\link{setPythonHandler}}
\code{\link{getPythonHandler}}
}

\examples{

}
\keyword{programming}
\keyword{interface}
\concept{Inter-system Interface}
\concept{Python}

