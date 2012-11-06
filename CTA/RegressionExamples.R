## Data Mining Camp -
# Machine Learning Examples
#
# Author: mike-bowles
###############################################################################

#################################################
# 1st example - read in wine quality data and have a look
# Step 1.1 read the red wine quality data
rm(list=ls())
wineTable <- read.table("winequality-red.csv",sep=";", header = TRUE)

# Step 1.2 have a look at the file
str(wineTable)

# Step 1.3 - look at a couple of cross-plots
plot(wineTable$alcohol, wineTable$quality)

plot(wineTable$sulphates, wineTable$quality)

plot(wineTable$free.sulfur.dioxide, wineTable$quality)

#########################################################
# 2nd example - fit "quality" as a linear function of other attributes
# in R, we can define what's called a linear model.  by the expression quality ~ alcohol + sulphates + pH + ...
# this says "quality is a linear function of alcohol and sulphates and ...

# Step 2.1 Read in the wine quality data
lmWine <- lm(quality~., data = wineTable)
str(lmWine)

# Step 2.2 let's see how well the model fits the data it was trained on. 
xx <- wineTable[-12]
lmFit <- predict(lmWine, newdata = xx)

# Step 2.3 Skip during pres Difference between original labels and lmFit = residuals 
summary((wineTable[12]-lmFit) - lmWine$residuals)

# Step 2.4 What's the sample standard deviation?
rmsError <- sqrt(sum(lmWine$residuals * lmWine$residuals)/length(lmWine$residuals))
print("rmsError = ")
rmsError

# Step 2.5 Let's look at a crossplot
plot(wineTable$quality, lmFit)


##############################################################
# 3rd Example - use linear regression to classify Sonar data mines vs rocks


# Step 3.1 read in the sonar data and have a look

sonarTrain <- read.table("sonar_train.csv", sep = ",")

# Step 3.2 What's it look like? 
str(sonarTrain)

# Step 3.3 Let's follow the same process
# Perform a linear regression and see how well it works
lmSonar <- lm(V61~., data = sonarTrain)
sonarFit <- sonarTrain[,-61]
lmFit <- predict(lmSonar, newdata = sonarFit)

correct <- sum(lmFit*sonarTrain$V61 >0)
correct/length(sonarTrain$V61)

# Step 3.4 - 7 wrong out of 130 - doesn't seem bad on the face of it. 
# but wait, there's more.  The sonar data comes in two parts.  sonar_train and
# sonar_test.  Let's check the performance on some new data

sonarTest <- read.table("sonar_test.csv", sep = ",")
str(sonarTest)

sonarFitTest <- sonarTest[,-61]
lmFitTest <- predict(lmSonar, newdata = sonarFitTest)

correct <- sum(lmFitTest*sonarTest$V61 > 0)
correct/length(sonarTest$V61)

# The error rate on the training data  was 5%, the error rate on the test data is 35%
# Considerably worse.  Is there anything wrong here? 

# Overfitting - What is overfitting??

# Step 3.5 - Here's a way to use a small amount of data (we'll use 10% in the example)
# to estimate preformance while training

# Cross-validation
Sonar <- sonarTrain
trainErr <- 0.0
testErr <- 0.0
I <- seq(from = 1, to = nrow(Sonar))
for(ixval in seq(from = 1, to = 10)){
    Iout <- which(I%%10 == ixval - 1)
    SonarIn <- Sonar[-Iout,]
    SonarOut <- Sonar[Iout,]
   
    lmSonar <- lm(V61~., data = SonarIn)
    sonarFit <- SonarIn[,-61]
    lmFit <- predict(lmSonar, newdata = sonarFit)
   
    correct <- sum(lmFit*SonarIn$V61 >0)
    trainErr <- trainErr + (1-correct/length(SonarIn$V61))/10
   
    sonarFit <- SonarOut[,-61]
    lmFit <- predict(lmSonar, newdata = sonarFit)
   
    correct <- sum(lmFit*SonarOut$V61 >0)
    testErr <- testErr + (1-correct/length(SonarOut$V61))/10
}
print("trainErr = ")
trainErr
print("testErr = ")
testErr

# Step 3.6 - Let's see how much reduction we can get in the overtraining
# error by using both sonar_train and sonar_test
Sonar <- rbind(sonarTrain, sonarTest)
trainErr <- 0.0
testErr <- 0.0
for(ixval in seq(from = 1, to = 10)){
    Iout <- which(I%%10 == ixval - 1)
    SonarIn <- Sonar[-Iout,]
    SonarOut <- Sonar[Iout,]
   
    lmSonar <- lm(V61~., data = SonarIn)
    sonarFit <- SonarIn[,-61]
    lmFit <- predict(lmSonar, newdata = sonarFit)
   
    correct <- sum(lmFit*SonarIn$V61 >0)
    trainErr <- trainErr + (1-correct/length(SonarIn$V61))/10
   
    sonarFit <- SonarOut[,-61]
    lmFit <- predict(lmSonar, newdata = sonarFit)
   
    correct <- sum(lmFit*SonarOut$V61 >0)
    testErr <- testErr + (1-correct/length(SonarOut$V61))/10
}
print("trainErr = ")
trainErr
print("testErr = ")
testErr

