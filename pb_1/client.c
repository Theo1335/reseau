#include "header_Exemple.h"



int main(int argc, char* argv[]) {

    if (argc < 2)
    {
        
        perror("vous devez specifier au moins un message");
        exit(EXIT_FAILURE);
    }
    

    struct sockaddr_in addr; 
    int sfd;
    char buf[BUF_SIZE];
    //socklen_t serv_len = sizeof(addr);
    ssize_t numRead;

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sfd == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET; 
    // strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path)-1); 

    if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    addr.sin_port = htons(port);

    if (connect(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    sleep(2);

    for (int i = 1; i < argc; i++)
    {
        snprintf(buf, BUF_SIZE, "%s",argv[i]);


        if(send(sfd, buf, strlen(buf) + 1, 0) == -1){
            perror("ERREUR sendto");
            exit(EXIT_FAILURE);
        }

        //printf("Message envoyé au serveur: %s\n", buf);

        numRead = recv(sfd, buf, BUF_SIZE, 0);
        if(numRead == -1){
            perror("ERREUR recv");
            exit(EXIT_FAILURE);
        }

        buf[numRead] = '\0';
        printf("%s ",buf);
        //printf("%s\n", buf);
    }
    
    printf("\n");
    close(sfd);

    return 0;
}
