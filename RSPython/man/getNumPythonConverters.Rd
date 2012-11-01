\name{getNumPythonConverters}
\alias{getNumPythonConverters}
\alias{getPythonConverterDescriptions}
\alias{removePythonConverter}
\title{Query the currently registered object converters between R and Python.}
\description{
 These functions allow the user to examine what C routines
and R and Python functions are currently registered
to implement the conversion between R and Python objects.
These control how values in one system are converted
to objects in the other system as they are transferred
as arguments to functions in the foreign system
and returned as results to the calling system.
These functions give both the number 
and descriptions of each of the converters registered
in either direction (i.e., from R to Python and vice-versa.)
The descriptions are usually specified by the registrant when the converters are
registered.
}
\usage{
getNumPythonConverters(which=c(fromPython = F, toPython = T))
getPythonConverterDescriptions(which=c(fromPython = F, toPython = T))
removePythonConverter(id, fromPython)
}
\arguments{
  \item{which}{the direction(s) of the conversion of interest, i.e. 
    from Python to R and/or from R to Python}
  \item{id}{an identifier for the converter. This can be either an
    integer giving the position of the converter or the description
    string obtained from \code{getPythonConverterDescriptions}.
    The index is 1-based, i.e. the first element is at position 1.}
  \item{fromPython}{a logical value. If \code{TRUE} this indicates that
    we are removing the specified converter in the from python to R
    collection of converters.}
}
\details{
 Converters are registered at the C level as pairs
of C routines given by a matching routine that determines
whether the converter is appropriate for the object to be converted
and the  converter routine itself which performs the translation from one
system to the other. Additionally, 
}
\value{
 \code{getNumPythonConverters} returns an integer vector
giving the number of converters in the list for each of
the specified directions.
 \code{getPythonConverterDescriptions} returns a list
with an element for each of the directions specified.
Each element in this list is a character vector 
containing the description string for each of the 
different (match, converter) pairs.

\code{removePythonConverter} returns an integer vector
whose names are the descriptions of the elements that
were removed.
If an element was not found, an NA appears for
that element.
}
\references{\url{http://www.omegahat.org/RSPython},
\url{http://www.python.org}}
\author{Duncan Temple Lang}

\seealso{
\code{\link{.Python}}
\code{\link{.PythonNew}}
\code{\link{.PythonMethod}}
}

\examples{

  getNumPythonConverters()
  getPythonConverterDescriptions()
  getPythonConverterDescriptions(c(fromPython = TRUE))
}
\keyword{programming}
\keyword{interface}
\concept{Inter-system Interface}
\concept{Python}

