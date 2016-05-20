#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[])
{
char hostname[1024];
hostname[1023]='\0';
gethostname(hostname, 1023);
puts(hostname);
return 0;
}
