#include "../include/MySQLHelper.h"
#include "serverLib.h"
#include <netinet/in.h>			// for sockaddr_in
#include <sys/types.h>			// for socket
#include <sys/socket.h>			// for socket
#include <stdio.h>				// for printf
#include <stdlib.h>				// for exit
#include <string.h>				// for bzero
#include <errno.h>

/***************************************************************************************
* Function Name :  Usage
*
* Description   :  命令行程序使用方法，设置服务器端口号
*
* Date          :  2012－9－10
*
* Parameter     : argc:命令行参数个数;argv:命令行参数列表
*
* Return Code   : 无
* Author        : 张龙德 
****************************************************************************************/
void Usage(int argc,char* argv[])
{
    int portnumber=0;
  	if(argc!=2)
	{
		fprintf(stderr,"Usage:%s portnumber\a\n",argv[0]);
		exit(0);
	}
    portnumber=atoi(argv[1]);
    if(portnumber<0)
	{
		fprintf(stderr,"portnumber must >0\a\n",argv[0]);
		exit(0);
	}
}

/***************************************************************************************
* Function Name :  writeto
*
* Description   :  向客户端管道写入信息
*
* Date          :  2012－9－10
*
* Parameter     : new_fd:管道标识符;buffer:缓冲区
*
* Return Code   : 无
* Author        : 张龙德 
****************************************************************************************/
void writeto (int new_fd, char *buffer)
{
	if (write (new_fd, buffer, strlen (buffer)) == -1)
	{
		fprintf (stderr, "write error%s\n", strerror (errno));
	}
}

/***************************************************************************************
* Function Name :  writeError
*
* Description   :  向客户端管道写入错误提示信息
*
* Date          :  2012－9－10
*
* Parameter     : new_fd:管道标识符;buffer:缓冲区
*
* Return Code   : 无
* Author        : 张龙德 
****************************************************************************************/
void writeError (int new_fd, char *buffer)
{
	printf ("client tried to operate ,but falied because of some errors!\n");
	if (write (new_fd, buffer, strlen (buffer)) == -1)
	{
		fprintf (stderr, "write error:%s\n", strerror (errno));
	}
	close (new_fd);
}

/***************************************************************************************
* Function Name :  Insert
*
* Description   :  向Mysql数据库插入一条记录
*
* Date          :  2012－9－10
*
* Parameter     : pcInsert：sqlinsert|sql语句
*
* Return Code   : -1	主健冲突或者查询语句出错或者不存在目标表或者内容不存在,未执行
				   0	受影响的记录数为0
			            正整数	"添加成功，受影响的记录数；向EmailTable插入一条记录时，如果成功返回自增的emailId"
* Author        : 张龙德 
****************************************************************************************/
int Insert (char *pcInsert)
{
	int iAdd = -1;				//增加
	//执行增加语句
	iAdd = ExecSql (pcInsert);
	if (iAdd > 0)
	{
		printf ("添加成功\n");
        if(strstr(pcInsert,"EmailTable")!=NULL)
        {
          return GetInsertId();
        }
	}
	else if (iAdd == 0)
	{
		printf ("受影响的记录数为0\n");
	}
	else
	{
		printf ("主健冲突或者查询语句出错或者不存在目标表,未执行\n");
	}
	return iAdd;
}

