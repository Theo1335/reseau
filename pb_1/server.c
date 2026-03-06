#include "header_Exemple.h"

void* multiClient(void *param);

struct client_parametre_ptrh {
    int cfd;  
    struct sockaddr_in client_addr;  
};

int main(void){
    // unlink(SV_SOCK_PATH) 

    struct sockaddr_in svaddr;
    struct sockaddr_in client_addr; 
    int sfd, cfd;
    //ssize_t numRead;
    //char buf[BUF_SIZE];
    socklen_t client_len = sizeof(client_addr);
    pthread_t trid;


    struct client_parametre_ptrh *parametre = malloc(sizeof(struct client_parametre_ptrh));  
    if (parametre == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    sfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sfd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&svaddr, 0, sizeof(struct sockaddr_in));  
    svaddr.sin_family = AF_INET; 

    if (inet_pton(AF_INET, "127.0.0.1", &svaddr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    //svaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    //strncpy(server_addr.sin_path, SV_SOCK_PATH, sizeof(server_addr.sin_path) - 1);
    svaddr.sin_port = htons(port); 

    if (bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_in)) == -1){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(sfd, 10) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf(" \n");
    printf("Server Pret: en attente de connexions...\n");
    printf(" \n");

    for(;;)
    {   

        cfd = accept(sfd, (struct sockaddr *) &client_addr, &client_len);
        if (cfd == -1) {
            perror("accept");
            exit(EXIT_FAILURE); 
        }

        parametre->cfd = cfd;
        parametre->client_addr = client_addr;

        if(pthread_create(&trid, NULL, multiClient, (void*)parametre) != 0){
            perror("pthread");
        }
        else{
            pthread_detach(trid);
        }



    }
    
    printf("arret du server...\n");
    close(sfd);
    //unlink(SV_SOCK_PATH);

    return 1;
}



void* multiClient(void *param){
    struct client_parametre_ptrh *parametre = (struct client_parametre_ptrh*)param; 
    int cfd = parametre->cfd;
    struct sockaddr_in client_addr = parametre->client_addr;
    ssize_t numRead;
    char buf[BUF_SIZE];

    for(;;){
        numRead = recv(cfd, buf, BUF_SIZE, 0); 
        if(numRead == -1)
        {
            perror("ERRUER recv");
            exit(EXIT_FAILURE);
        }
        else if(numRead == 0){

            close(cfd);
            break;

        }
        
        buf[numRead] = '\0';


        printf("Message recu: %s, nb d'octets: %ld \n", buf, numRead);
        printf("client: ip: %s, port: %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        printf("Renvoie du message: \n");
        
        if (send(cfd, buf, strlen(buf) + 1, 0) == -1){
            perror("ERREUR sendto");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("@echo: message envoye avec succes\n");
            printf(" \n");
        }

    
    }

    return NULL;
    

}