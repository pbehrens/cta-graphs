\name{[[.PythonReference}
\alias{[[.PythonReference}
\title{Get an element in a Python object}
\description{
  This gets a field or slot in the Python object
  identified by the PythonReference.
}
\usage{
"[[.PythonReference"(x, i, ..., .convert=T)
}

\arguments{
  \item{x}{the \code{PythonReference} object in which to find the field
named \code{name}.}
  \item{i}{a character vector specifiying the name of the field to retrieve.}
  \item{\dots}{ignored, but present for compatability with the generic.}
  \item{.convert}{a logical value indicating whether to attempt to
     convert the resulting Python object to R (TRUE) or leave as a reference (FALSE)}
}

\value{
 The value of the Python field
 named \code{name} in the specified object,
 converted to an R object or exported to
 R as a reference to the Python object.
}

\references{\url{http://www.omegahat.org/RSPython},
\url{http://www.python.org}}
\author{Duncan Temple Lang}

\note{
One can get the names of the different
fields that are accessible in a Python reference
\code{x} using the command
\code{x[["__dict__", .convert=F]]$keys()}
}

\seealso{
\code{\link{$.PythonReference}}
}

\examples{
\dontrun{
  p <- .PythonNew("SDefsParser", "foo.defs", .module="Sgenerate")
  p[["functions"]]


  p[["__dict__", .convert=F]]$keys()
}
}
\keyword{interface}

