#ifndef SERVERLIB_H
#define SERVERLIB_H
#define BUFFER_SIZE 1024
#define FILE_BUFFER_SIZE 1024*10
#define HELLO_WORLD_SERVER_PORT    12341
#define LENGTH_OF_LISTEN_QUEUE  20
#define FILE_NAME_MAX_SIZE 512
#define SQL_MAX_SIZE  300
	int Insert (char *pcInsert);
	int Delete (char *pcDelete);
	int Update (char *pcUpdate);
	int MySelect (char *pcSelect,char * buffer);
    
    void Usage(int argc,char* argv[]);
	void writeto(int new_fd,char * buffer);
	void writeError(int new_fd,char * buffer);

	int send_file(char* file_name,int sockfd);
	int recv_file (char *file_name,int sockfd);

	int MakeDirectory(char * pcFileName);
	int MakeEmailIdDirectory(char * pcUserId, char * pcType,char *pacEmailId);
    
    int recv_attachedfile (char *acTempData,int sockfd);
    int send_attachedfile (char * pcTempData, int sockfd);
#endif

