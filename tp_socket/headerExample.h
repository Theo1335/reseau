#include <sys/un.h> //sockaddr_un
#include <stdio.h> //perror
#include <stdlib.h> //exit
#include <unistd.h> //read, write etc
#include <fcntl.h> //O_RDONLY etc
#include <sys/socket.h> //sockets
#include <sys/wait.h>
#include <string.h> //memset, strncpy, strlen, strcpy


#define SV_SOCK_PATH "/tmp/mysock"

#define BUF_SIZE 100
