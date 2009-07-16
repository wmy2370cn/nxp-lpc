
#include "includes.h"
#include "IAP.h" 

//#define		FLASHADDR			0x00050000				/*片内FLASH地址*/
#define  LPC_2468

#ifdef LPC_2468
#define		MAX_FLASH_ADDR		0x0007DFFF				/*最大访问地址上限*/
#else
#define		MAX_FLASH_ADDR		0x0003FFFF				/*最大访问地址上限*/
#endif // LPC_2468 

/* 定义CCLK值大小，单位为KHz */
#define		IAP_FCCLK	48000

#define 	IAP_ENTER_ADR   0x7FFFFFF1              //  IAP入口地址定义

static INT32U	paramin[8];		                            // IAP入口参数缓冲区
static INT32U	paramout[8];	                            // IAP出口参数缓冲区

//__align(4) 	INT8U TempFile[MAX_FILE_LEN];		
//__align(4)	INT8U Sector_1_File[SECTOR_1_LEN];
//__align(4)	INT8U Sector_2_File[SECTOR_2_LEN];

/*********************************************************************************************************
**函数名称:  SelSector
**函数功能:  IAP操作缓冲区选择，代码为50
**入口参数:  sec1	起始扇区
**			 sec2	终止扇区
**出口参数:  IAP 操作状态码
**			 IAP返回值（paramout缓冲区）
********************************************************************************************************/
INT32U	SelSector(INT8U	sec1,INT8U	sec2)
{
	paramin[0] = IAP_SELECTOR;	                    // 设置命令字
	paramin[1] = sec1;			                    // 设置参数
	paramin[2] = sec2;
	(*(void(*)())IAP_ENTER_ADR)(paramin,paramout);	// 调用IAP服务程序
	return(paramout[0]);	                        // 返回状态码
}
/*********************************************************************************************************
**函数名称:  RamToFlash
**函数功能:  复制RAM的数据到FLASH，命令代码51
**入口参数:  dst		目标地址，即FLASH起始地址，以256字节为分界
**			 src		源地址，即RAM地址，地址必须字对其
**			 no		    复制字节个数，为256/512/1024/4096
**出口参数:  IAP 操作状态码
**			 IAP返回值（paramout缓冲区）
********************************************************************************************************/
INT32U	RamToFlash(INT32U dst, INT32U src, INT32U  no)
{
	paramin[0] = IAP_RAMTOFLASH;	                // 设置命令字
	paramin[1] = dst;				                // 设置参数
	paramin[2] = src;
	paramin[3] = no;
	paramin[4] = IAP_FCCLK;
	(*(void(*)())IAP_ENTER_ADR)(paramin,paramout);	// 调用IAP服务程序
	return(paramout[0]);			                // 返回状态码
}
/*********************************************************************************************************
**函数名称：EraseSector
**函数功能：擦除扇区，命令代码52
**入口参数：sec1	起始扇区
**			sec2	终止扇区
**出口参数：IAP 操作状态码
**			IAP返回值（paramout缓冲区）
********************************************************************************************************/
INT32U	EraseSector(INT32U sec1, INT32U sec2)
{
	paramin[0] = IAP_ERASESECTOR;	                // 设置命令字
	paramin[1] = sec1;				                // 设置参数
	paramin[2] = sec2;
	paramin[3] = IAP_FCCLK;
	(*(void(*)())IAP_ENTER_ADR)(paramin,paramout);	// 调用IAP服务程序
	return(paramout[0]);			                // 返回状态码
}
/*********************************************************************************************************
**函数名称:  BlankCHK
**函数功能:  查空扇区，命令代码53
**入口参数:  sec1	起始扇区
**			 sec2	终止扇区
**出口参数:  IAP 操作状态码
**			 IAP返回值（paramout缓冲区）
********************************************************************************************************/
INT32U	BlankCHK(INT32U	sec1, INT32U  sec2)
{
	paramin[0] = IAP_BLANKCHK;	                    // 设置命令字
	paramin[1] = sec1;				                // 设置参数
	paramin[2] = sec2;
	paramin[3] = IAP_FCCLK;
	(*(void(*)())IAP_ENTER_ADR)(paramin,paramout);	// 调用IAP服务程序
	return(paramout[0]);			                // 返回状态码
}
/*********************************************************************************************************
**函数名称:  ReadParID
**函数功能:  读器件ID，命令代码54
**入口参数:  器件ID地址指针
**出口参数:  IAP 操作状态码
**			 IAP返回值（paramout缓冲区）
********************************************************************************************************/
INT32U	ReadParID(INT32U *Device_ID)
{
	paramin[0] = IAP_READPARTID;	                // 设置命令字
	(*(void(*)())IAP_ENTER_ADR)(paramin,paramout);	// 调用IAP服务程序
	*Device_ID = paramout[1];
	return(paramout[0]);			                 // 返回状态码
}
/*********************************************************************************************************
**函数名称:  BootCodeID
**函数功能:  读boot代码ID，命令代码55
**入口参数:  boot代码ID地址指针
**出口参数:  IAP 操作状态码
**			 IAP返回值（paramout缓冲区）
********************************************************************************************************/
INT32U	BootCodeID(INT32U *Boot_ID)	
{
	paramin[0] = IAP_BOOTCODEID;	                // 设置命令字
	(*(void(*)())IAP_ENTER_ADR)(paramin,paramout);	// 调用IAP服务程序
	*Boot_ID = paramout[1];
	
	return(paramout[0]);			                // 返回状态码
}
/*********************************************************************************************************
**函数名称:  Compare
**函数功能:  校验数据，命令代码56
**入口参数:  dst		目标地址，即RAM/FLASH起始地址，地址必须字对齐
**			 src	 	源地址，即RAM/RAM地址，地址必须字对齐
**			 no		    比较字节个数，必须能被4整除
**出口参数:  IAP 操作状态码
**			 IAP返回值（paramout缓冲区）
********************************************************************************************************/
INT32U	Compare(INT32U	dst, INT32U	src, INT32U	no)
{
	paramin[0] = IAP_COMPARE;	                    // 设置命令字
	paramin[1] = dst;				                // 设置参数
	paramin[2] = src;
	paramin[3] = no;
	(*(void(*)())IAP_ENTER_ADR)(paramin,paramout);	// 调用IAP服务程序
	return(paramout[0]);			                // 返回状态码
}
/*********************************************************************************************************
**函数名称:  Reinvoke_ISP
**函数功能:  重新调用ISP，命令代码57。
**入口参数:  无
**出口参数:  无
********************************************************************************************************/
void  Reinvoke_ISP(void)
{
	paramin[0] = IAP_REINVOKE_ISP;					//  设置命令字
	(*(void(*)())IAP_ENTER_ADR)(paramin,paramout);	//  重新调用ISP
} 
/*********************************************************************************************************
**函数名称:  WriteFlash
**函数功能:  向FLASH中写入数据
**入口参数:  dst		目标地址，即FLASH起始地址，地址必须以256字节对齐，即地址的低8位必须为0
**			 src		源地址，即RAM地址，地址必须字对齐，即地址的低两位必须为0
**			 no		    写入字节数，必须为256/512/1024/4096
**出口参数:  Compare的返回值，操作成功正确返回0
********************************************************************************************************/
INT8U	WriteFlash(INT32U  dst, INT32U	src, INT32U	no)
{
	SelSector((dst/0x1000),(dst+no)/0x1000);	// 选择扇区
	EraseSector((dst/0x1000),(dst+no)/0x1000);	// 擦除扇区
	BlankCHK((dst/0x1000),(dst+no)/0x1000);		// 查空扇区
	
	SelSector((dst/0x1000),(dst+no)/0x1000);	// 选择扇区
	
	RamToFlash(dst,src,no);			            // 写数据到FLASH
	return(Compare(dst,src,no));	            // 比较数据
}
/*
void IAP_WriteFlash(void)
{

	int i;
	for(i=0;i<256;i++)
	{
		SendData[i] = i;
	}
	SelSector(17, 17);					// 选择扇区
	EraseSector(17, 17);				// 擦除扇区
	SelSector(17, 17);					// 选择扇区
	RamToFlash(0x00050000, (INT32U)SendData, 1024);	// 写数据到FLASH
	
}
*/
/*-------------------------------------------------------*/
// 函数名称：FLASH_WriteStr
// 输入参数：INT16U StartAddr：FLASH内存地址, 
//			 INT8U * buff：存放数据指针, INT16U len：数据长度	
// 输出参数：
// 功能描述：写数据到FLASH
/*-------------------------------------------------------*/
// INT8U FLASH_WriteStr(INT16U StartPos, INT8U * buff, INT16U len)
// {
// 	INT16U	i;	
// 	for(i=0;i<MAX_FILE_LEN;i++)			//读取原存储的文件数据
// 	{
// 		TempFile[i] = *(volatile unsigned long *)(FLASHADDR+i); 		
// 	}	
// 				for(i=0;i<len;i++)					//更新对应位置数据
// 	{
// 		TempFile[StartPos+i] = buff[i];
// 	}
// 	
// 	for(i=0;i<SECTOR_1_LEN;i++)			//数据区1
// 	{
// 		Sector_1_File[i] = TempFile[i];
// 	}
// 		
// 	for(i=0;i<SECTOR_2_LEN;i++)			//数据区2
// 	{
// 		Sector_2_File[i] = TempFile[SECTOR_1_LEN+i];
// 	}
// 	
// 	OS_ENTER_CRITICAL();	
// 	SelSector(17, 17);					// 选择扇区
// 	EraseSector(17, 17);
// 	SelSector(17, 17);					// 选择扇区
// 	RamToFlash(SECTOR_1_DESTADDR, (INT32U)Sector_1_File, SECTOR_1_LEN);	// 写数据区1到FLASH	
// 	RamToFlash(SECTOR_2_DESTADDR, (INT32U)Sector_2_File, SECTOR_2_LEN);	// 写数据区2到FLASH	
// 	OS_EXIT_CRITICAL();		
// 	return	TRUE;
// }

/*-------------------------------------------------------*/
// 函数名称：FLASH_ReadByte
// 输入参数: INT16U StartAddr：FLASH内存地址, 
//			void * buff：存放数据指针, INT16U len：数据长度			
// 输出参数：错误返回FALSE，正确返回TURE
// 功能描述：从FLASH读取一串数据
/*-------------------------------------------------------*/
INT8U FLASH_ReadByte(INT32U StartPos, INT8U * buff, INT16U len)
{
	INT32U	i;
	if(StartPos+len>MAX_FLASH_ADDR)
	{
		return FALSE;
	}	 
	
	for(i=0;i<len;i++)
	{
		buff[i] = (*(volatile unsigned long *)(StartPos+i)); 
	}

	return	TRUE;
}
