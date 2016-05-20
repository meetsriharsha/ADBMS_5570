#!/bin/bash

while read node `cat nodesList.txt`
do
while read  fileName
do
echo "clientv6 ${node} ${fileName}";
done<filesList.txt
done<nodesList.txt