/***************************************************************************************
* Function Name :  Delete
*
* Description   : 删除Mysql数据库的一条记录
*
* Date          :  2012－9－10
*
* Parameter     : pcDelete：sqldelete|sql语句
*
* Return Code   : -1	查询语句出错或者不存在目标表或者内容不存在,未执行
				   0	受影响的记录数为0
				   正整数	删除成功，受影响的记录数
* Author        : 张龙德 
****************************************************************************************/
int Delete (char *pcDelete)
{
	int iDelete = -1;			//删除
	//执行删除语句
	iDelete = ExecSql (pcDelete);
	if (iDelete > 0)
	{
		printf ("删除成功\n");
	}
	else if (iDelete == 0)
	{
		printf ("受影响的记录数为0\n");
	}
	else
	{
		printf ("查询语句出错或者不存在目标表,未执行\n");
	}
	return iDelete;
}
/***************************************************************************************
* Function Name :  Update
*
* Description   : 删除Mysql数据库的一条记录
*
* Date          :  2012－9－10
*
* Parameter     : pcUpdate：sqlupdate|sql语句
*
* Return Code   : -1	查询语句出错或者不存在目标表或者内容不存在,未执行
				   0	受影响的记录数为0
				     正整数	删除成功，受影响的记录数
* Author        : 张龙德 
****************************************************************************************/
int Update (char *pcUpdate)
{
	int iUpdate = -1;			//更新
	//更新更改语句
	iUpdate = ExecSql (pcUpdate);
	if (iUpdate > 0)
	{
		printf ("更新成功\n");
	}
	else if (iUpdate == 0)
	{
		printf ("受影响的记录数为0\n");
	}
	else
	{
		printf ("查询语句出错或者不存在目标表,未执行\n");
	}
	return iUpdate;
}

/***************************************************************************************
* Function Name :  Update
*
* Description   : 删除Mysql数据库的一条记录
*
* Date          :  2012－9－10
*
* Parameter     : pcUpdate：sqlselect|sql语句
*
* Return Code   : -1	查询语句存在错误或者不存在目标表,未执行查询
				   0	查询到的结果为空集
				  字符串	"查询到的结果字符数组，每条记录之间用‘~’分隔，每个字段之间用'|'分隔；注意最后一条记录后面跟着‘~’"
* Author        : 张龙德 
****************************************************************************************/
int MySelect (char *pcSelect, char *buffer)
{
	int iSelect = -1;			//查询
	int iFields = 0;
	int i = 0;
	MYSQL_ROW r = NULL;			//存放查找到的一行数据
	iSelect = Select (pcSelect);	//查询语句正确且结果集不为空
	printf ("查询返回值%d\n", iSelect);
	if (iSelect > 0)
	{
		printf ("所查到的数据是:\n ");
		iFields = GetFields ();
		while ((r = GetNextRowData ()))
		{
			char acTemp[1024] = {0};
			for (i = 0; i < iFields; i++)
			{
				strcat (buffer, r[i]);
				strcat (buffer, "|");
				printf ("%s\t", r[i]);
			}
			strcat (buffer, "~");
		}
		strcat (buffer, "\0");
		//释放查询数据
		FreeResult ();
	}
	else if (iSelect == 0)
	{
		printf ("查询到的结果为空集\n");
	}
	else
	{
		printf ("查询语句存在错误或者不存在目标表,未执行查询\n");
	}
	return iSelect;
}

int MakeEmailIdDirectory(char * pcUserId, char * pcType,char *pacEmailId)
{
    char  acEmailIdDirectory[100]={0}; 
    char  acUserDirectory[100]={0};
    sprintf(acEmailIdDirectory,"../data/%s/%s/%s",pcUserId,pcType,pacEmailId); 
    sprintf(acUserDirectory,"../data/%s",pcUserId);
	if( (access( acUserDirectory, 0 )) == -1 )//user directory does exit.
    {     
         printf( "File %s does not exist.\n",acEmailIdDirectory );
		 if(MakeDirectory(acUserDirectory)==1)//create user directory failed.
         {
             return 1;
         }
         char  acTypeDirectory[100]={0};
         sprintf(acTypeDirectory,"../data/%s/1",pcUserId); 
         if(MakeDirectory(acTypeDirectory)==1)//create acTypeDirectory failed.
         {
             //printf( "Problem creating directory :%s\n",pcFileName );
             return 1;
         } 
         memset(acTypeDirectory,'\0',strlen(acTypeDirectory));  
         sprintf(acTypeDirectory,"../data/%s/2",pcUserId);  
         if(MakeDirectory(acTypeDirectory)==1)//create acTypeDirectory failed.
         {
             return 1;
         }
          memset(acTypeDirectory,'\0',strlen(acTypeDirectory));
          sprintf(acTypeDirectory,"../data/%s/3",pcUserId);  
         if(MakeDirectory(acTypeDirectory)==1)//create acTypeDirectory failed.
         {
             return 1;
         }
         memset(acTypeDirectory,'\0',strlen(acTypeDirectory));
         sprintf(acTypeDirectory,"../data/%s/4",pcUserId);  
         if(MakeDirectory(acTypeDirectory)==1)//create acTypeDirectory failed.
         {
             return 1;
         }
          if(MakeDirectory(acEmailIdDirectory)==1)//
         {
             return 1;
         } 
    }
    else
    {	
        printf( "File %s exists.\n ",acEmailIdDirectory);
        if(MakeDirectory(acEmailIdDirectory)==1)//
         {
             return 1;
         } 
    } 
    return 0;
}

