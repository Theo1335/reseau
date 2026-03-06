
#include "headerExample.h"


int main(void){
    struct sockaddr_un addr;
    int sfd, cfd;
    ssize_t numRead;
    char buf[BUF_SIZE], cmd[7], filename[20],contenue_du_filename[BUF_SIZE], retour[BUF_SIZE]; 

    sfd = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sfd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;

    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    unlink(SV_SOCK_PATH);

    if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(sfd, 3) == -1){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("⌛ attente de connexions...\n");


    while(1)
    {   
        cfd = accept(sfd, NULL, NULL);

        if (cfd == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        int pid = fork();

        if (pid == -1) {
            perror("fork");
            close(cfd);
        } else if (pid == 0) {  
            close(sfd);
            printf("-o-> connecter\n");
            fflush(stdout);
            numRead = read(cfd, buf, BUF_SIZE - 1);
            if (numRead == -1) {
                perror("lecture numread");
                close(cfd);
                exit(EXIT_FAILURE);
            }
            
        buf[numRead] = '\0';
        printf("---> Requête reçue : %s\n", buf);


   
        int nbArgs = sscanf(buf, "%s %s %s", cmd, filename, contenue_du_filename);
  
        if (strcmp(cmd, "create") == 0 && nbArgs == 2) {
            
            int fd = open(filename, O_CREAT | O_WRONLY, 0644);
            if (fd == -1) {
                perror("open");
                strcpy(retour, "Erreur : impossible de creer le fichier.");
            } else {
                strcpy(retour, "fichier cree avec succes.");
                close(fd);
            }
        } else if (strcmp(cmd, "read") == 0 && nbArgs == 2) {
            int fd = open(filename, O_RDONLY);
            if (fd == -1) {
                perror("open");
                strcpy(retour, "Erreur : fichier introuvable.");
            } else {
                numRead = read(fd, retour, BUF_SIZE - 1);
                if (numRead == -1) {
                    perror("read");
                    strcpy(retour, "Erreur de lecture.");
                } else {
                    retour[numRead] = '\0';
                }
                close(fd);
            }

        } else if (strcmp(cmd, "append") == 0 && nbArgs == 3) {
            int fd = open(filename, O_WRONLY | O_APPEND);
            if (fd == -1) {
                perror("open");
                strcpy(retour, "Erreur : impossible d'ouvrir le fichier.");
            } else {
                if (write(fd, contenue_du_filename, strlen(contenue_du_filename)) == -1) {
                    perror("write");
                    strcpy(retour, "Erreur : ecriture echouee.");
                } else {
                    strcpy(retour, "Donnees ajoutees avec succes.");
                }
                close(fd);
            }

        } else {
            strcpy(retour, "cmde invalide.");
        }

        write(cfd, retour, strlen(retour));
        write(cfd, "\n", strlen(retour));
        close(cfd);
        exit(EXIT_SUCCESS);

        } else { 
            wait(NULL);  
            close(cfd);
        }     
    }
    
    close(sfd);
    unlink(SV_SOCK_PATH);

    return 1;
}