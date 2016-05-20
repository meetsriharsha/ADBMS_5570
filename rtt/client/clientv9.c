

/* Client side of an ftp service.  Actions:
   - connect to server and request service
   - send size-of-sile info to server
   - start receiving file from server
   - close connection
History: v8: Looping the program to collect all 5 files.
         v9: Looping the program to calculate average delay.
*/


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

#define SERVER_PORT_ID 60841 
#define CLIENT_PORT_ID 0
#define SERVER_HOST_ADDR argv[1]
                          /*149.43.80.20*/
                         
#define MAXSIZE 512

#define ACK                   2
#define NACK                  3
#define REQUESTFILE           100
#define COMMANDNOTSUPPORTED   150
#define COMMANDSUPPORTED      160
#define BADFILENAME           200
#define FILENAMEOK            400
#define STARTTRANSFER         500
int readn(int sd,char *ptr,int size);
int writen(int sd,char *ptr,int size);

int hostname_to_ip(char *hostname , char *ip)
{
    int sockfd;  
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_in *h;
    int rv;
 
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // FOR IPV6 OR IPV4 SPECIFICATION
    hints.ai_socktype = SOCK_STREAM;
 
    if ( (rv = getaddrinfo( hostname , "http" , &hints , &servinfo)) != 0) 
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
 
    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) 
    {
        h = (struct sockaddr_in *) p->ai_addr;
        strcpy(ip , inet_ntoa( h->sin_addr ) );
    }
     
    freeaddrinfo(servinfo); // all done with this structure
    return 0;
}

  main(int argc,char *argv[])
  
