#include "../include/MySQLHelper.h"
#include "serverLib.h"
#include <netinet/in.h>		// for sockaddr_in
#include <sys/types.h>		// for socket
#include <sys/socket.h>		// for socket
#include <stdio.h>		// for printf
#include <stdlib.h>		// for exit
#include <string.h>		// for bzero
#include <errno.h>
#include "serverLib.h"


int
main (int argc, char **argv)
{
  Usage(argc,argv);
  //设置一个socket地址结构server_addr,代表服务器internet地址, 端口
  struct sockaddr_in server_addr;
  bzero (&server_addr, sizeof (server_addr));	//把一段内存区的内容全部设置为0
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htons (INADDR_ANY);
  server_addr.sin_port = htons (atoi(argv[1]));

  //创建用于internet的流协议(TCP)socket,用server_socket代表服务器socket
  int server_socket = socket (PF_INET, SOCK_STREAM, 0);
  if (server_socket < 0)
  {
      printf ("Create Socket Failed!");
      exit (1);
  }

  //把socket和socket地址结构联系起来
  if (bind
      (server_socket, (struct sockaddr *) &server_addr, sizeof (server_addr)))
    {
      printf ("Server Bind Port : %d Failed!", atoi(argv[1]));
      exit (1);
    }

  //server_socket用于监听
  if (listen (server_socket, LENGTH_OF_LISTEN_QUEUE))
    {
      printf ("Server Listen Failed!");
      exit (1);
    }
   printf("server is running suceessfully!\n");
  while (1)			//服务器端要一直运行
    {
      //定义客户端的socket地址结构client_addr
      struct sockaddr_in client_addr;
      socklen_t length = sizeof (client_addr);

      int new_server_socket =accept (server_socket, (struct sockaddr *) &client_addr, &length);
      if (new_server_socket < 0)
	  {
	    printf ("Server Accept Failed!\n");
	    continue;
	  }

      char buffer[BUFFER_SIZE];
      bzero (buffer, BUFFER_SIZE);

      //读取第一条字符流
      length = recv (new_server_socket, buffer, BUFFER_SIZE, 0);
      printf("string received from client:%s\nlength:%d\n",buffer,length);
      if (length < 15)
	  {
		  printf ("Server Recieve Data length<15!\n");
          writeError(new_server_socket,"illeagal_string_length");
		  continue;
	  }
      
      char cmdType[15];//存放报头信息
      bzero (cmdType, 15);
	  char * pcTempCmdType=strchr(buffer,'|');
	  char  acTempData[1024]={0};//存储报头外的其他信息
      if(pcTempCmdType==NULL)
	  {
			printf("there is no head in the string from client!\n");
            writeError(new_server_socket,"illeagal_string");
            continue;
	   }
       strncpy (cmdType, buffer,strlen(buffer)-strlen(pcTempCmdType));
       printf("cmdType:%s\n",cmdType);
	   pcTempCmdType++;
       strcpy(acTempData,pcTempCmdType);
       printf("string received from server  excluding header:%s\tlength:%d\n",acTempData,strlen(acTempData));
		//远程连接（连接到张文跃那台电脑上）:OpenMysql("10.19.46.88", "guest",  "123", "FlowerMail");
	if (OpenMysql ("localhost", "neusoft", "neusoft", "FlowerMail") != 0)
	{
		printf ("open database failed\n");
        writeError(new_server_socket,"open_database_failed");
		continue;
	}
     memset(buffer,0,strlen(buffer));
      if(strcmp(cmdType,"sqlselect")==0)
      {
	  //执行查询语句，把查询结果返回给客户端
			int iSelect=MySelect(acTempData,buffer);          
            if(iSelect<1)
			{   
                sprintf(buffer,"%d",iSelect);
                //itoa(iSelect,buffer);
				writeError(new_server_socket,buffer);
				continue;
			}          	
      }
      else if(strcmp(cmdType,"sqlinsert")==0)
      {
         //执行sql返回0，1给客户端
		int iInsert=Insert(acTempData);//
        
        sprintf(buffer,"%d",iInsert);printf("buffer:%s\n",buffer);
       /* printf("string sended to client:%s\n",buffer);
        writeto(new_server_socket,buffer);*/
        //itoa(iInsert,buffer);	
      }
      else if(strcmp(cmdType,"sqldelete")==0)
      {
         //执行sql返回0，1给客户端
         int iDelete=Delete(acTempData);
         sprintf(buffer,"%d",iDelete);
      }
      else if(strcmp(cmdType,"sqlupdate")==0)
      {
         //执行sql返回0，1给客户端
		 int iUpdate=Update(acTempData);
         sprintf(buffer,"%d",iUpdate);
      }
      else if(strcmp(cmdType,"filefrcli")==0)
      {
          memset(buffer,0,strlen(buffer));
         if(recv_file(acTempData,new_server_socket)==0)
         {
			strcpy(buffer,"filefrcli_ok!");
          }else
          {
          	strcpy(buffer,"filefrcli_failed!");
          }
      }
      else if(strcmp(cmdType,"filetocli")==0)
      {
	 	 //从服务器指定路径下，把某个邮件传送给客户端 
         if(send_file(acTempData,new_server_socket)==0)
         {
            printf("filetocli_OK\n");
            continue;
         }
         else
         {
			 strcpy(buffer,"filetocli_failed");
             printf("filetocli_failed\n");
         }
      }else if(strcmp(cmdType,"attatocli")==0)
	  {
		if(send_attachedfile(acTempData,new_server_socket)==0)
         {
            printf("attatocli_OK\n");
            continue;
         }
         else
         {
             strcpy(buffer,"attatocli_failed");
             printf("attatocli_failed\n");
         }
	  }
	  else if(strcmp(cmdType,"attafrcli")==0)
      {
         memset(buffer,0,strlen(buffer));
         if(recv_attachedfile(acTempData,new_server_socket)==0)
         {
			strcpy(buffer,"attafrcli_OK!");
          }else
          {
          	strcpy(buffer,"attafrcli_failed!");
          }
	  }
      else if(strcmp(cmdType,"deletmail")==0)
      {
          if(DeleteMail(acTempData,acTempData)==0)
           {
			strcpy(buffer,"deletmail_OK!");
            printf("deletmail_ok\n");
          }else
          {
          	strcpy(buffer,"deletmail_failed!");
            printf("deletmail_failed\n");
          }
      }
      else
	  {
			printf("illeagal_string from client\n");
            writeError(new_server_socket,"illeagal_string");
            continue;
	   }
       printf("string sended to client:%s\n",buffer);
       writeto(new_server_socket,buffer);
      //关闭与客户端的连接
      close (new_server_socket);
    }
  //关闭监听用的socket
  close (server_socket);
  exit(0);
}


