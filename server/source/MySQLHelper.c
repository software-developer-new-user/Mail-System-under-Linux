#include "../include/MySQLHelper.h"
#include <stdio.h>
#include <string.h>

/***************************************************************************************
* Function Name :  OpenMysql
*
* Description   :  打开数据库
	使用方法		:  本地连接:OpenMysql("localhost","neusoft","neusoft","FlowerMail");
				      远程连接:OpenMysql("10.19.46.88", "guest",  "123", "FlowerMail");
*
* Date          :  2012-09-07
*
* Parameter     :  pcHost为连接主机,pcUser为用户名,pcPwd为密码,pcDb为数据库名
*
* Return Code   :  0打开成功，1打开失败
*
* Author        :  张文跃
* Revise		:  张龙德
****************************************************************************************/
int OpenMysql(char * pcHost,char *pcUser,char * pcPwd,char * pcDb)
{
	if(pcHost==NULL||pcUser==NULL||pcPwd==NULL||pcDb==NULL)
	{
		return 1;
	}
	conn_ptr=mysql_init(&conn);
	if(conn_ptr==NULL)//初始化连接字符串失败
	{
		return 1;
	}  
	//第五个参数为端口号 ;                       0表示使用默认端口号最后一个参数为标示符
    //第六个参数为socket类型 ;                   NULL表示使用默认socket,windows下默认为TCP/IP               
    //最后一个参数表示标示符flag类型;              0表示使用默认标示符                                           
	if (mysql_real_connect(&conn, pcHost, pcUser,  pcPwd, pcDb, 0, NULL, 0))
	{
		return 0;
	}
	else 
	{
		return 1;
	}
}

/***************************************************************************************
* Function Name :  ExecSql
*
* Description   :  增加,更新,删除通用函数,0为执行成功，1为执行失败
*
* Date          :  2012-09-07
*
* Parameter     :  sqltext为增加,更新,删除语句
*
* Return Code   :  -1表示查询失败/0表示查询结果集为空集/执行增加,更新,删除语句后的受影响记录数
*
* Author        :  张文跃
* Revise		:  张龙德
****************************************************************************************/
int ExecSql(char *sqltext)
{
	if(sqltext==NULL)
	{
		return -1;
	}
	int i = -1;
	mysql_query(&conn, sqltext); 
	i = mysql_affected_rows(&conn);
	return i;
}

/***************************************************************************************
* Function Name :  Select
*
* Description   :  选择并存储查询结果 ,可以用函数GetNextRowData逐条读取以table形式存储的数据
*
* Date          :  2012-09-07
*
* Parameter     :  sqltext为select查询语句
*
* Return Code   :   -1表示查询失败/0表示查询结果集为空集/正整数表示查询结果的记录数
*
* Author        :   张文跃
* Revise		:   张龙德
****************************************************************************************/
int Select(char *sqltext)
{
	int i=mysql_query(&conn, sqltext);
    if(i==0)
	{
		res_ptr = mysql_store_result(&conn);
		return mysql_num_rows(res_ptr);
	}
	else if(i==1)
	{
		return -1;
	}
	else
	{
		printf("Error\n");
		return -1;
	}
}

/***************************************************************************************
* Function Name :  GetNextRowData
*
* Description   :  获取每条数据数据;起初指针指向第一条记录，没执行一次该函数，则下移一条记录，直到MYSQL_RES数据指向的table的尾端;
				      该函数依赖查询结果,
*
* Date          :  2012-09-07
*
* Parameter     :  无
*
* Return Code   :  MYSQL类型的单条记录;NULL表示，未得到查询结果集而直接使用该函数
*
* Author        :  张文跃
* Revise		:  张龙德
****************************************************************************************/
MYSQL_ROW GetNextRowData()
{
	if (res_ptr == NULL)
	{
		return NULL;
	}
	return mysql_fetch_row(res_ptr);
}

/***************************************************************************************
* Function Name :  GetRows
*
* Description   :  返回查询到的记录数;注意：该函数依赖查询结果
*
* Date          :  2012-09-07
*
* Parameter     :  无
*
* Return Code   :  记录数,-1表示，未得到查询结果集而直接使用该函数
*
* Author        :  张文跃
* Revise		:  张龙德
****************************************************************************************/
int GetRows()
{
	if (res_ptr == NULL)
	{
		return -1;
	}
	return mysql_num_rows(res_ptr);
}

/***************************************************************************************
* Function Name :  GetFields
*
* Description   :  返回查询到的字段数;注意：该函数依赖查询结果
*
* Date          :  2012-09-07
*
* Parameter     :  无
*
* Return Code   :  记录数,-1表示，未得到查询结果集而直接使用该函数
*
* Author        :  张文跃
* Revise		:  张龙德
****************************************************************************************/
int GetFields()
{
	if (res_ptr == NULL)
	{
		return -1;
	}
	return mysql_num_fields(res_ptr);
}

/***************************************************************************************
* Function Name :  GetInsertId
*
* Description   :  获取内存中刚刚插入数据库的带有auto increment属性的id
*
* Date          :  2012-09-07
*
* Parameter     :  无
*
* Return Code   :   -1表示失败/>0表示id值
*
* Author        :   张文跃
* Revise		:   张龙德
****************************************************************************************/
int GetInsertId()
{
 	return mysql_insert_id(&conn);
}

/***************************************************************************************
* Function Name :  FreeResult
*
* Description   :  释放查询结果所占内存,如果没有执行查询语句，释放空指针
				      该函数依赖查询结果
*
* Date          :  2012-09-07
*
* Parameter     :  无
*
* Return Code   :  无 
*
* Author        :  张文跃
* Revise		:  张龙德
****************************************************************************************/
void FreeResult()
{
	mysql_free_result(res_ptr);
}

/***************************************************************************************
* Function Name :  CloseMySql
*
* Description   :  关闭数据库
				      该函数依赖函数OpenMysql
*
* Date          :  2012-09-07
*
* Parameter     :  无
*
* Return Code   :  无 
*
* Author        :  张文跃
* Revise		:  张龙德
****************************************************************************************/
void CloseMySql()
{
	mysql_close(&conn);
}
