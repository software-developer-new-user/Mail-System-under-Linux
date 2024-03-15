#include "../include/MySQLHelper.h"
#include <stdio.h>
void Test ();
void Insert (char *pcInsert);
void Delete (char *pcDelete);
void Update (char *pcUpdate);
void MySelect (char *pcSelect);

int main ()
{
	//远程连接（连接到张文跃那台电脑上）:OpenMysql("10.19.46.88", "guest",  "123", "FlowerMail");
	if (OpenMysql ("localhost", "neusoft", "neusoft", "FlowerMail") != 0)
	{
		printf ("failed\n");
	}
	else
	{ 
		 Insert("Insert into UserTable(userId, nickName, passwd, state, telephone)values('1555','xiaohei','123',1,'123')");//
         Insert("Insert into UserTable(userId, nickName, passwd, state, telephone)values('1355','xiaohei','123',1,'123')");//主健冲突
 		Insert("Insert into UserTable(userId, nickName, passwd, state, telephone)values('1355','xiaohei','123',1,'1)");//主健冲突或者查询语句出错
     	Insert("Insert into UserTable2(userId, nickName, passwd, state, telephone)values('1355','xiaohei','123',1,'123')");//不存在目标表
		 Delete("delete from UserTable Where UserId = '1098'");
		 Update("update UserTable set passwd = 'zld' where nickName = 'xiaohei'");
		 MySelect("select * from UserTable");
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	CloseMySql ();
	return 0;
}

void Insert (char *pcInsert)
{
	int iAdd = -1;				//增加
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//执行增加语句
	iAdd = ExecSql (pcInsert);
	if (iAdd > 0)
	{
		printf ("增加返回值%d\n", iAdd);
		printf ("添加成功\n");
	}
	else if (iAdd == 0)
	{
		printf ("增加返回值%d\n", iAdd);
		printf ("受影响的记录数为0\n");
	}
	else
	{
		printf ("增加返回值%d\n", iAdd);
		printf ("主健冲突或者查询语句出错或者不存在目标表,未执行\n");
	}
}

void Delete (char *pcDelete)
{
	int iDelete = -1;			//删除
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//执行删除语句
	iDelete = ExecSql (pcDelete);
	//printf("delete rows:%lu\n",(unsigned long)mysql_affected_rows(&res_ptr));
	if (iDelete > 0)
	{
		printf ("删除返回值%d\n", iDelete);
		printf ("删除成功\n");
	}
	else if (iDelete == 0)
	{
		printf ("删除返回值%d\n", iDelete);
		printf ("受影响的记录数为0\n");
	}
	else
	{
		printf ("增加返回值%d\n", iDelete);
		printf ("查询语句出错或者不存在目标表,未执行\n");
	}
}

void Update (char *pcUpdate)
{
	int iUpdate = -1;			//更新
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//更新更改语句
	iUpdate = ExecSql (pcUpdate);
	if (iUpdate > 0)
	{
		printf ("更新返回值%d\n", iUpdate);
		printf ("更新成功\n");
	}
	else if (iUpdate == 0)
	{
		printf ("更新返回值%d\n", iUpdate);
		printf ("受影响的记录数为0\n");
	}
	else
	{
		printf ("增加返回值%d\n", iUpdate);
		printf ("查询语句出错或者不存在目标表,未执行\n");
	}
}
void MySelect (char *pcSelect)
{
	int iSelect = -1;			//查询
	MYSQL_ROW r = NULL;			//存放查找到的一行数据
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//获取查询数据，一行一行获取
		//selectresult = Select("select * ");//查询语句存在错误查询语句存在错误或者不存在目标表
		//selectresult = Select("select * from EmailTail");//查询到的结果为空集
		iSelect = Select (pcSelect);	//查询语句正确且结果集不为空
		printf ("查询返回值%d\n", iSelect);
		if (iSelect > 1)
		{
			while ((r = GetNextRowData ()))
			{
				printf ("所查到的数据是  %s %s %s %s %s\n", r[0], r[1], r[2], r[3], r[4]);
			}
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
}

void Test ()
{
	MYSQL *conn2;
	int i = -1;
	conn2 = mysql_init (NULL);	//printf("CLIENT_FOUND_ROWS:%d\n",CLIENT_FOUND_ROWS);
	mysql_real_connect (conn2, "localhost", "neusoft", "neusoft", "FlowerMail", 0, NULL, CLIENT_FOUND_ROWS);
	mysql_query (conn2, "delete from UserTable where userId='1058'");	//printf("%ld\n",(long)mysql_affected_rows(conn2));

	//i=mysql_query(conn2,"select *from UserTable");
	//printf("%d\n",i);//1 opearate failed  ,0 operate success maybe result is null;
	//printf("%d\n",mysql_num_rows(conn2));
	//printf("%d\n",mysql_delete_id(conn));
	mysql_query (conn2, "Insert into UserTable values('1498','xx','123',1,'123')");
	printf ("%ld\n", (long) mysql_affected_rows (conn2));

	mysql_query (conn2, "update UserTable set passwd = 'zld4' where nickName = 'xiaohei'");
	//printf("CLIENT_FOUND_ROWS:%d\n",CLIENT_FOUND_ROWS);
	//printf("%d\n",mysql_rows_affected(conn));
	//MYSQL_RES* res = mysql_store_result(&conn);
	printf ("%d\n", mysql_affected_rows (conn2));
	printf ("%d\n", mysql_insert_id (conn2));

	//printf("CLIENT_FOUND_ROWS:%d\n",CLIENT_FOUND_ROWS);
	mysql_close (conn2);
}
