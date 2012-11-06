#use build in help files to search for functions
?install.packages
help.search("install.packages")
# assign numbers to two variables
t <- 45
u <- 45.543543
# from the interpreter you can print out a variable by typing in the variable name
# print out t and u
t
u
#Define a string using single or double quotes.
y <- "hello world"
y

x <- c(54,43,43,76,21,32)
y <- c("one", "two", "three", "four")
z <- c(TRUE, FALSE, TRUE, TRUE, FALSE, FALSE, TRUE)
x(c(2,5))
# 2nd and 5th index
z[2:5]
#index 2 through 5

#create an 8 x8 matrix of numbers of numbers from 1 to 64
x <-matrix(1:64, nrow=8, ncol=4)
# create a 4x4 matrix with the columns and rows labels
# byrow=TRUE makes sure the matrix is filled by columns instead of rows
# dimnames is set to a list of of two vectors that will be set as the row and column labels y <-matrix(1:16, nrow=2, ncol=2,byrow=TRUE, dimnames=list(c("a", "b"), c("1", "2")))


a <- c("one", "two", "three", "four")
b <- "Hello World"
c <- matrix(1:64, nrow=8, ncol=4)
# create list with variables of mixed data types
the.list <- list(avector=a, astring=b, amatrix=c)

x <- c(1,2,3,4)
y <- c("one", "two", "three", "four")
z <- c(TRUE,FALSE,FALSE,FALSE)
the.data <- data.frame(x,y,z)
# set the column names of the data frame by passing names() a vector of labels
names(the.data) <- c("integers","strings","logical") # variable names
head(the.data)