/***************************************************************************************
* Function Name :  MakeDirectory
*
* Description   :  建立单级文件夹
*
* Date          :  2012－9－10
*
* Parameter     : pcUpdate：sqlselect|sql语句
*
* Return Code   :  1	创建文件夹失败
				   0	创建文件夹成功
* Author        : 张龙德 
****************************************************************************************/
int MakeDirectory(char * pcFileName)
{
	 if( mkdir( pcFileName,0777 ) == 0 )
	{
		printf( "Directory %s was successfully created\n",pcFileName );
        return 0;
	}
	else
    {
		printf( "Problem creating directory :%s\n",pcFileName );
        return 1;
     }
}

int recv_file (char *acTempData,int sockfd)
{
     char * pcTemp=strchr(acTempData,'|');printf("pcTemp:%s\n",pcTemp);
     char * pcBuffer=strrchr(pcTemp,'|');printf("pcBuffer length:%d\n",strlen(pcBuffer));
     if(pcTemp==NULL)
     {
		return 1;
     }
     char  buffer[1024]={0};
     memset(buffer,'\0',strlen(buffer));
     strncpy(buffer,pcBuffer+1,strlen(pcBuffer));//strcat(buffer,'\0');
     char  acEmailId[20]={0};
     char  sql[SQL_MAX_SIZE]={0};
     char  acEmailInfo[100]={0};
     char  acFilename[100]={0};
     char  acDirectory[100]={0};
     strncpy(acEmailId,acTempData,pcTemp-acTempData);
     sprintf(sql,"select title,type,userID from EmailTable where emailId=%s",acEmailId);
     MySelect(sql,acEmailInfo);
     printf("\nemail information according to emailId:%s\n",acEmailInfo);
     char acContent[100]={0};char acT1[100]={0};
     char acType[10]={0};
     char acUserId[26]={0};
     char * pcType=strchr(acEmailInfo,'|');pcType++;strcpy(acT1,pcType);
     char * pcUserId=strchr(acT1,'|');pcUserId++;
     char * pcTail=strrchr(acT1,'|');
     
     strncpy(acContent,acEmailInfo,pcType-acEmailInfo-1);printf("acContent:%s\n",acContent);
     strncpy(acType,pcType,1);printf("acType:%s\n",acType);
	 strncpy(acUserId,pcUserId,pcTail-pcUserId);
     sprintf(acDirectory,"../data/%s/%s/%s",acUserId,acType,acEmailId);
     sprintf(acFilename,"../data/%s/%s/%s/%s",acUserId,acType,acEmailId,acContent);
     printf("acDirectory:%s\nacFileName:%s\n",acDirectory,acFilename);
     
   // printf("%d\n",access( acDirectory, 0 ));
    if(MakeEmailIdDirectory(acUserId, acType,acEmailId)==1)
    {
        printf( "Problem creating EmailIdDirectory %s\n",acDirectory);
		return 1;
    }      

	int length = 0;
	//char buffer[FILE_BUFFER_SIZE];
	FILE *fp = fopen (acFilename, "wb");
	if (NULL == fp)
	{
		printf ("File:\t%s Can Not Open To Write\n", acFilename);
		//exit (1);
        return 1;
	}
	//        int write_length = write(fp, buffer,length);
	int write_length = fwrite (buffer, sizeof (char), FILE_BUFFER_SIZE, fp);
	if (write_length < length)
	{
		printf ("File:\t%s Write Failed\n", acFilename);
		//break;
	}	

    memset(buffer,'\0',strlen(buffer));
    sprintf(sql,"update EmailTable set content='%s' where emailId=%s",acFilename,acEmailId);
    Update(sql);
	bzero (buffer, BUFFER_SIZE);
    return 0;
}

