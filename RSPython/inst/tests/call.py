import RS;

RS.call("rnorm", 10);

RS.call("objects");
RS.call("objects",3);

RS.call("sin", 2);
RS.call("sin", 2.0);

RS.call("plot", [1,2,3,4]);

RS.call("plot", RS.call("rnorm", 10));
    

# Even "easier"

#import R from R
from R import R

R.sin(2)
R.sin(2.0)

R.sum([1, 2, 3, 4])

R.rnorm(10)
R.rnorm(10, sd = 8)

# R.as.numeric("1")  won't work!


