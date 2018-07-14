#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "commands.h"

int loop(int sock) {
    struct stat obj;
    size_t size;
    char buf[128], command[8], filename[128];

    accept:
    printf("%s\n", "Waiting for command.");
    recv(sock, buf, 128, 0);
    sscanf(buf, "%s", command);
    printf("%s\n", "Command received.");
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
        printf("%s\n","FTP server quitting..");
        quit(sock);
        return 0;
    }

    goto accept;
}

int verify_account(char* account, char* password) {
    FILE *fp=fopen("accounts","r");
    short authenticated = 0;
    char tmp[512]={0x0};
    char p[256];
    strcpy(p, ";");
    strcat(p, password);

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

    sock1 = socket(AF_INET, SOCK_STREAM, 0);

    if(sock1 == -1)
    {
        printf("Socket creation failed");
        exit(1);
    }

    server.sin_port = htons(1977);
    server.sin_addr.s_addr = 0;

    k = bind(sock1,(struct sockaddr*)&server,sizeof(server));
    if(k == -1)
    {
        printf("Binding error");
        exit(1);
    }
    k = listen(sock1,1);
    if(k == -1)
    {
        printf("Listen failed");
        exit(1);
    }

    printf("%s\n","Waiting for client to connect...");
    len = sizeof(client);
    sock2 = accept(sock1, (struct sockaddr*)&client, (socklen_t *) &len);
    printf("%s\n","Client connected...");
}