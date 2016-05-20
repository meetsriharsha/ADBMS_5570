#/bin/bash
#####################################################################
#Description: This script calls another script on the remote node
#	      to start or stop the server application
#Author: Sri Harsha Chennavajjala
#Date Modified: 30 Sep 2015
#Version: 1.0
#Inputs: Script accepts one parameter start|stop
#	 This script requires nodesList.txt file
#Syntax: ./server.sh start 
#	 ./server.sh stop
#Output: This script won't return any object but will display 
#	 some messages on stdout
######################################################################

if [[ -z $1 ]]; then
echo "Wrong input. Syntax: startserver.sh start|stop";
exit;
fi

if [ $1 = "start" ]; then
awk '$0 !~ /^#/ {print "echo Server: "$1";ssh -t -o connectTimeout=10 umkc_sc9v9@"$1" \"sh /home/umkc_sc9v9/rtt/startStopServer.sh start;exit\""}' nodesList.txt > start.sh;
sh start.sh;
sleep 1;
rm start.sh;
else
awk '$0 !~ /^#/ {print "echo Server: "$1";ssh -o connectTimeout=10 umkc_sc9v9@"$1" \"export TERM=xterm;sh /home/umkc_sc9v9/rtt/startStopServer.sh stop;exit\""}' nodesList.txt > stop.sh;
sh stop.sh;
sleep 1;
rm stop.sh;
fi
