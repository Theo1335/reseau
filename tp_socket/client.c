
#include "headerExample.h"

int main(void) {
    struct sockaddr_un addr;
    int sfd;
    
    char buffer[BUF_SIZE] = "create test.txt";   
    //char buffer[BUF_SIZE] = "read test.txt";   // fonctionne
    //char buffer[BUF_SIZE] = "append test.txt tttteeesttttt"; // fonctionne
    

    char buf[BUF_SIZE];

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;

    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path)-1);

    if (connect(sfd, (struct sockaddr *) &addr, sizeof (struct sockaddr_un)) == -1){
        perror("connect");
        exit(EXIT_FAILURE);
    }

    printf("-o-> Connecté au serveur !\n");

    ssize_t bytes_sent = write(sfd, buffer, strlen(buffer));
    if (bytes_sent == -1) {
        perror("write");
        close(sfd);
        exit(EXIT_FAILURE);
    }

    printf("---> Message envoye : %s\n", buffer);

    
    ssize_t bytes_received = read(sfd, buf, BUF_SIZE - 1);
    if (bytes_received == -1) {
        perror("read");
        close(sfd);
        exit(EXIT_FAILURE);
    }

    buf[bytes_received] = '\0'; 
    printf("---> Réponse du serveur : %s\n", buf);




    printf("---\n");

    strcpy(buffer, "read test.txt");

    bytes_sent = write(sfd, buffer, strlen(buffer));
    if (bytes_sent == -1) {
        perror("write");
        close(sfd);
        exit(EXIT_FAILURE);
    }

    printf("---> Message envoye : %s\n", buffer);

    
    bytes_received = read(sfd, buf, BUF_SIZE - 1);
    if (bytes_received == -1) {
        perror("read");
        close(sfd);
        exit(EXIT_FAILURE);
    }

    buf[bytes_received] = '\0'; 
    printf("---> Réponse du serveur : %s\n", buf);

    // erreur je n'ai pas vraiment réussi à gérer correctement mes clients pour executer la creation la lecture et l'ecriture d'un coup apres avec netcat normalement ca fonctionne. 


    close(sfd);
    return 0;
}
