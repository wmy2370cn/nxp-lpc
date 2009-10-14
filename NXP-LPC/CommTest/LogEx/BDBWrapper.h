

#include "db_cxx.h"
class CBDBWrapper
{
public:
	CBDBWrapper( );
	~CBDBWrapper( );
public:
protected:
	Db	   *m_pDB;					//数据库
	DbEnv  *m_pDbEnv;				//数据库环境
	DbTxn  *m_pDbTxn;				//定义事务处理


};