# We could keep adding more data until the trainErr and testErr are close to the same
# but we're out of data.  What to do?  We need to reduce degrees of freedom somehow

#######################################################################
# 4th Example - Cross-validation in conjunction with ridge regression
# Step 4.1 read in both sonar_train and sonar_test and combine them
# into a single file.  Perform a ridge regression for a variey of penalty values
library(MASS)
sonarTrain <- read.table("sonar_train.csv", sep = ",")
sonarTest <- read.table("sonar_test.csv", sep = ",")
Sonar <- rbind(sonarTrain, sonarTest)

Err <- matrix(nrow = 31, ncol = 3)
I <- seq(1:nrow(Sonar))

for(iLambda in seq(from = 0, to = 30)){
    #
    exp <- (+3 -4*(iLambda/20))
    xlambda <- 10^exp
   
    testErr <- 0.0
    trainErr <- 0.0
   
    for(ixval in seq(from = 1, to = 10)){
        Iout <- which(I%%10 == ixval - 1)
        SonarIn <- Sonar[-Iout,]
        SonarOut <- Sonar[Iout,]
        #Xin <- X[-Iout,]
        Xin <- SonarIn[,1:60]
        Xout <- SonarOut[,1:60]
        Yin <- SonarIn[,61]
        Yout <- SonarOut[,61]
        mod <- lm.ridge(V61~.,data=SonarIn,lambda=xlambda)
        C <- mod$coef/mod$scales
        XM <- Xin
        for(i in seq(from = 1, to = ncol(Xin))){
            XM[,i]<-Xin[,i]-mod$xm[i]
        }
        X <- as.matrix(XM)
        A <- as.array(C)
        Yh <- X%*%A + mod$ym
        #trainErr <- trainErr + sum(abs(Yin - Yh))/(nrow(as.matrix(Yin))*10)
        trainErr <- trainErr + (1- sum(Yin*Yh > 0)/length(Yin))/10
        XM <- Xout
        for(i in seq(from = 1, to = ncol(Xout))){
            XM[,i]<-Xout[,i]-mod$xm[i]
        }
        X <- as.matrix(XM)
        A <- as.array(C)
        Yh <- X%*%A +mod$ym
        #testErr <- testErr + sum(abs(Yout - Yh))/(nrow(as.matrix(Yout))*10)
        testErr <- testErr + (1 - sum(Yout*Yh > 0)/length(Yout))/10
    }
    Err[(iLambda+1),1] = trainErr
    Err[(iLambda+1),2] = testErr
    Err[(iLambda+1),3] = xlambda
}
plot(Err[,1], type='p', col='red', ylim=c(0,1),
        main = 'Error vs Log(Lambda)',
        ylab='Error',
        xlab='3 - Log(Lambda)')
points(Err[,1], pch=15, col='red')
#lines(out[,2], type='l', col='blue')
points(Err[,2], pch=16, col='blue')
legend(5, 0.2, c("TRAIN", "TEST"), cex = 1, col = c("red", "blue"),
        pch = c(15, 16), lty = 1:2)

############################################################################
# 5th example
# using regression for multiclass problems.

# Step 5.1 - read in the iris data
iris <- read.table("irisdata.csv",sep = ",",header = FALSE)
summary(iris)

# Step 5.2 - Data visualization
# plot in V1 vs V2 axes
plot(iris$V1, iris$V2)
points(iris[1:50,1:2],col = 4)
points(iris[51:100,1:2], col = 2)
points(iris[101:150,1:2],col = 3)

# plot in V2 vs V3 axes
plot(iris[,2], iris[,3])
points(iris[1:50,2:3],col = 4)
points(iris[51:100,2:3], col = 2)
points(iris[101:150,2:3],col = 3)

# plot in V3 vs V4 axes
plot(iris[,3], iris[,4])
points(iris[1:50,3:4],col = "red")
points(iris[51:100,3:4], col = "blue")
points(iris[101:150,3:4],col = "green")

# Step 5.3 - create 3 separate "label" vectors - one for each class
Y1 <- c(rep(-1,150))
Y2 <- Y1
Y3 <- Y2
Y1[1:50] <- 1
Y2[51:100] <- 1
Y3[101:150] <- 1