int send_file (char * pcTempData, int sockfd)
{   
    char  sql[SQL_MAX_SIZE]={0};
    char  file_name[100]={0};
    char  buf[100]={0};
	char* head=pcTempData;
	char* last=pcTempData;
	printf("pcTempData:%s\n",pcTempData);
    memset(file_name,'\0',strlen(file_name));
    sprintf(sql,"select content from EmailTable where emailId=%s",pcTempData);
    MySelect(sql,buf);printf("buf:%s\n",buf);
    head=buf;
	last=buf;
    last=strchr(head,'|');
	strncpy(file_name,head,last-head);
    printf("file_name:%s\n",file_name);
	char buffer[FILE_BUFFER_SIZE];
	FILE *fp = fopen (file_name, "r");
	if (NULL == fp)
	{
		printf ("File:\t%s Not Found\n", file_name);
        return 1;
	}
	else
	{
		bzero (buffer, BUFFER_SIZE);
		int file_block_length = 0;
		//            while( (file_block_length = read(fp,buffer,BUFFER_SIZE))>0)
		if ((file_block_length = fread (buffer, sizeof (char), BUFFER_SIZE, fp)) > 0)
		{
			printf ("file_block_length = %d\n", file_block_length);
			//发送buffer中的字符串到new_server_socket,实际是给客户端
			/*if (send (sockfd, buffer, file_block_length, 0) < 0)
			{
				printf ("Send File:\t%s Failed\n", file_name);
				break;
			}*/
			
			//bzero (buffer, BUFFER_SIZE);
	        printf("string sended to client:%s\n",buffer);
        	writeto(sockfd,buffer);
            close (sockfd);
        }
		//            close(fp);
	}
	fclose (fp);
	printf ("File:\t%s Transfer Finished\n", file_name);
    return 0;
}

int send_attachedfile (char * pcTempData, int sockfd)
{
   char  sql[SQL_MAX_SIZE]={0};
    char  file_name[100]={0};
    char  buf[100]={0};
	char* head=pcTempData;
	char* last=pcTempData;
	printf("pcTempData:%s\n",pcTempData);
    memset(file_name,'\0',strlen(file_name));
    sprintf(sql,"select attachedFilePath from EmailTable where emailId=%s",pcTempData);
    MySelect(sql,buf);printf("buf:%s\n",buf);
    head=buf;
	last=buf;
    last=strchr(head,'|');
	strncpy(file_name,head,last-head);
    printf("file_name:%s\n",file_name);
	char buffer[FILE_BUFFER_SIZE];
	FILE *fp = fopen (file_name, "r");
	if (NULL == fp)
	{
		printf ("File:\t%s Not Found\n", file_name);
        return 1;
	}
	else
	{
		bzero (buffer, BUFFER_SIZE);
		int file_block_length = 0;
		//            while( (file_block_length = read(fp,buffer,BUFFER_SIZE))>0)
		if ((file_block_length = fread (buffer, sizeof (char), BUFFER_SIZE, fp)) > 0)
		{
			printf ("file_block_length = %d\n", file_block_length);
	        printf("string sended to client:%s\n",buffer);
        	writeto(sockfd,buffer);
            close (sockfd);
        }
	}
	fclose (fp);
	printf ("File:\t%s Transfer Finished\n", file_name);
    return 0; 
}


