/******************************************************************************
模块名      :  
文件名      : 
相关文件    :
实现功能    : 内存中的数据结构
修改记录    :
备注        :  
------------------------------------------------------------------------------
日期        修改人      修改内容
2007/7/26
******************************************************************************/

#ifndef MEM_CNST_H
#define MEM_CNST_H


#define  MAX_I_CNT   512 
#define  MAX_Q_CNT   512
#define  MAX_AI_CNT  128 
#define  MAX_AQ_CNT  128 


#define  MAX_MB_CNT   512
#define  MAX_MW_CNT   512
#define  MAX_MD_CNT   32

#define  MAX_IM_CNT   4096

#define  MAX_S_CNT   128
#define  MAX_SW_CNT  128
#define  MAX_TM_CNT   32 //定时器
#define  MAX_C_CNT	  16 //计数器


//以下是寄存器的实际地址起始
#define I_START_ADDR   0
#define Q_START_ADDR   0 

//#define Q_END_ADDR     511 

#define AI_START_ADDR   0 
#define AQ_START_ADDR  2000  

#define MB_START_ADDR   2000 
//#define MB_END_ADDR     2511 
#define MD_START_ADDR   0 
#define MW_START_ADDR   0 


typedef enum enmMemType
{
	I_TYPE = 1,
	AI_TYPE = 2,
	Q_TYPE = 3,
	AQ_TYPE = 4,
	M_TYPE = 5,
	MW_TYPE = 6,
	MD_TYPE = 7,
	S_TYPE = 8,
	SW_TYPE = 9,
	IM_TYPE = 10,
	CNST_TYPE = 11
}MEM_TYPE ;

 

#endif
