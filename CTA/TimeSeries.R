library(ggplot2)
library("RPostgreSQL")


drv <- dbDriver("PostgreSQL")
con <- dbConnect(drv, dbname="cta")
trips <- dbGetQuery(con,"select * from trips_trip")

# import trip data stored from the cta api
# trips <- read.csv(file="data/trips.csv", head=TRUE, sep=",")
# import the redline stops 
red.line <- read.csv(file="data/redline.csv", head=FALSE, sep=",")
# set column names to something different than what was from the db dump
colnames(trips) <- c("id", "station_id","stop_id", "station_name","stop_desc","run_number",
"route_id","destination_id","destination_name","route_direction_code", "prediction_generated",
 "expected_arrival","is_approaching","is_scheduled","is_fit","is_delayed")

colnames(red.line) <- c("stop_id", "direction_id", "stop_long_name", "lon", "lat", "station_name", "short_name", "parent_id")

# format the columns in each data frame to datatypes R can understand
# use posix data type rto store timestamp data
trips$prediction_generated <- as.POSIXlt(trips$prediction_generated, "%Y-%m-%d %H:%M:%S", tz="UTC")
trips$expected_arrival <- as.POSIXlt(trips$expected_arrival, "%Y-%m-%d %H:%M:%S", tz="UTC")

trips$station_name <- as.character(trips$station_name)
trips$destination_name <- as.character(trips$destination_name)
trips$stop_desc <- as.character(trips$stop_desc)


red.line$stop_long_name <- as.character(red.line$stop_long_name)
red.line$station_name <- as.character(red.line$station_name)
red.line$short_name <- as.character(red.line$short_name)

pred.time <- trips$prediction_generated
exp.arrival <- trips$expected_arrival

station.name <- trips$station_name
stop.direction <- trips$stop_desc
combined.name <- paste(station.name, stop.direction, sep=" ", collapse=NULL)

diff <- as.numeric(difftime(exp.arrival, pred.time, unit="sec"), units="secs")

all.trips <- data.frame(stop_description=combined.name, prediction_generated=pred.time,
	 			prediction_time=diff, stringsAsFactors=TRUE)


trips.plot<-ggplot(all.trips, aes(x=prediction_generated,y=prediction_time))+
 geom_line(aes(color="darkblue"))+ 
facet_wrap(~stop_description,nrow=10,ncol=7)+
theme_bw()+
scale_color_manual(values=c("darkblue"="darkblue"),legend=FALSE)+
xlab("Time Created")+
ylab("Predicted Arrival")+
opts(title="Data over time")

ggsave(plot=trips.plot, filename="./graphs/trips.pdf",width=30,height=20)



