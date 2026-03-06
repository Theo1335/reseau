#include <sys/un.h> //sockaddr_un
#include <stdio.h> //perror
#include <stdlib.h> //exit
#include <unistd.h> //read, write etc
//#include <fcntl.h> //O_RDONLY etc
#include <sys/socket.h> //sockets
#include <arpa/inet.h> // pour inet_addr (loopback)
#include <pthread.h>
#include <stdint.h>
#include <string.h>

//#define SV_SOCK_PATH "/tmp/mysock" // inutile ici car ps de descripteur de fichier

#define BUF_SIZE 100
#define port 12345
