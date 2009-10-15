/****************************************Copyright (c)**************************************************
**                         用于从LOGEX.DLL中导入函数
**                                      
**                                      
**
**                           
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名:  
**创   建   人:  
**最后修改日期: 
**描        述:   
**
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人:  
** 版  本:  
** 日　期:  
** 描　述: 
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 描　述:  
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/

#pragma once

#ifndef LOG_EXT
#define LOG_DATA_EXT extern "C"   __declspec(dllimport)
#else 
#define LOG_DATA_EXT extern "C"  __declspec(dllexport)
#endif



LOG_DATA_EXT  bool RegisterLogEx(TCHAR *szModuleName  );
LOG_DATA_EXT  void UnregisterLogEx( TCHAR *szModuleName  ); 
LOG_DATA_EXT void LogStringEx( TCHAR * szModuleName, TCHAR *  lpszTxt ,int nStrType);
LOG_DATA_EXT void LogStringTmEx( TCHAR * szModuleName,SYSTEMTIME st,  TCHAR *  lpszTxt ,int nStrType);