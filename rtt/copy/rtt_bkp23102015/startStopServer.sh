#/bin/bash
#####################################################################
#Description: This script starts or stops file server 
#Author: Sri Harsha Chennavajjala
#Date Modified: 30 Sep 2015
#Version: 1.0
#Inputs: Script accepts one parameter start|stop
#Syntax: ./startStopServer.sh start 
#	 ./startStopServer.sh stop
#Output: This script won't return any object but will display 
#	 some messages on stdout
######################################################################

if [[ -z "$1" ]]; then
echo "Wrong input";
exit;
fi

if [ $1 = "start" ]; then
	pgrep serverv5 > /dev/null;
	if [ $? -eq 0 ]; then
		echo "Server is already running";
		exit;
	else
		echo "Starting the server...";
		cd /home/umkc_sc9v9/rtt/;
		nohup ./serverv5 & > server.log;
		sleep 1;
		pgrep serverv5 > /dev/null;
		if [ $? -eq 0 ]; then
			echo "Server started successfully";
			exit;
		else
			echo "Unable to start the server";
			exit;
		fi
	fi
exit;
fi

if [ $1 = "stop" ]; then
	pgrep serverv5 > /dev/null;
	if [ $? -ne 0 ]; then
		echo "Server is not running"; 
		exit;
	else 
		echo "Stopping the server...";
		pid=`pgrep serverv5`;
		kill -9 $pid;
		sleep 1;
		pgrep serverv5 > /dev/null;
		if [ $? -ne 0 ]; then
			echo "Server stopped successfully";     
			exit;
		else 
			echo "Unable to stop the server...";
			exit;
		fi
	fi
fi

if [ $1 != "stop" ] && [ $1 != "start" ]; then
	echo "Wrong input";
fi
