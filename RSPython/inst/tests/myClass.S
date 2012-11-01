myClass <- function(a=1,b=letters[1:3])
{
  x <- list(a=a,b=b)
  class(x) <- "MyClass"

 return(x)
}
