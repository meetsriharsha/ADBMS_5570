#!/bin/bash
#####################################################################
#Description: This script calls another script on the remote node
#	      to start or stop the client application
#Author: Sri Harsha Chennavajjala
#Date Modified: 30 Sep 2015
#Version: 1.0
#Inputs: Script accepts one parameter start|stop
#	 This script requires nodesList.txt file
#Syntax: ./client.sh start 
#	 ./client.sh stop
#Output: This script won't return any object but will display 
#	 some messages on stdout
######################################################################

if [[ -z "$1" ]]; then
echo "Invalid input, start or stop?";
exit;
fi

if [ $1 = "start" ]; then
awk '$0 !~ /^#/ {print "echo Server: "$1";ssh -t -o connectTimeout=10 umkc_sc9v9@"$1" \"nohup sh /home/umkc_sc9v9/rtt/startClient.sh & > startClient.log;exit\""}' nodesList.txt > startclient.sh;
sh startclient.sh;
sleep 1;
rm startclient.sh;
exit;
fi

if [ $1 = "stop" ]; then
awk '$0 !~ /^#/ {print "echo Server: "$1";ssh -t -o connectTimeout=10 umkc_sc9v9@"$1" \"nohup sh /home/umkc_sc9v9/rtt/stopClient.sh & > stopClient.log;exit\""}' nodesList.txt > stopclient.sh;
sh stopclient.sh;
sleep 1;
rm stopclient.sh;
exit;
fi
