library(ggplot2)
library("RPostgreSQL")
library(chron)

timeseriesgraph <- function(stopid, trips){
	one.stop <- na.omit(subset(trips, stop_id==stopid))
	min.time <- as.POSIXlt("2012-10-10 11:46:53")
	min.time <- times(format(min.time, "%H:%M:%S"))
	one.stop$prediction_generated_time <- times(format(one.stop$prediction_generated, "%H:%M:%S"))
	
	one.stop <- subset(one.stop, prediction_generated_time>min.time )
	
	print(head(one.stop))
	if(nrow(one.stop) > 0){
	
		pred.time <- one.stop$prediction_generated
		exp.arrival <- one.stop$expected_arrival
		
		station.name <- one.stop$station_name[1]
		stop.direction <- one.stop$stop_desc[1]

		diff <- as.numeric(difftime(exp.arrival, pred.time, unit="sec"), units="secs") /60
		# diff <- strptime(diff, "%H:%M:%S")
		plot.frame <- data.frame(prediction_generated=one.stop$prediction_generated_time , prediction_time=diff)
		
		#format combined name for this stop ids
		combined.name <- paste(station.name, stop.direction, sep=" ", collapse=NULL)
		combined.name <- gsub("/", "", combined.name, fixed = TRUE)
		combined.name <- gsub(" ", "", combined.name, fixed = TRUE)
		combined.name <- gsub("^\\s+|\\s+$", "", combined.name)
		
		print(stopid)
		print(head(plot.frame))
		stop.plot<-ggplot(plot.frame, aes(x=prediction_generated,y=prediction_time))+
		geom_point(aes(color="darkblue"))+ 
		theme_bw()+
		scale_color_manual(values=c("darkblue"="darkblue"),legend=FALSE)+
		xlab("Time Created")+
		ylab("Predicted Arrival")+
		opts(title=combined.name)

		ggsave(plot=stop.plot, filename=paste("./graphs/time_series_graphs/", combined.name,".pdf"),width=30,height=20)
	}
}

drv <- dbDriver("PostgreSQL")
con <- dbConnect(drv, dbname="cta")
trips <- na.omit(dbGetQuery(con,"select * from trips_trip where degree='0'"))

# trips <- na.omit(dbGetQuery(con,"select * from trips_trip where stop_id_id ='30106'"))

# import the redline stops 
redline <- na.omit(read.csv(file="data/redline.csv", head=FALSE, sep=","))
# set column names to something different than what was from the db dump
colnames(trips) <- c("id", "station_id","stop_id", "station_name","stop_desc","run_number",
"route_id","destination_id","destination_name","route_direction_code", "prediction_generated",
 "expected_arrival","is_approaching","is_scheduled","is_fit","degree", "is_delayed")

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
	timeseriesgraph(stopid, trips)
	# stopaverages[i,] <- stoprow
}

# 
# head(stopids)
# head(stopaverages)
# # plot the average time for each L stop
# ggplot(stopaverages, aes(x = stop_name, y = avg, width=.3)) +
# geom_bar(colour="black", stat="identity", width=2, position="dodge") +
# scale_x_discrete(labels = stopaverages$stop_name) +
# scale_y_discrete(breaks = 1:10) +
# opts(axis.text.y=theme_text(size=6))+
# coord_flip()




