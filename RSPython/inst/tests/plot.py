import RS
RS.call("rnorm", 10)
RS.call('par', mfrow=(2,2))
RS.call("plot", [1,2,3,4])
RS.call("plot", [1,2,3,4], main='The integers 1 to 4', ylab='The integers', xlab='The index')

RS.call("plot", RS.call("rnorm", 10), main='Ten Random Normal Deviates',ylab='The Deviates', xlab='The Index');
x = RS.call("rnorm", 100)
y = RS.call("rnorm", 100)
z = RS.call('rnorm',100)
RS.call("plot", x, y, main='Two sets of 100 Random Normal Deviates', ylab='Second Set', xlab='First Set');
RS.call('abline', h=0, v=0, lty=2)
print RS.call('cor',x,y)

# You can now use the locator() function in R.
# For example,
#   RS.call("locator", 2)
# allows you to identify two points.
