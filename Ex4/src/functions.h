void tokenize(char **path, char **RequestType, char *buf);
void execute(char * path,char * type,char* action );
unsigned int countFileLength(FILE *fp);
const char *get_filename_ext(const char *filename);
void findMIME(char *filename, char **MIME);