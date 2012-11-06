#load in ggplot 2 library in order to make grpahs later on
library(ggplot2)


# read in data.csv
# head=TRUE imports header values from the csv file and sets them to column names
# set the seperator using the sep argument
baby.data <- read.csv(file="data/babies.txt", head=TRUE, sep=" ")

# print out the new data's columns
colnames(baby.data)

# if you want to change the column labels you can pass the colnames() function a vector of labels
colnames(baby.data) <- c("BirthWeight", "Gestation", "Parity", "Age", "Height", "Weight", "Smoke")

# you can use head() to print out the first 6 entries in the data tail
head(baby.data)

# tail returns the last 6
tail(baby.data)

# new column names
head(baby.data)


# you can also view one single column of the data frame by using the $ symbol. It will return a vector containing that column's data
# this is good use to when formatting data
head(baby.data$Weight)
# print out a summary of the imported data
summary(baby.data)
# str will print out the data type for each column in the data frame
# It looks like there are some null so we'll use na.omit() to remove them
str(baby.data)
baby.data <- na.omit(baby.data)
smokes <- baby.data$Smokes

# smokes.string <- > sapply(smokes, switch, 0='Does Not Smoke', 1='Does Smoke')

# baby.data$Smokes <- sapply(baby.data$Smokes, switch, 0='Does Not Smoke', 1='Does Smoke')

# smoking <-ifelse(smokes == 0 | smokes == 1,, "TRUE")


# start off with a ggplot() objhect then add on the other layers to build the graph
baby.plot<-ggplot(baby.data, aes(x=BirthWeight,y=Gestation,))+
geom_point(aes(colour=Smoke))+
theme_bw()+
ylab("Gestation Period In Days")+
xlab("Birth Weight in Ounces")+
opts(title="Baby Weight Compared to Gestation Period")+
# fit a line using a linear model (lm)
stat_smooth(method="lm")
ggsave(plot=baby.plot, filename="./graphs/babies/babies-smoking.pdf",width=8,height=9)

# To create separate graphs for the baby data based on the parity of their birth you can use the
# facet_wrap() function to break down the data into multiple graphs based on the value of a third column in the data frame.




sp + facet_grid(. ~ sex, labeller=mf_labeller)
baby.plot<-ggplot(baby.data, aes(x=BirthWeight,y=Gestation,))+
geom_point(aes(colour=Smoke))+
theme_bw()+
facet_wrap(~Parity, nrow = 6, ncol = 1) +
ylab("Gestation Period In Days")+
xlab("Birth Weight in Ounces")+
opts(title="Baby Weight Compared to Gestation Period")+
# fit a line using a linear model (lm)
stat_smooth(method="lm")
ggsave(plot=baby.plot, filename="./graphs/babies/babies-by-parity.pdf",width=8,height=9)