# perform cross-validated ridge regression for each of these
#


# Step 5.4 train on Y1  ***************
#********************************************************
X <- iris[,-5]
I <- 1:150
Err <- matrix(nrow = 21, ncol = 3)


for(iLambda in seq(from = 0, to = 20)){
    #
    exp <- (+2 -4*(iLambda/20))
    xlambda <- 10^exp
   
    testErr <- 0.0
    trainErr <- 0.0
   
    for(ixval in seq(from = 1, to = 10)){
        Iout <- which(I%%10 == ixval - 1)
        #Xin <- X[-Iout,]
        Xin <- X[-Iout,]
        Xout <- X[Iout,]
        Yin <- Y1[-Iout]
        Yout <- Y1[Iout]
        dataIn <- cbind(Xin,Yin)
        mod <- lm.ridge(Yin~.,data=dataIn,lambda=xlambda)
        C <- mod$coef/mod$scales
        XM <- Xin
        for(i in seq(from = 1, to = ncol(Xin))){
            XM[,i]<-Xin[,i]-mod$xm[i]
        }
        XX <- as.matrix(XM)
        A <- as.array(C)
        Yh <- XX%*%A + mod$ym
        YhP <- Yh>=0.0
        Yp <- Yin>= 0.0
        #trainErr <- trainErr + sum(abs(Yin - Yh))/(nrow(as.matrix(Yin))*10)   
        trainErr <- trainErr + sum(YhP != Yp)/(length(Yin)*10)
        XM <- Xout
        for(i in seq(from = 1, to = ncol(Xout))){
            XM[,i]<-Xout[,i]-mod$xm[i]
        }
        XX <- as.matrix(XM)
        A <- as.array(C)
        Yh <- XX%*%A + mod$ym
        YhP <- Yh>=0.0
        Yp <- Yout>= 0.0
        testErr <- testErr + sum(YhP != Yp)/(length(Yout)*10)
    }
    Err[(iLambda+1),1] = trainErr
    Err[(iLambda+1),2] = testErr
    Err[(iLambda+1),3] = xlambda
}
plot(Err[,1], type='p', col='red', ylim=c(0,1),
        main = 'Error vs Log(Lambda)',
        ylab='Error Y1',
        xlab='3 - Log(Lambda)')
points(Err[,1], pch=15, col='red')
#lines(out[,2], type='l', col='blue')
points(Err[,2], pch=16, col='blue')
legend(5, 0.2, c("TRAIN", "TEST"), cex = 1, col = c("red", "blue"),
        pch = c(15, 16), lty = 1:2)

# Step 5.5 train on Y2 ************************
#*******************************************************************************
X <- iris[,-5]
I <- 1:150
Err <- matrix(nrow = 21, ncol = 3)


for(iLambda in seq(from = 0, to = 20)){
    #
    exp <- (+2 -4*(iLambda/20))
    xlambda <- 10^exp
   
    testErr <- 0.0
    trainErr <- 0.0
   
    for(ixval in seq(from = 1, to = 10)){
        Iout <- which(I%%10 == ixval - 1)
        #Xin <- X[-Iout,]
        Xin <- X[-Iout,]
        Xout <- X[Iout,]
        Yin <- Y2[-Iout]
        Yout <- Y2[Iout]
        dataIn <- cbind(Xin,Yin)
        mod <- lm.ridge(Yin~.,data=dataIn,lambda=xlambda)
        C <- mod$coef/mod$scales
        XM <- Xin
        for(i in seq(from = 1, to = ncol(Xin))){
            XM[,i]<-Xin[,i]-mod$xm[i]
        }
        XX <- as.matrix(XM)
        A <- as.array(C)
        Yh <- XX%*%A + mod$ym
        YhP <- Yh>=0.0
        Yp <- Yin>= 0.0
        #trainErr <- trainErr + sum(abs(Yin - Yh))/(nrow(as.matrix(Yin))*10)   
        trainErr <- trainErr + sum(YhP != Yp)/(length(Yin)*10)
        XM <- Xout
        for(i in seq(from = 1, to = ncol(Xout))){
            XM[,i]<-Xout[,i]-mod$xm[i]
        }
        XX <- as.matrix(XM)
        A <- as.array(C)
        Yh <- XX%*%A + mod$ym
        YhP <- Yh>=0.0
        Yp <- Yout>= 0.0
        testErr <- testErr + sum(YhP != Yp)/(length(Yout)*10)
    }
    Err[(iLambda+1),1] = trainErr
    Err[(iLambda+1),2] = testErr
    Err[(iLambda+1),3] = xlambda
}
plot(Err[,1], type='p', col='red', ylim=c(0,1),
        main = 'Error vs Log(Lambda)',
        ylab='Error Y2',
        xlab='3 - Log(Lambda)')
