#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "commands.h"
#include <stdbool.h>
#include <unistd.h>

bool loop(int sock) {
    struct stat obj;
    size_t size;
    char buf[128], command[8], filename[128];

    //boucle de command
    while(true) {
        printf("%s\n", "Waiting for command.");
        recv(sock, buf, 128, 0);
        sscanf(buf, "%s", command);
        printf("Command \"%s\" received.\n", command);

        if(!strcmp(command, "rls")) {
            rls(sock, obj);
        } else if(!strcmp(command,"downl")) {
            downl(sock, buf, filename, obj);
        } else if(!strcmp(command, "upld")) {
            upld(sock, buf, command, filename, size);
        } else if(!strcmp(command, "rpwd")) {
            rpwd(sock, buf);
        } else if(!strcmp(command, "rcd")) {
            rcd(sock, buf);
        } else if(!strcmp(command, "quit")) {
            printf("%s\n","FTP client closing...");
            quit(sock);
            return true;
        } else if(!strcmp(command, "shutdown")) {
            printf("%s\n","FTP server quitting...");
            quit(sock);
            return false;
        }
    }
}

//verify les credentials envoyés par le client 
int verify_account(char* account, char* password) {
    //ouvre le fichier contenant les compte 
    FILE *fp=fopen("accounts","r");
    short authenticated = 0;
    char tmp[512]={0x0};
    char p[256];
    strcpy(p, ";");
    strcat(p, password);

    //on cherche le compte correspondant au credentials passé en param
    while(fp != NULL && fgets(tmp, sizeof(tmp), fp) != NULL)
    {
        if(strstr(tmp, account) && strstr(tmp, p))
            authenticated = 1;
    }

    if(fp != NULL)
        fclose(fp);

    return authenticated;
}

int main(int argc,char *argv[])
{
    struct sockaddr_in server, client;

    char buf[128], account[128], password[128];
    size_t size = 0;
    int sock1, sock2;
    int k, len;

    // creation socket
    sock1 = socket(AF_INET, SOCK_STREAM, 0);

    if(sock1 == -1)
    {
        printf("Socket creation failed");
        exit(1);
    }

    server.sin_port = htons(1977);
    server.sin_addr.s_addr = 0;

    //binding du socket 
    k = bind(sock1,(struct sockaddr*)&server,sizeof(server));
    if(k == -1)
    {
        printf("Binding error");
        exit(1);
    }
    //on passe le socket en ecoute pour recevoir la connection d'un client 
    k = listen(sock1,1);
    if(k == -1)
    {
        printf("Listen failed");
        exit(1);
    }

    bool continu = true;
    while(continu) {
        printf("%s\n","Waiting for client to connect...");
        len = sizeof(client);
        //recoit la connection d'un client
        sock2 = accept(sock1, (struct sockaddr*)&client, (socklen_t *) &len);
        printf("%s\n","Client connected...");

        //etape d'identification du client par login/mdp
        recv(sock2, &buf, 128, 0);
        printf("%s\n", buf);
        if(strcmp(buf, "BONJ") == 0) {
            send(sock2, "WHO", 128, 0);
            recv(sock2, &account, 128, 0);

            send(sock2, "PASSWD", 128, 0);
            //annule la connection apres trois essais raté
            for (int i = 0; i < 3; ++i) {
                recv(sock2, &password, 128, 0);
                if(verify_account(account, password)) {
                    send(sock2, "WELC", 128, 0);
                    continu = loop(sock2);
                    break;
                } else {
                    //quand le mdp de passe rentré est incorrect
                    send(sock2, "WRONG", 128, 0);
                }
            }

            send(sock2, "BYE", 128, 0);
        }
        close(sock2);
    }
    close(sock1);
}
