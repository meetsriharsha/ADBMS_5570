#!/bin/sh
#####################################################################
#Description: This script collects log files from all nodes and 
#	      merges all log files and creates one file 
#Author: Sri Harsha Chennavajjala
#Date Modified: 30 Sep 2015
#Version: 1.0
#Inputs: Script accepts no parameters
#	 This script requires nodesList.txt file
#Syntax: ./loadDownloadDetails.sh 
#Output: This script won't return any object but will display 
#	 some messages on stdout
######################################################################

echo "Collecting log files from all nodes...";
while read node
do
scp -o connectTimeout=10 umkc_sc9v9@$node:/home/umkc_sc9v9/rtt/client/RTT_log_$node.log ./
done < nodesList.txt

echo "Merging all files";
cat RTT_* > downloaddetails.csv;

./loadDownloadDetails.sql
