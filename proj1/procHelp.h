/* name: Yoodong Lee, Won Jun Cho*/
/* read directory and file */
void readFile(char*, char*, char *);

/* parse stat file */
char **parseStat(FILE *);

/* parse statm file */
int parseStatm(FILE *,char*);

/* read stat file */
char *readCmd(FILE*, char*);

/* compare the uid of every process with my uid */
int readUid(char*);

/* check if buffer is only number */
int onlyNum(char *buffer);


