#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>

#include<fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

void downl(int sock2, char buf[], char filename[], struct stat obj) {
    sscanf(buf, "%s%s", filename, filename);
    stat(filename, &obj);
    int fd = open(filename, O_RDONLY);
    size_t size = (size_t) obj.st_size;

    if(fd == -1) {
        printf("%s\n","No such file.");
        size = 0;
    }

    send(sock2, &size, sizeof(int), 0);

    char file[size];
    int i = 0;
    FILE*f = fopen(filename,"r");
    while(!feof(f))
        file[i++] = (char) fgetc(f);
    file[i-1] = '\0';
    fclose(f);

    send(sock2, file, size, 0);
}

void upld(int sock2, char buf[], char command[], char filename[], size_t size) {
    int c = 0, fd;
    sscanf(buf+strlen(command) + 1, "%s", filename);
    recv(sock2, &size, sizeof(int), 0);

    char *f = malloc(size);
    recv(sock2, f, size, 0);

    fd = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0666);
    c = (int) write(fd, f, size);
    close(fd);

    send(sock2, &c, sizeof(int), 0);
}

void rpwd(int sock2, char buf[]) {
    system("pwd>temp.txt");
    int i = 0;
    FILE*f = fopen("temp.txt","r");
    while(!feof(f))
        buf[i++] = (char) fgetc(f);
    buf[i-1] = '\0';
    fclose(f);
    system("rm temp.txt");
    send(sock2, buf, 100, 0);
}

void rls(int sock2, struct stat obj) {
    system("ls>temps.txt");
    stat("temps.txt",&obj);
    size_t size = (size_t) obj.st_size;

    char buf[size];
    int i = 0;
    FILE*f = fopen("temps.txt","r");
    while(!feof(f))
        buf[i++] = (char) fgetc(f);
    buf[i-1] = '\0';

    fclose(f);
    system("rm temps.txt");

    send(sock2, &size, sizeof(unsigned long),0);

    send(sock2, buf, size, 0);
}

void rcd(int sock2, char buf[]) {
    int c = chdir(buf+4) == 0 ? 1 : 0;
    send(sock2, &c, sizeof(int), 0);
}

void quit(int sock2) {
    int i = 1;
    send(sock2, &i, sizeof(int), 0);
}