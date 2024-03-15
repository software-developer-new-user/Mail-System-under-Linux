#ifndef MYSQLHELPER_H
#define MYSQLHELPER_H

#include "mysql.h"
#include <stdio.h>
#include <stdlib.h>

//创建连接对象;
MYSQL conn;

MYSQL* conn_ptr;

//查询数据表存放指针
MYSQL_RES *res_ptr;

//每一行数据对象
MYSQL_ROW sqlrow;

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
int OpenMysql(char * pcHost,char *pcUser,char * pcPwd,char * pcDb);

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
int ExecSql(char *sqltext);

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
int Select(char *sqltext);

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
int GetRows();

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
int GetFields();

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
int GetInsertId();

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
int Select(char *sqltext);

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
MYSQL_ROW GetNextRowData();

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
void FreeResult();

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
void CloseMysql();


#endif