{
    int i,j,k,n,len; 
    pid_t pid;
    char startTime[4][26], endTime[4][26];
    FILE *lfp;
    char filname[100]; //log filename
    char filnm[100];   // filenames with different sizes
    char hstname[100];   
    long int milsecS, milsecE;
    float delay, swp, totalDelay=0, avgDelay;
    char delayS[30];
    int itr=4; //Iterator for looping each file collection
    hstname[99]='\0';
	time_t timers, timere;
    struct tm *tm_infos, *tm_infoe;
    char mesg[1000];
    float dlay[itr];
   

    for(i=1;i<=5;i++)
    {
     totalDelay=0;
     avgDelay=0;
     if(i==1)
     strcpy(filnm, "file32B.txt");
     else if(i==2)
     strcpy(filnm, "file1KB.txt");
     else if(i==3)
     strcpy(filnm, "file256KB.txt");
     else if(i==4)
     strcpy(filnm, "file512KB.txt");
     else if(i==5)
     strcpy(filnm, "file1MB.txt");
     printf("Filename is: %s\n", filnm);
     len=strlen(filnm)+1;
    
   /* GETTING CURRENT HOSTNAME */ 
    gethostname(hstname, 99);
    
    /* CREATING LOG MESSAGES */
    /*resetting mesg*/
    strcpy(mesg, "");
    strcat(mesg, argv[1]);
    strcat(mesg, ",");
    strcat(mesg, hstname);
    strcat(mesg, ",");
    strcat(mesg,filnm);
    strcat(mesg, ",");
   
    /* START TIME */ 
    struct timeval tvBegin, tvEnd;
    struct timeval *tvB, *tvE;
   
    for(j=0;j<itr;j++)
    { 
    
    //strcat(mesg, "\n Start time: ");
    //strcat(mesg, buffer); 
    //printf("%s\n", mesg);
  
    /* CREATING FILE NAME DYNAMICALLY */ 
    strcpy(filname, "RTT_log_");
    strcat(filname, hstname);
    strcat(filname, ".log");
    int sockid, newsockid,i,getfile,ack,msg,msg_2,c;
    int no_writen,start_xfer, num_blks,num_last_blk;
    struct sockaddr_in my_addr, server_addr; 
    FILE *fp; 
    char in_buf[MAXSIZE];
    char ip[100];
    if(argc != 2) {
	printf("error: usage : sftp hostname \n"); 
	exit(0);
	}
    no_writen = 0;
    num_blks = 0;
    num_last_blk = 0;
    //len = strlen(argv[2])+1;
    //printf("File name is: %s\n", argv[2]);
    printf("Hostname is: %s\n", argv[1]);
    printf("client: creating socket\n");
    if ((sockid = socket(AF_INET,SOCK_STREAM,0)) < 0)
      { printf("client: socket error : %d\n", errno); exit(0);
          }
  
    printf("client: binding my local socket\n");
    bzero((char *) &my_addr,sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_addr.sin_port = htons(CLIENT_PORT_ID);
    if (bind(sockid ,(struct sockaddr *) &my_addr,sizeof(my_addr)) < 0)
           {printf("client: bind  error :%d\n", errno);
           
    }
                                             
    printf("client: starting connect\n");
    bzero((char *) &server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    char *hostname=argv[1];
    hostname_to_ip(hostname , ip);
    printf("IP is: %s\n", ip);
    server_addr.sin_addr.s_addr = inet_addr(ip);
   // printf("address is:%s\n", server_addr.sin_addr.s_addr);
    server_addr.sin_port = htons(SERVER_PORT_ID);
     if (connect(sockid ,(struct sockaddr *) &server_addr,
                                             sizeof(server_addr)) < 0)
           {printf("client: connect  error :%d\n", errno); exit(0);
           }


  /* Once we are here, we've got a connection to the server */

    /* tell server that we want to get a file */
    getfile = htons(REQUESTFILE);
    printf("client: sending command request to ftp server\n");
    if((writen(sockid,(char *)&getfile,sizeof(getfile))) < 0)
       {printf("client: write  error :%d\n", errno); exit(0);} 

    /* want for go-ahead from server */
    msg = 0;  
    if((readn(sockid,(char *)&msg,sizeof(msg)))< 0)
       {printf("client: read  error :%d\n", errno); exit(0); }
    msg = ntohs(msg);   
    if (msg==COMMANDNOTSUPPORTED) {
	 printf("client: server refused command. goodbye\n");
         exit(0);
         }
       else
         printf("client: server replied %d, command supported\n",msg);
    

    /* send file name to server */
    printf("client: sending filename\n");
       //if ((writen(sockid,argv[2],len))< 0)
       if ((writen(sockid,filnm,len))< 0)
         {printf("client: write  error :%d\n", errno); exit(0);}
    /* see if server replied that file name is OK */
    msg_2 = 0;
    if ((readn(sockid,(char *)&msg_2,sizeof(msg_2)))< 0)
        {printf("client: read  error :%d\n", errno); exit(0); }   
    msg_2 = ntohs(msg_2);
    if (msg_2 == BADFILENAME) {
       printf("client: server reported bad file name. goodbye.\n");
       exit(0);
       }
     else
         printf("client: server replied %d, filename OK\n",msg_2);


    printf("client: sending start transfer command\n");
    start_xfer = STARTTRANSFER;
    start_xfer = htons(start_xfer);
    if ((writen(sockid,(char *)&start_xfer,sizeof(start_xfer)))< 0)
           {printf("client: write  error :%d\n", errno); exit(0);
           }
    //if ((fp = fopen(argv[2],"w")) == NULL)
    if ((fp = fopen(filnm,"w")) == NULL)
        {printf(" client: local open file error \n");exit(0);}
            
    

     if((readn(sockid,(char *)&num_blks,sizeof(num_blks))) < 0)
             {printf("client: read error on nblocks :%d\n",errno);exit(0);}
     num_blks = ntohs(num_blks);
     printf("client: server responded: %d blocks in file\n",num_blks);
     ack = ACK;  
     ack = htons(ack);
     if((writen(sockid,(char *)&ack,sizeof(ack))) < 0)
        {printf("client: ack write error :%d\n",errno);exit(0);
        }

   
     if((readn(sockid,(char *)&num_last_blk,sizeof(num_last_blk))) < 0)
             {printf("client: read error :%d on nbytes\n",errno);exit(0);}
     num_last_blk = ntohs(num_last_blk);  
     printf("client: server responded: %d bytes last blk\n",num_last_blk);
     if((writen(sockid,(char *)&ack,sizeof(ack))) < 0)
        {printf("client: ack write error :%d\n",errno);exit(0);
        }


  /* BEGIN READING BLOCKS BEING SENT BY SERVER */
  long int schDelay=ntohl(schDelay);
   printf("Schedule delay is: %ld\n", schDelay);
  printf("client: starting to get file contents\n");
    gettimeofday(&tvBegin, NULL);
    tvB=&tvBegin;
    milsecS=(tvB->tv_sec)*1000000L-tvB->tv_usec;
    printf("Start in milliseconds: %ld\n", milsecS);

    //time_t timers, timere;
    //struct tm *tm_infos, *tm_infoe;
    time(&timers);
    tm_infos = localtime(&timers);
    strftime(startTime[j], 26, "%Y/%m/%d %H:%M:%S", tm_infos);
	strcat(mesg, startTime[j]);
	strcat(mesg, ",");
    printf("Start time of %d transfer:%s\n", j, startTime[j]);
    for(i= 0; i < num_blks; i ++) {
      if((readn(sockid,in_buf,MAXSIZE)) < 0)
	  {printf("client: block error read: %d\n",errno);exit(0);}
      no_writen = fwrite(in_buf,sizeof(char),MAXSIZE,fp);
      if (no_writen == 0) {printf("client: file write error\n");exit(0);}
      if (no_writen != MAXSIZE) 
         {printf("client: file write  error : no_writen is less\n");exit(0);}
      /* send an ACK for this block */
      if((writen(sockid,(char *)&ack,sizeof(ack))) < 0)
         {printf("client: ack write  error :%d\n",errno);exit(0);}
      printf(" %d...",i);
      }


/*IF THERE IS A LAST PARTIALLY FILLED BLOCK, READ IT */

    if (num_last_blk > 0) {
        printf("%d\n",num_blks);      
        if((readn(sockid,in_buf,num_last_blk)) < 0)
           {printf("client: last block error read :%d\n",errno);exit(0);}
        no_writen = fwrite(in_buf,sizeof(char),num_last_blk,fp); 
        if (no_writen == 0) 
          {printf("client: last block file write err :%d\n",errno);exit(0);}
        if (no_writen != num_last_blk) 
        {printf("client: file write error : no_writen is less 2\n");exit(0);}
        if((writen(sockid,(char *)&ack,sizeof(ack))) < 0)
	         {printf("client :ack write  error  :%d\n",errno);exit(0);}
        }
      else printf("\n");
      

  /*FILE TRANSFER ENDS. CLIENT TERMINATES AFTER  CLOSING ALL ITS FILES
    AND SOCKETS*/ 
    
   // strftime(buffer, 26, "%Y/%m/%d %H:%M:%S", tm_infoe);
    //strcat(mesg, "\n End time: ");
    //strcat(mesg, ",");
    //strcat(mesg, buffer);
    gettimeofday(&tvEnd, NULL);
    tvE=&tvEnd;
    milsecE=(tvE->tv_sec)*1000000L-tvE->tv_usec;
	time(&timere);
    tm_infoe = localtime(&timere);
    printf("End time in milliseconds: %ld\n", milsecE);
	strftime(endTime[j], 26, "%Y/%m/%d %H:%M:%S", tm_infoe);
    strcat(mesg, endTime[j]);
	strcat(mesg, ",");
    printf("End time of %d transfer:%s\n", j, endTime[j]);
    fclose(fp);
    printf("client: FILE TRANSFER COMPLETE\n");
    close(sockid);
    
    delay=fabs((milsecE-milsecS)/1000);
    //printf("Without fabs Delay is: %ld\n",(milsecE-milsecS)/1000); 
    printf("Delay is: %f\n", delay);
    dlay[j]=delay;
    }
    for (k=0;k<itr-1;k++)
    {
        for (n=0;n<itr-k-1;n++)
        {
            if (dlay[n] > dlay[n+1])
            {
                swp =  dlay[n];
                dlay[n] = dlay[n+1];
                dlay[n+1] = swp;
            }
        }
    }
    for(k=0;k<itr-1;k++)
    {
    snprintf(delayS, 30, "%f", dlay[k]);
    strcat(mesg, delayS);
    strcat(mesg, ",");
      printf("%f\n",dlay[k]);
    }
    totalDelay=dlay[0]+dlay[1]+dlay[2]; 
    printf("Total delay is: %f\n", totalDelay);
    avgDelay=totalDelay/3;
    printf("Average delay is: %f\n", avgDelay);
    snprintf(delayS, 30, "%f", avgDelay);
    //strcat(mesg, "\n Delay is: ");
    strcat(mesg, delayS);
    strcat(mesg, "\n"); 
    
    printf("%s\n", mesg);

   /* WRITING STATISTICS TO LOG FILE*/
    lfp=fopen(filname, "a");
    fputs(mesg, lfp);
    fclose(lfp); 
}
}
    
  

int readn(int sd,char *ptr,int size)

{         int no_left,no_read;
          no_left = size;
          while (no_left > 0) 
                     { no_read = read(sd,ptr,no_left);
                       if(no_read <0)  return(no_read);
                       if (no_read == 0) break;
                       no_left -= no_read;
                       ptr += no_read;
                     }
          return(size - no_left);
}


int writen(int sd,char *ptr,int size)
{         int no_left,no_written;
          no_left = size;
          while (no_left > 0) 
                     { no_written = write(sd,ptr,no_left);
                       if(no_written <=0)  return(no_written);
                       no_left -= no_written;
                       ptr += no_written;
                     }
          return(size - no_left);
}
