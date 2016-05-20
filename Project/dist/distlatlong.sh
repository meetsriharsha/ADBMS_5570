while read line
do
while read line2
do
source=`echo $line | awk -F"," '{print $1}'`
dst=`echo $line2 | awk -F"," '{print $2}'`
latlong1=`echo $line | awk -F"," '{print $3"#"$4}'`
latlong2=`echo $line2 | awk -F"," '{print $3"#"$4}'`
echo "$source,$dst,$latlong1,$latlong2";
done < harnodes.txt
done < harnodes.txt
