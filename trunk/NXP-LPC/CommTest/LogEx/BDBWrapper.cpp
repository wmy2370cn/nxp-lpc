


#include "stdafx.h"

#include "BDBWrapper.h"
 
 

CBDBWrapper::CBDBWrapper()
{
	m_pDB = NULL;					//数据库
	m_pDbEnv = NULL;				//数据库环境
	m_pDbTxn = NULL;				//定义事务处理
}

CBDBWrapper::~CBDBWrapper( )
{

}