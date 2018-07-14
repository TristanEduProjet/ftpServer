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