points(Err[,1], pch=15, col='red')
#lines(out[,2], type='l', col='blue')
points(Err[,2], pch=16, col='blue')
legend(5, 0.2, c("TRAIN", "TEST"), cex = 1, col = c("red", "blue"),
        pch = c(15, 16), lty = 1:2)


# Step 5.6 - train on Y3  *********************************
#*************************************************************
X <- iris[,-5]
I <- 1:150
Err <- matrix(nrow = 21, ncol = 3)


for(iLambda in seq(from = 0, to = 20)){
    #
    exp <- (+2 -4*(iLambda/20))
    xlambda <- 10^exp
   
    testErr <- 0.0
    trainErr <- 0.0
   
    for(ixval in seq(from = 1, to = 10)){
        Iout <- which(I%%10 == ixval - 1)
        #Xin <- X[-Iout,]
        Xin <- X[-Iout,]
        Xout <- X[Iout,]
        Yin <- Y3[-Iout]
        Yout <- Y3[Iout]
        dataIn <- cbind(Xin,Yin)
        mod <- lm.ridge(Yin~.,data=dataIn,lambda=xlambda)
        C <- mod$coef/mod$scales
        XM <- Xin
        for(i in seq(from = 1, to = ncol(Xin))){
            XM[,i]<-Xin[,i]-mod$xm[i]
        }
        XX <- as.matrix(XM)
        A <- as.array(C)
        Yh <- XX%*%A + mod$ym
        YhP <- Yh>=0.0
        Yp <- Yin>= 0.0
        #trainErr <- trainErr + sum(abs(Yin - Yh))/(nrow(as.matrix(Yin))*10)   
        trainErr <- trainErr + sum(YhP != Yp)/(length(Yin)*10)
        XM <- Xout
        for(i in seq(from = 1, to = ncol(Xout))){
            XM[,i]<-Xout[,i]-mod$xm[i]
        }
        XX <- as.matrix(XM)
        A <- as.array(C)
        Yh <- XX%*%A + mod$ym
        YhP <- Yh>=0.0
        Yp <- Yout>= 0.0
        testErr <- testErr + sum(YhP != Yp)/(length(Yout)*10)
    }
    Err[(iLambda+1),1] = trainErr
    Err[(iLambda+1),2] = testErr
    Err[(iLambda+1),3] = xlambda
}
plot(Err[,1], type='p', col='red', ylim=c(0,1),
        main = 'Error vs Log(Lambda)',
        ylab='Error Y3',
        xlab='3 - Log(Lambda)')
points(Err[,1], pch=15, col='red')
#lines(out[,2], type='l', col='blue')
points(Err[,2], pch=16, col='blue')
legend(5, 0.2, c("TRAIN", "TEST"), cex = 1, col = c("red", "blue"),
        pch = c(15, 16), lty = 1:2)


# Step 5.6 - Fit all three and claculate misclassification error
xlambda <- 0.15
Xin <- X
Yin <- Y1
dataIn <- cbind(Xin,Yin)
mod <- lm.ridge(Yin~.,data=dataIn,lambda=xlambda)
C <- mod$coef/mod$scales
XM <- Xin
for(i in seq(from = 1, to = ncol(Xin))){
    XM[,i]<-Xin[,i]-mod$xm[i]
}
XX <- as.matrix(XM)
A <- as.array(C)
Yh1 <- XX%*%A + mod$ym


Yin <- Y2
dataIn <- cbind(Xin,Yin)
mod <- lm.ridge(Yin~.,data=dataIn,lambda=xlambda)
C <- mod$coef/mod$scales
XM <- Xin
for(i in seq(from = 1, to = ncol(Xin))){
    XM[,i]<-Xin[,i]-mod$xm[i]
}
XX <- as.matrix(XM)
A <- as.array(C)
Yh2 <- XX%*%A + mod$ym

Yin <- Y3
dataIn <- cbind(Xin,Yin)
mod <- lm.ridge(Yin~.,data=dataIn,lambda=xlambda)
C <- mod$coef/mod$scales
XM <- Xin
for(i in seq(from = 1, to = ncol(Xin))){
    XM[,i]<-Xin[,i]-mod$xm[i]
}
XX <- as.matrix(XM)
A <- as.array(C)
Yh3 <- XX%*%A + mod$ym

#positive predictions by class
Y1h <- (Yh1 > Yh2) & (Yh1 > Yh3)
Y2h <-(Yh2> Yh1) & (Yh2>Yh3)
Y3h <- (Yh3> Yh2) & (Yh3>Yh1)

#number of correct positives in each class
sum(Y1>0 & Y1h)
sum(Y2>0 & Y2h)
sum(Y3>0 & Y3h)

