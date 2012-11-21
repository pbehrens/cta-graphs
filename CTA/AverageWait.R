library(ggplot2)

averagetime <- function(stopid, trips){
	onestop <- subset(trips, stop_id==stopid)

	predtime <- onestop$prediction_generated
	exparrival <- onestop$expected_arrival
	station_name <- onestop$station_name[1]
	stop_direction <- onestop$stop_desc[1]
	
	combinedname <- paste(station_name, stop_direction, sep=" ", collapse=NULL)

	diff <- difftime(exparrival, predtime, unit="sec")
	avg <- mean(diff)
	avg <- round(avg / 60, digits = 3)
	onestopdata <- c(stopid, combinedname, avg)
	return (onestopdata)
}


# import trip data stored from the cta api
trips <- read.csv(file="../data/trips.csv", head=TRUE, sep=",")
# import the redline stops 
redline <- read.csv(file="../data/redline.csv", head=FALSE, sep=",")
# set column names to something different than what was from the db dump
colnames(trips) <- c("id", "station_id","stop_id", "station_name","stop_desc","run_number",
"route_id","destination_id","destination_name","route_direction_code", "prediction_generated",
 "expected_arrival","is_approaching","is_scheduled","is_fit","is_delayed")

colnames(redline) <- c("stop_id", "direction_id", "stop_long_name", "lon", "lat", "station_name", "short_name", "parent_id")

# format the columns in each data frame to datatypes R can understand
# use posix data type rto store timestamp data
trips$prediction_generated <- as.POSIXlt(trips$prediction_generated, "%Y-%m-%d %H:%M:%S", tz="UTC")
trips$expected_arrival <- as.POSIXlt(trips$expected_arrival, "%Y-%m-%d %H:%M:%S", tz="UTC")

trips$station_name <- as.character(trips$station_name)
trips$destination_name <- as.character(trips$destination_name)
trips$stop_desc <- as.character(trips$stop_desc)


redline$stop_long_name <- as.character(redline$stop_long_name)
redline$station_name <- as.character(redline$station_name)
redline$short_name <- as.character(redline$short_name)

# create data frame with dimensions of all the redline stops 
N <- length(redline$stop_id)
stopids <- redline$stop_id
stopaverages <- data.frame(stop_id=rep(NA, N), stop_name=rep("", N), avg=rep(NA, N), 
                 stringsAsFactors=FALSE)
# iterate through all of the stops and find the average time for a subset of the trips
for(i in 1:N) {	
	stopid <- stopids[i]
	stoprow <- averagetime(stopid, trips)
	stopaverages[i,] <- stoprow
}


head(stopids)
head(stopaverages)
# plot the average time for each L stop
avg.plot <- ggplot(stopaverages, aes(x = stop_name, y = avg, width=.3)) +
geom_bar(colour="black", stat="identity", width=2, position="dodge") +
scale_x_discrete(labels = stopaverages$stop_name) +
scale_y_discrete(breaks = 1:10) +
opts(axis.text.y=theme_text(size=6))+
coord_flip()

ggsave(plot=avg.plot, filename="../graphs/trips.pdf",width=30,height=20)





