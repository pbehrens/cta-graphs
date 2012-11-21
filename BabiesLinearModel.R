source("Babies.R")
head(baby.data)
# input data frame and seed for random value to split up data frame
splitdf <- function(dataframe, seed=NULL) {
    if (!is.null(seed)) set.seed(seed)
	#take index from 1 to the length of the data frame
    index <- 1:nrow(dataframe)
	#find training set index
    trainindex <- sample(index, trunc(length(index)/2))
	# assign training and test set
    trainset <- dataframe[trainindex, ]
    testset <- dataframe[-trainindex, ]
    list(trainset=trainset,testset=testset)
}

two.sets <- splitdf(baby.data, seed=NULL)

train.data <- two.sets$trainset
test.data <- two.sets$testset

#create a linear model of the baby data trying to predict birthweight giv en other factors
baby.model <- lm(formula = BirthWeight ~ Gestation, data = train.data)

#show coefficients for the model
coefficients(baby.model)

# create a linear fit line for the model using the test data
lm.fit <- predict(baby.model, newdata = test.data)

# Difference between original labels and lm.fit = residuals 
summary((train.data-lm.fit) - baby.model$residuals)

# find the root mean squared error to see the sample standard deviation
rmsError <- sqrt(sum(baby.model$residuals * baby.model$residuals)/length(baby.model$residuals))

print("rmsError = ")
rmsError

# the RMS error is around 16 so this means on average the prediction will be off by around 16 ounces of the birth weight

