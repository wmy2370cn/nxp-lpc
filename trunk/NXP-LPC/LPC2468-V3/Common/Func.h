/****************************************Copyright (c)**************************************************
**                                         公共模块
**                                      
**                                      
**
**                           
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名:  
**创   建   人:  
**最后修改日期: 
**描        述:  一些通用函数
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
#ifndef _FUNC_H
#define _FUNC_H
 

extern const INT8U ORBIT_ARRAY[8];
extern const INT8U CLRBIT_ARRAY[8];

#define OrBit(bias)   ORBIT_ARRAY[bias]			//取相应位值
#define ClrBit(bias)  CLRBIT_ARRAY[bias]		//清相应位值

//CRC16校验函数
INT16U crc16(INT8U * buf,INT32U len);
void crc16_ex(INT8U * buf, INT32U len, INT16U * crc);

INT16U GetCRC16(INT8U *Buff_addr,INT16U len);

#define aabs(a) (a)>0?(a):(0-(a))

//带四舍五入的除法
#define RoundDiv(a,b) (((a)/(b))+((a)%(b))*2/(b))

#ifndef max
#	define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#	define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#define max3(a,b,c) max(max((a),(b)),(c))
#define min3(a,b,c) min(min((a),(b)),(c))

#ifndef MAKEWORD
#	define MAKEWORD(a, b)      ((INT16U)(((INT8U)(a)) | ((INT16U)((INT8U)(b))) << 8))
#endif

#ifndef MAKEDWORD
#	define MAKEDWORD(a, b)      ((INT32U)(((INT16U)(a)) | ((INT32U)((INT16U)(b))) << 16))
#endif

#ifndef LOWORD
#	define LOWORD(l)           ((INT16U)(l))
#endif

#ifndef HIWORD
#	define HIWORD(l)           ((INT16U)(((INT32U)(l) >> 16) & 0xFFFF))
#endif

#ifndef LOBYTE
#	define LOBYTE(w)           ((INT8U)(w))
#endif

#ifndef HIBYTE
#	define HIBYTE(w)           ((INT8U)(((INT16U)(w) >> 8) & 0xFF))
#endif

//将BCD码装换为16进制实际值
#define BCD2HEX(b) ((((b)>>4)&0xF)*10+((b)&0xF))
#define HEX2BCD(b) ((((INT8U)(b))/10*0x10)+((INT8U)(b))%10)
  

#endif
