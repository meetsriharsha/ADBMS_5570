#!/bin/sh
#####################################################################
#Description: This script stops the client application 
#Author: Sri Harsha Chennavajjala
#Date Modified: 30 Sep 2015
#Version: 1.0
#Inputs: Script accepts no parameter
#Syntax: ./stopClient.sh 
#Output: This script won't return any object but will display 
#	 some messages on stdout
######################################################################

while [ 1=1 ] 
do
pid=`pgrep clientv8`;
echo "PID: $pid";
if [[ -z "$pid" ]]; then
echo "Client stopped successfully";
exit;
else
kill -9 $pid;
sleep 1;
fi
done
