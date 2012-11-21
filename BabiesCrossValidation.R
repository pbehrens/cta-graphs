source("Babies.R")

# first we are going to split the baby.data daata frame into two seperate parts so we can train the linear then test it

#function to split data frame into two parts for training and testing
splitdf <- function(dataframe, seed=NULL) {
    if (!is.null(seed)) set.seed(seed)
    index <- 1:nrow(dataframe)
    trainindex <- sample(index, trunc(length(index)/2))
    trainset <- dataframe[trainindex, ]
    testset <- dataframe[-trainindex, ]
    list(trainset=trainset,testset=testset)
}
two.sets <- splitdf(baby.data, seed=NULL)

train.data <- two.sets$trainset
test.data <- two.sets$testset

all.data<- rbind(train.data,test.data)
head(train.data)
head(test.data)
nxval <- 10

out <- matrix(nrow = nxval, ncol = 2)
I <- seq(from = 1, to = nrow(all.data))

# number of folds for cross validation 
trainErr <- 0.0
testErr <- 0.0
# 10 iterations of 2 fold validation
for(ixval in seq(from =  1, to = nxval)){
	# select one of 10 partitions of the data
	Iout <- which(I%%nxval == ixval%%nxval)
	
	# set d0 and d1 to train and test		
	d0 <- all.data[-Iout,]
	d1 <- all.data[Iout,]
	
	yin <- d0[,7]
	yout <- d1[,7]

	xin <- d0[,1:7]
	xout <- d1[,1:7]
	# create first linear model that will be tested on 
	train.model <- lm(formula = BirthWeight~., data=d0)
	
	# compute training and test error for this partition using the regression
	tr <- sum(abs(yin - predict(train.model,xin)))/(nrow(as.matrix(yin))) 
	te<-sum(abs(yout - predict(train.model,xout)))/(nrow(as.matrix(yout)))
	
	#add error for this partition to overall training and test error 
	trainErr <- trainErr + tr/nxval
	testErr <- testErr + te/nxval
	
	#store each iterations training and test error	
	out[ixval,1] <- tr
	out[ixval,2] <- te
}


print(out)
print(trainErr)
print(testErr)


# create first linear model that will be tested on 
# train.model <- lm(formula = BirthWeight~., data=train.data)
# str(train.model)
# coefficients(train.model)
# str(train.data)

# test.model <- lm(formula = BirthWeight~., data=test.data)
# coefficients(train.model)
# str(test.data)

# lm.fit <-predict(train.model, newdata=test.data)

# see diff between original and the lmfit prediction
# summary((test.data))
# 
# 
# # look at diff be regulars and lmfit model 
# 
# summary((train.data-train.model[12]) - train.model$residuals)