int recv_attachedfile (char *acTempData,int sockfd)
{
	 char * pcTemp=strchr(acTempData,'|');printf("pcTemp:%s\n",pcTemp);
     char * pcBuffer=strrchr(pcTemp,'|');printf("pcBuffer length:%d\n",strlen(pcBuffer));
     if(pcTemp==NULL)
     {
		return 1;
     }
     char  buffer[1024]={0};
     memset(buffer,'\0',strlen(buffer));
     strncpy(buffer,pcBuffer+1,strlen(pcBuffer));//strcat(buffer,'\0');
     char  acEmailId[20]={0};
     char  sql[SQL_MAX_SIZE]={0};
     char  acEmailInfo[100]={0};
     char  acFilename[100]={0};
     char  acDirectory[100]={0};
     strncpy(acEmailId,acTempData,pcTemp-acTempData);
     sprintf(sql,"select attachedFilePath,type,userID from EmailTable where emailId=%s",acEmailId);
     MySelect(sql,acEmailInfo);
     printf("\nemail information according to emailId:%s\n",acEmailInfo);
     char acContent[100]={0};char acT1[100]={0};
     char acType[10]={0};
     char acUserId[26]={0};
     char * pcType=strchr(acEmailInfo,'|');pcType++;strcpy(acT1,pcType);printf("pcType:%s\n",pcType);
     char * pcUserId=strchr(acT1,'|');pcUserId++;printf("pcUserId:%s\n",pcUserId);
     char * pcTail=strrchr(acT1,'|');printf("pcTail:%s\n",pcTail);
     
     strncpy(acContent,acEmailInfo,pcType-acEmailInfo-1);printf("acContent:%s\n",acContent);
     strncpy(acType,pcType,1);printf("acType:%s\n",acType);
	 strncpy(acUserId,pcUserId,pcTail-pcUserId);
     sprintf(acDirectory,"../data/%s/%s/%s",acUserId,acType,acEmailId);
     sprintf(acFilename,"../data/%s/%s/%s/%s",acUserId,acType,acEmailId,acContent);
     printf("acDirectory:%s\nacFileName:%s\n",acDirectory,acFilename);
     
   // printf("%d\n",access( acDirectory, 0 ));
    if(MakeEmailIdDirectory(acUserId, acType,acEmailId)==1)
    {
        printf( "Problem creating EmailIdDirectory %s\n",acDirectory);
		return 1;
    }      

	int length = 0;
	//char buffer[FILE_BUFFER_SIZE];
	FILE *fp = fopen (acFilename, "wb");
	if (NULL == fp)
	{
		printf ("File:\t%s Can Not Open To Write\n", acFilename);
        return 1;
	}
	//        int write_length = write(fp, buffer,length);
	int write_length = fwrite (buffer, sizeof (char), FILE_BUFFER_SIZE, fp);
	if (write_length < length)
	{
		printf ("File:\t%s Write Failed\n", acFilename);
		//break;
	}	

    memset(buffer,'\0',strlen(buffer));
    sprintf(sql,"update EmailTable set attachedFilePath='%s' where emailId=%s",acFilename,acEmailId);
    Update(sql);
	bzero (buffer, BUFFER_SIZE);
    return 0;
}

int DeleteMail(char * pcEmailId,int sockfd)
{   
	char  sql[SQL_MAX_SIZE]={0};
    char  emailDirectory[FILE_NAME_MAX_SIZE]={0};
    char  buf[FILE_NAME_MAX_SIZE]={0};
    char  acUserId[100]={0};
    char  acType[10]={0};
    sprintf(sql,"select userId from EmailTable where emailId=%s",pcEmailId);
    int i=MySelect(sql,buf);//从数据库中获得带拼接符号的字符串
    if(i<1)
    {
		return 1;
    }
    char * last=strchr(buf,'|');//去掉尾缀，解析出用户ID
    strncpy(acUserId,buf,last-buf);
    
    memset(buf,0,strlen(buf));
    sprintf(sql,"select type from EmailTable where emailId=%s",pcEmailId);
    i=MySelect(sql,buf);//从数据库中获得带拼接符号的字符串
    if(i<1)
    {
		return 1;
    }
    last=strchr(buf,'|');//去掉尾缀，解析出邮件类型
    strncpy(acType,buf,last-buf);
    sprintf(emailDirectory,"rm -r ../data/%s/%s/%s",acUserId,acType,pcEmailId);  //拼接获得该封邮件所对应的文件夹
    //printf("remove %s\n",buf);
     system(emailDirectory); 
     return 0;
   /* if( (access( emailDirectory, 0 )) == -1 )//如果不存在该封邮件所对应的文件夹
    {
         printf("%s does not exits\n",emailDirectory);
         return -1;
    }
    
    if(remove(buf)==0)
    {
		printf("rmdir %s successfully!\n",emailDirectory);
        return 0;
    }
    else
    {
		printf("rmdir %s failed\n",emailDirectory);
        return 1;
    }
	*/
}

