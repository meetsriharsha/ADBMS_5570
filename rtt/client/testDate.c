#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

main(int argc,char *argv[])
{
	char endTime[4][26];
	time_t timere;
	struct tm *tm_infoe;
	double dlay[4][3];
	time(&timere);
	tm_infoe = localtime(&timere);
	strftime(endTime[1], 26, "%Y%m%d%H%M%S", tm_infoe);
	printf("Endtime %s\n", endTime[1]);
	dlay[1][1]=atol(endTime[1]);
	printf("Value from atol:%ld\n", atol(endTime[1]));
	dlay[1][1]=atof(endTime[1]);
	printf("Float value from atof:%f\n", dlay[1][1]);
	sscanf(endTime[1], "%lf", &dlay[1][1]);
	printf("Float value from sscanf:%f\n", dlay[1][1]);
}
