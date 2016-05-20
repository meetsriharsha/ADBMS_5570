#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <math.h>

main(int argc,char *argv[])
  
{
    int i,j,k,n,q,len; 
   int itr=4;
   float swp;
   float dlay[4][3]={{1,2,9},{4,5,6},{7,8,9},{10,11,3}};
for(k=0;k<itr-1;k++)
    {
      for(n=0;n<itr-k-1;n++)
      {
        if(dlay[n][2]>dlay[n+1][2])
	{
	  for(q=0;q<3;q++)
	  {
	  swp = dlay[n][q];
	  dlay[n][q]=dlay[n+1][q];
	  dlay[n+1][q]=swp;
	  }
	}
       }
     }
for(k=0;k<itr;k++)
{
printf("%f     %f     %f\n", dlay[k][0],dlay[k][1],dlay[k][2]);
}
struct tm tm;
    char buf[255];

    memset(&tm, 0, sizeof(struct tm));
    strptime("1445216095567872", "%s", &tm);
    strftime(buf, sizeof(buf), "%b %d %H:%M %Y", &tm);
    puts(buf); /* -> Feb 09 20:09 2013 */
  char dat[20]="20151018213709";
  printf("Float value is:%f\n", atof(dat));
}
