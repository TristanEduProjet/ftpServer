void downl(int sock2, char buf[], char filename[], struct stat obj);

void upld(int sock2, char buf[], char command[], char filename[], size_t size);

void rpwd(int sock2, char buf[]);

void rls(int sock2, struct stat obj);

void rcd(int sock2, char buf[]);

void quit(int sock2);
