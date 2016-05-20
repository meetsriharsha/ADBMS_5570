#/bin/bash
#####################################################################
#Description: This script starts the client application
#Author: Sri Harsha Chennavajjala
#Date Modified: 30 Sep 2015
#Version: 1.0
#Inputs: Script accepts no parameter
#	 This script requires nodesList.txt file
#Syntax: ./startClient.sh 
#Output: This script won't return any object but will display 
#	 some messages on stdout
######################################################################

pgrep clientv10 > /dev/null;
if [ $? -eq 0 ]; then
	echo "client is already running";
	exit;
else
	echo "Starting the client...";
	rm /home/umkc_sc9v9/rtt/client/RTT_log* > /dev/null;
	sort -R /home/umkc_sc9v9/rtt/nodesList.txt | grep -v ^# | while read node
	do
		cd /home/umkc_sc9v9/rtt/client/;
		echo "Collecting from $node" >> client.log;
		./clientv10 $node >> client.log;
	done
fi
exit;
