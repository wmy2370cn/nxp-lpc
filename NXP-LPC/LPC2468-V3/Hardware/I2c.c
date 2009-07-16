/***********************************************Copyright (c)*********************************************
**                                Guangzou ZLG-MCU Development Co.,LTD.
**                                     
**                                       http://www.zlgmcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			    IIC.c
** Last modified Date:      2007-10-15
** Last Version:		    1.0
** Descriptions:		    I2C函数实现，采用设备描述符来操作
**
**--------------------------------------------------------------------------------------------------------
** Created by:			    lixiaocheng
** Created date:		    2007-10-15
** Version:				    1.0
** Descriptions:		    无
**
**--------------------------------------------------------------------------------------------------------
** Modified by:			    lixiaocheng
** Modified Date:		    2007-10-26
** Version:				    1.1
** Descriptions:		    增加了I2C对UCOSII操作系统的兼容
**
*********************************************************************************************************/
#include <stdlib.h>
#include "includes.h"
#include "hd_init.h"
#include "I2cPrivate.h"
#include "I2c.h"                                                        
#include "I2cHook.h"


/*********************************************************************************************************
   定义操作I2C器件的结构体，有多个器件就需要声明多个结构体
*********************************************************************************************************/
static __I2C_INFO   __I2C0Data;
static __I2C_INFO   __I2C1Data;
static __I2C_INFO   __I2C2Data;

/*********************************************************************************************************
   是操作系统就申明信号量
*********************************************************************************************************/
OS_EVENT       *gpI2c0Sem;
OS_EVENT       *gpI2c1Sem;
OS_EVENT       *gpI2c2Sem;
OS_EVENT      **g_arrI2cTable[__IIC_MAX_NUM] = {&gpI2c0Sem, &gpI2c1Sem, &gpI2c2Sem};

/*********************************************************************************************************
   下面使用指针数组来保存结构体指针,方便对该驱动的扩展
*********************************************************************************************************/
const __PI2C_INFO   g_arrI2CInfoTab[__IIC_MAX_NUM] = {&__I2C0Data, &__I2C1Data, &__I2C2Data};

/*********************************************************************************************************
   下面定义了I2C0基地址值，如果有多个I2C器件可以在该位置添加相应的基地址即可
*********************************************************************************************************/
const INT32U        g_arrI2cBaseAddrTab[__IIC_MAX_NUM] = {I2C0_BASE_ADDR, I2C1_BASE_ADDR, I2C2_BASE_ADDR};

/*********************************************************************************************************
** Function name:           endBus
** Descriptions:            总线错误结束总线，改函数被__i2cISR调用
** Input parameters:        Parm--I2C设备描述符结构体指针
** Output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void __endBus (__PI2C_INFO Parm)
{
	            INT32U				uiOffBase;
	volatile 	INT8U 	           *pucAddrBase;
	
	pucAddrBase	= Parm->pucAddrBase;
	uiOffBase   = Parm->uiOffBase;
    
	pucAddrBase[__B_IIC_SET << uiOffBase] = 1 << 4;                     /*  总线出错，结束总线.STO      */
	pucAddrBase[__B_IIC_CLR << uiOffBase] = 0x2C;                       /*  总线出错，清零STA,SI,AA     */
	Parm->ucIICflag                       = I2C_ERR;
    __I2C_UNLOCK(*g_arrI2cTable[Parm->uiID]);                          /*  释放信号量                  */
}

/*********************************************************************************************************
** Function name:           AddrWrite
** Descriptions:            写入从机地址，改函数被__i2cISR调用
** Input parameters:        Parm--I2C设备描述符结构体指针
** Output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void __AddrWrite (__PI2C_INFO Parm)
{
	            INT32U				uiOffBase;
	volatile 	INT8U 	           *pucAddrBase;
	
	pucAddrBase	= Parm->pucAddrBase;
	uiOffBase   = Parm->uiOffBase;
	
    if (Parm->ucI2CSubAddrCtl == 1)
	{/*  指定子地址读时，先写入地址  */
		pucAddrBase[__B_IIC_DAT << uiOffBase] = (INT8U)((Parm->ucSLAddr)&0xFE);
	} 
	else
	{/*  否则直接发送从机地址        */
		pucAddrBase[__B_IIC_DAT << uiOffBase] = (Parm->ucSLAddr);
	}
	pucAddrBase[__B_IIC_CLR << uiOffBase]     = 0x28;                   /*  清除中断标志                */
}
/*********************************************************************************************************
** Function name:           dateWrite
** Descriptions:            写入数据，改函数被__subAddrWrite调用
** Input parameters:        Parm--I2C设备描述符结构体指针
** Output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void __dateWrite (__PI2C_INFO Parm)
{
	            INT32U				uiOffBase;
	volatile 	INT8U 	           *pucAddrBase;
	
	pucAddrBase	= Parm->pucAddrBase;
	uiOffBase   = Parm->uiOffBase;
	
	if (Parm->usDataNum > 0) 
	{
		pucAddrBase[__B_IIC_DAT << uiOffBase] = *Parm->pucDataBuf++;
		pucAddrBase[__B_IIC_CLR << uiOffBase] = 0x28;                   /*  清零SI，STA                 */
		Parm->usDataNum--;
	}
	else
	{
		pucAddrBase[__B_IIC_SET << uiOffBase] = 0x10;                   /*  无数据发送结束总线          */
		pucAddrBase[__B_IIC_CLR << uiOffBase] = 0x28;
		Parm->ucIICflag = I2C_WRITE_END;						        /*  设置总线结束标志            */
        __I2C_UNLOCK(*g_arrI2cTable[Parm->uiID]);                      /*  释放信号量                  */
	}
}
/*********************************************************************************************************
** Function name:           dateRead
** Descriptions:            读取数据，改函数被__i2cISR调用
** Input parameters:        Parm--I2C设备描述符结构体指针
** Output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void __dateRead (__PI2C_INFO Parm)
{
	            INT32U				uiOffBase;
	volatile 	INT8U 	           *pucAddrBase;
	
	pucAddrBase	= Parm->pucAddrBase;
	uiOffBase   = Parm->uiOffBase;
	
	*(Parm->pucDataBuf++) = pucAddrBase[__B_IIC_DAT << uiOffBase];
	Parm->usDataNum--;
	
	switch ( Parm->usDataNum ) 
	{
	
	case 0:
		pucAddrBase[__B_IIC_SET << uiOffBase] = 0x10;                   /*  结束总线                    */
		pucAddrBase[__B_IIC_CLR << uiOffBase] = 0x28;
		Parm->ucIICflag = I2C_READ_END;                                 /*  返回读取成功                */
        __I2C_UNLOCK(*g_arrI2cTable[Parm->uiID]);                       /*  释放信号量                  */
	break;
	
	case 1:                                                             /*  接收最后一个字节            */
		pucAddrBase[__B_IIC_CLR << uiOffBase] = 0x2C;                   /*  再接收到数据返回非应答位    */
	break;
	
	default:
		pucAddrBase[__B_IIC_SET << uiOffBase] = 0x04;                   /*  接收到数据，返回ACK         */
		pucAddrBase[__B_IIC_CLR << uiOffBase] = 0x28;
	break;
	}
}
/*********************************************************************************************************
** Function name:           subAddrWrite
** Descriptions:            写入从机子地址地址，改函数被__i2cISR调用
** Input parameters:        Parm--I2C设备描述符结构体指针
** Output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void __subAddrWrite (__PI2C_INFO Parm)
{
	            INT32U				uiOffBase;
	volatile 	INT8U 	           *pucAddrBase;
	
	pucAddrBase	= Parm->pucAddrBase;
	uiOffBase   = Parm->uiOffBase;

	if (Parm->ucI2CSubAddrCtl == __IIC_SUB_NONE)
	{/*  无子地址或子地址已处理      */
	    __dateWrite( Parm );                                            /*  直接发送数据                */
	                                                                
	} 
	else if (Parm->ucI2CSubAddrCtl == __IIC_SUB_R) 
	{                  /*  读操作发送子地址            */
		if(Parm->ucSubAddrNum == 2) 
		{
			pucAddrBase[__B_IIC_DAT << uiOffBase] = (INT8U)((Parm->usSubAddr >> 8) & 0xff);	
			                                                            /*  多于一个字节从机子地址      */
			                                                            /*  先发送高字节地址            */
			                                                        
			pucAddrBase[__B_IIC_CLR << uiOffBase] = 0x28;
			Parm->ucSubAddrNum--;
		}
		else if (Parm->ucSubAddrNum == 1)
		{
			pucAddrBase[__B_IIC_DAT << uiOffBase] = (INT8U)((Parm->usSubAddr) & 0xff);
			                                                            /*  发送低字节地址或单字节地址  */
			pucAddrBase[__B_IIC_CLR << uiOffBase] = 0x28;
			Parm->ucSubAddrNum--;
		} 
		else if (Parm->ucSubAddrNum == 0)
		{  /*  发送完子地址，重起总线      */	
			pucAddrBase[__B_IIC_SET << uiOffBase] = 0x20;
			pucAddrBase[__B_IIC_CLR << uiOffBase] = 0x18;
			Parm->ucI2CSubAddrCtl = __IIC_SUB_NONE;
		}
	} 
	else if (Parm->ucI2CSubAddrCtl == __IIC_SUB_W)
	{    /*  写操作发送子地址            */
		if(Parm->ucSubAddrNum == 2) 
		{
			pucAddrBase[__B_IIC_DAT << uiOffBase] = (INT8U)((Parm->usSubAddr >> 8) & 0xff);	
			                                                            /*  多于一个字节从机子地址      */
			                                                            /*  先发送高字节地址            */
			                                                        
			pucAddrBase[__B_IIC_CLR << uiOffBase] = 0x28;
			Parm->ucSubAddrNum--;
		}
		else if ( Parm->ucSubAddrNum == 1 )
		{
			pucAddrBase[__B_IIC_DAT << uiOffBase] = (INT8U)(Parm->usSubAddr & 0xff);
			                                                            /*  发送低字节地址或单字节地址  */
			                                                        
			pucAddrBase[__B_IIC_CLR << uiOffBase] = 0x28;
			Parm->ucI2CSubAddrCtl                 = __IIC_SUB_NONE;     /*  子地址已处理                */
			Parm->ucSubAddrNum--;
		} 
		else if (Parm->ucSubAddrNum == 0)
		{   /*  写数据无子地址              */
		     __dateWrite( Parm );                                       /*  直接发送数据                */
		     Parm->ucI2CSubAddrCtl                = __IIC_SUB_NONE;     /*  子地址已处理                */
		}
	}
}
/*********************************************************************************************************
** Function name:           hostRecvModeEntry
** Descriptions:            进入主接收模式，改函数被__i2cISR调用
** Input parameters:        Parm--I2C设备描述符结构体指针
** Output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void __hostRecvModeEntry (__PI2C_INFO Parm)
{
	            INT32U				uiOffBase;
	volatile 	INT8U 	           *pucAddrBase;
	
	pucAddrBase	= Parm->pucAddrBase;
	uiOffBase   = Parm->uiOffBase;
	
	if(Parm->usDataNum <= 1) 
	{
		pucAddrBase[__B_IIC_CLR << uiOffBase] = 0x2c;                   /*  接收字节少于等于1个         */
		                                                                /*  在收到下个字节时返回非ACK   */
		                                                            
	}
	else 
	{
		pucAddrBase[__B_IIC_SET << uiOffBase] = 0x04;	                /*  当接收字节多于1个时,置位AA  */
		pucAddrBase[__B_IIC_CLR << uiOffBase] = 0x28;                   /*  在收到下个字节时返回ACK     */
	}
}
/*********************************************************************************************************
** Function name:           ISR_I2C
** Descriptions:            中断服务函数底层调用，该中断服务函数没有完成从机模式
** Input parameters:        Parm--I2C设备描述符结构体指针
** Output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void __i2cISR (__PI2C_INFO Parm)
{
	            INT8U				ucSta;
	            INT32U				uiOffBase;
	volatile 	INT8U 	           *pucAddrBase;
	
	pucAddrBase	= Parm->pucAddrBase;
	uiOffBase   = Parm->uiOffBase;
	
	ucSta = pucAddrBase[__B_IIC_STAT << uiOffBase];                     /*  获取I2C状态码               */
	switch(ucSta & 0xF8) 
	{

	case __SEND_START:                                                  /*  已发送起始条件,共用写函数   */
	case __SEND_RESTART:                                                /*  重新启动总线后，发送从机地址*/
        __AddrWrite( Parm );                                            /*  写入器件地址                */
		break;
		
	case __SEND_SLA_W_ACK:												/*  已发送SLA+W并已经接收应答    */
	case __SEND_DATA_ACK:                                               /*  发送数据，已接收应答         */
	                                                                    /*  这两种情况都使用下面函数     */
        __subAddrWrite( Parm );                                         /*  I2C写数据和地址在一起        */
		break;
		
	case __SEND_SLA_W_NOACK:                                            /*  发送SLA_W,收到非应答         */
	case __SEND_DATA_NOACK:                                             /*  发送数据，收到非应答         */
	case __LOSS_BUS:                                                    /*  丢失仲裁                     */
	case __SEND_SLA_R_NOACK:                                            /*  发送SLA+R 接收非应答         */
	                                                                    /*  以上四中情况都要结束总线     */
	    __endBus( Parm );                                               /*  结束总线                     */
		break;
		
	case __SEND_SLA_R_ACK:                                              /*  发送SLA+R,收到ACK            */
		__hostRecvModeEntry( Parm );                                    /*  进入主接收模式               */
		break;
		
	case __RECV_DATA_ACK:                                               /*  接收了数据，返回应答位       */
	case __RECV_DATA_NOACK:                                             /*  接收完数据字节,返回非应答位  */
	                                                                    /*  以上两种情况，都采用读函数   */
		__dateRead( Parm );
		break;
		
	default:                                                            /*  其他状态                     */
        __endBus( Parm );                                               /*  结束总线                     */
		break;
	}
}
/*********************************************************************************************************
** Function name:           I2C0IRQ
** Descriptions:            库函数默认的中断服务函数，进行I2C中断处理,该函数挂接了钩子函数可处理用户任务
** Input parameters:        NONE
** Output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void i2c0IRQ ( void )
{
#if OS_CRITICAL_METHOD == 3                                
	OS_CPU_SR  cpu_sr;
#endif
    __OS_ENTER_CIRCT();
	__i2cISR(g_arrI2CInfoTab[0]);
	i2c0Hook();
	VICVectAddr = 0x00;                                                 /*  清除VIC中断                 */
    __OS_EXIT_CIRCT();
}

/*********************************************************************************************************
** Function name:           I2C1IRQ
** Descriptions:            库函数默认的中断服务函数，进行I2C中断处理,该函数挂接了钩子函数可处理用户任务
** Input parameters:        NONE
** Output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void i2c1IRQ ( void )
{
#if OS_CRITICAL_METHOD == 3                                
	OS_CPU_SR  cpu_sr;
#endif
    __OS_ENTER_CIRCT();
	__i2cISR(g_arrI2CInfoTab[1]);
	i2c1Hook();
	VICVectAddr = 0x00;                                                 /*  清除VIC中断                 */
    __OS_EXIT_CIRCT();
}

/*********************************************************************************************************
** Function name:           I2C2IRQ
** Descriptions:            库函数默认的中断服务函数，进行I2C中断处理,该函数挂接了钩子函数可处理用户任务
** Input parameters:        NONE
** Output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
void i2c2IRQ ( void )
{
#if OS_CRITICAL_METHOD == 3                                
	OS_CPU_SR  cpu_sr;
#endif
    __OS_ENTER_CIRCT();
	__i2cISR(g_arrI2CInfoTab[2]);
	i2c2Hook();
	VICVectAddr = 0x00;                                                 /*  清除VIC中断                 */
    __OS_EXIT_CIRCT();
}

/*********************************************************************************************************
** Function name:           I2CInit
** Descriptions:            I2c初始化程序(非字符串型)，该函数需要被封装一层的，如果设置参数的速度超过400K
**                          则设置标准速度100K
** Input parameters:        ID              器件子设备号
**                          Speed           I2C通讯速度
**                          Rsv             保留参数
** Output parameters:       NONE
** Returned value:          PIN_ERR         管脚配置错误
**                          OPERATE_FAIL    初始化失败，ID错误
**                          OPERATE_SUCCESS 初始化成功
*********************************************************************************************************/
INT32U I2CInit ( INT32U ID, INT32U Speed,  void  *Rsv )
{
	volatile INT8U *pucAddrBase;
	volatile INT32U uiOffBase;
	
	Rsv = Rsv;                                                          /*  防止编译报警                */
	/*  参数过滤,判断 */
	if ( ID >= __IIC_MAX_NUM ) 
	{
	    return OPERATE_FAIL;
	}
	
	if ( Speed > 400000 )
	{
	    Speed = 100000;
	}
	
	/*  创建信号量，操作系统部分 */
	*g_arrI2cTable[ID] = OSSemCreate(1);
	if(*g_arrI2cTable[ID] == (void *)NULL)
	{ /*  如果创建信号量失败返回错误  */
		return OPERATE_FAIL;
	}
 	 
	/*   初始化一些参数  */
	g_arrI2CInfoTab[ID]->uiID          = ID;
	g_arrI2CInfoTab[ID]->pucAddrBase   = (INT8U*)g_arrI2cBaseAddrTab[ID];
	g_arrI2CInfoTab[ID]->uiOffBase     = 2;
	g_arrI2CInfoTab[ID]->ucIICflag     = I2C_IDLE;                     /*  标识该器件现在空闲          */
	g_arrI2CInfoTab[ID]->ucSlave       = __IIC_MASTER;                 /*  标识该器件为主机            */
	g_arrI2CInfoTab[ID]->usDataNum     = 0;
	
	pucAddrBase = g_arrI2CInfoTab[ID]->pucAddrBase;	                /*  获取指针参数                */
	uiOffBase   = g_arrI2CInfoTab[ID]->uiOffBase;
	
	*(INT16U*)(pucAddrBase+(__B_IIC_SCLH << uiOffBase) ) = (INT16U)((Fpclk / Speed + 1) / 2);
	                                                                    /*  设置时钟高电平时间          */
	                                                                    
	*(INT16U*)(pucAddrBase+(__B_IIC_SCLL << uiOffBase) ) = (INT16U)((Fpclk / Speed) / 2);	
	                                                                    /*  设置时钟低电平时间          */
                                                                    
    pucAddrBase[__B_IIC_CLR << uiOffBase]  = 0x2C;
    pucAddrBase[__B_IIC_SET << uiOffBase]  = 0x40;                      /*  设置主模式                  */
    
    return OPERATE_SUCCESS;
} 
/*********************************************************************************************************
** Function name:           i2cSetMode
** Descriptions:            设置I2C速度，主要用在初始化后用户想改变I2C参数，暂时该函数只开放设置速度。
**                          如果写入速度大于400K，则设置成系统默认的100K
** Input parameters:        ID   器件子设备号,例如ID=0,表示操作的设备是I2C0
**				            arg	 设置参数的字符串，该函数暂时只设置I2C速度
**				 	             例如要设置的速度为200K，则字符串为"Speed=200000"
**				            Rsv	 保留参数
** Output parameters:       NONE
** Returned value:          OPERATE_FAIL    设置失败
**                          OPERATE_SUCCESS 设置成功
*********************************************************************************************************/
INT32U i2cSetMode (INT32U   ID,   INT32U Speed,  void   *Rsv)
{
	volatile INT8U     *pucAddrBase;
	volatile INT32U     uiOffBase;

	Rsv         = Rsv;
	pucAddrBase = g_arrI2CInfoTab[ID]->pucAddrBase;
	uiOffBase   = g_arrI2CInfoTab[ID]->uiOffBase;

	if (ID < __IIC_MAX_NUM) 
	{
		/*  参数过滤 */
		if (Speed > 400000)
		{
		    Speed = 100000;
		}
		
    	__I2C_LOCK(*g_arrI2cTable[ID]);                                /*  申请I2C资源                 */
    	
		                                                                /*  设置速度                    */
		*(INT16U*)(pucAddrBase + (__B_IIC_SCLH << uiOffBase)) = (INT16U)((Fpclk / Speed + 1) / 2);		
		*(INT16U*)(pucAddrBase + (__B_IIC_SCLL << uiOffBase)) = (INT16U)((Fpclk / Speed) / 2);
				
        __I2C_UNLOCK(*g_arrI2cTable[ID]);                              /*  释放I2C资源                 */  
            	
		return OPERATE_SUCCESS;
	}
	return OPERATE_FAIL;
}

/*********************************************************************************************************
** Function name:           __i2cStart
** Descriptions:            启动I2C总线
** Input parameters:        ID   器件子设备号,例如ID=0,表示操作的设备是I2C0
** Output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
static void __i2cStart (INT32U ID)
{
	volatile INT8U     *pucAddrBase;
	volatile INT32U     uiOffBase;
	
	if (g_arrI2CInfoTab[ID]->ucSlave == __IIC_SLAVER) 
	{
	
        __I2C_UNLOCK(*g_arrI2cTable[ID]);                              /*  释放I2C资源                 */    	
	    return;                                                         /*  如果是从机就不启动总线      */
	 }
	 
	pucAddrBase = g_arrI2CInfoTab[ID]->pucAddrBase;
	uiOffBase   = g_arrI2CInfoTab[ID]->uiOffBase;
	
	g_arrI2CInfoTab[ID]->ucIICflag = I2C_BUSY;
	pucAddrBase[__B_IIC_CLR << uiOffBase] = 0x2C;                       /*  启动总线,设置为主机         */
	pucAddrBase[__B_IIC_SET << uiOffBase] = 0x60;
}

/*********************************************************************************************************
** Function name:           i2cRead
** Descriptions:            I2c读数据函数
** Input parameters:        ID			   : I2C设备标识
**                          Addr		   : 从机地址
**                          Ret 		   : 指向返回数据存储位置的指针	
**                          Sub_Addr	   : 从机内部子地址
**                          Sub_Addr_NByte : 从机内部子地址类型 ---- 1: 单字节地址; 2: 双字节地址; 
**                          ReadNbyte      : 需要读数据个数
** Output parameters:       NONE
** Returned value:          操作成功       : OPERATE_SUCCESS
**                          操作失败       : OPERATE_FAIL
*********************************************************************************************************/
INT32U i2cRead (INT32U ID,
               INT8U  Addr, 
               INT8U *Ret, 
               INT16U Sub_Addr, 
               INT8U  Sub_Addr_NByte, 
               INT16U ReadNbyte)
{
	if (ReadNbyte == 0) 
	{
	    return OPERATE_FAIL;					                        /*  不允许读取0个字节           */
	}
		
	if (ID < __IIC_MAX_NUM) 
	{
         __I2C_LOCK(*g_arrI2cTable[ID]);                               /*  申请信号量                  */	
		if (g_arrI2CInfoTab[ID]->ucIICflag == I2C_BUSY)
		{		
		    __I2C_UNLOCK(*g_arrI2cTable[ID]);                          /*  释放I2C资源                 */
		    return OPERATE_FAIL;                                        /*  忙，就返回错误              */
		}
		g_arrI2CInfoTab[ID]->ucSLAddr          = (INT8U)(Addr | 0x01); /*  从机地址                    */
		g_arrI2CInfoTab[ID]->usSubAddr         = Sub_Addr;             /*  从机地址子地址              */
		g_arrI2CInfoTab[ID]->ucSubAddrNum      = Sub_Addr_NByte;       /*  从机子地址数量              */
		g_arrI2CInfoTab[ID]->ucI2CSubAddrCtl   = __IIC_SUB_R;          /*  设置为读操作                */
		g_arrI2CInfoTab[ID]->pucDataBuf        = Ret;                  /*  保存数据的缓冲区            */
		g_arrI2CInfoTab[ID]->usDataNum         = ReadNbyte;            /*  要读取的数据个数            */
		__i2cStart(ID);
		return OPERATE_SUCCESS;
	}
	return OPERATE_FAIL;
}

/*********************************************************************************************************
** Function name:           i2cWrite
** Descriptions:            I2c写数据函数
** Input parameters:        ID			   : I2C设备标识
**                          Addr		   : 从机地址
**                          Buf 		   : 指向写入数据存储位置的指针	
**                          Sub_Addr	   : 从机内部子地址
**                          Sub_Addr_NByte : 从机内部子地址类型 
**                                              0: 无内部子地址；
**                                              1: 单字节地址; 
**                                              2: 双字节地址; 
**                          Nbyte          : 写入数据长度
** Output parameters:       NONE
** Returned value:          操作成功       : OPERATE_SUCCESS
**                          操作失败       : OPERATE_FAIL
*********************************************************************************************************/
INT32U i2cWrite (INT32U  ID,
                INT8U   Addr, 
                INT8U  *Buf, 
                INT16U  Sub_Addr, 
                INT8U   Sub_Addr_NByte, 
                INT16U  Nbyte)
{
	if (Nbyte == 0) 
	{
	    return OPERATE_FAIL;					                        /*  不允许写入0个字节           */
	}
     	
	if (ID < __IIC_MAX_NUM)
	{
         __I2C_LOCK(*g_arrI2cTable[ID]);                               /*  申请信号量                  */
		if (g_arrI2CInfoTab[ID]->ucIICflag == I2C_BUSY) 
		{	
		     __I2C_UNLOCK(*g_arrI2cTable[ID]);                         /*  释放I2C资源                 */
		    return OPERATE_FAIL;
		}
    	
		g_arrI2CInfoTab[ID]->ucSLAddr        = (INT8U)(Addr & 0xfe);   /*  从机地址                    */
		g_arrI2CInfoTab[ID]->usSubAddr       = Sub_Addr;               /*  从机地址子地址              */
		g_arrI2CInfoTab[ID]->ucSubAddrNum    = Sub_Addr_NByte;         /*  从机子地址数量              */
		g_arrI2CInfoTab[ID]->ucI2CSubAddrCtl = __IIC_SUB_W;            /*  设置为写操作                */
		g_arrI2CInfoTab[ID]->pucDataBuf      = Buf;					/*  保存数据的缓冲区            */
		g_arrI2CInfoTab[ID]->usDataNum       = Nbyte;                  /*  要写入的数据个数            */
		__i2cStart(ID);
		return OPERATE_SUCCESS;
	}
	return OPERATE_FAIL;
}
/*********************************************************************************************************
** Function name:           i2cGetFlag
** Descriptions:            返回I2C器件当前状态如果通信失败，则表示上次总线操作失败，
**                          但是现在总线仍然是空闲的,空闲表示上次通信成功或没有进行过通信
**                          现在总线是空闲的，可以启动总线通信
** Input parameters:        ID             : I2C设备标识
** Output parameters:       NONE
** Returned value:          操作成功       : 返回当前I2C总线状态
**                                           I2C_IDLE： 表示总线空闲
**                                           I2C_WRITE_END：表示写操作成功
**                                           I2C_READ_END：表示读操作成功
**                                           I2C_ERR：表示总线操作失败
**                                           I2C_BUSY：表示总线正在工作中
**                          操作失败       : OPERATE_FAIL
*********************************************************************************************************/
INT32U i2cGetFlag (INT8U ID)
{   
	if (ID < __IIC_MAX_NUM)
	{    
	    __I2C_LOCK(*g_arrI2cTable[ID]);                                /*  先申请资源看总线操作是否结束*/ 
        __I2C_UNLOCK(*g_arrI2cTable[ID]);                              /*  释放I2C资源                 */    	
	    
		return g_arrI2CInfoTab[ID]->ucIICflag;
	}
	return OPERATE_FAIL;
}
/*********************************************************************************************************
** Function name:           i2cGetRemainBytes
** Descriptions:            返回I2C器件还有多少字节数据没有发送完毕
** Input parameters:        ID             : I2C设备标识
** Output parameters:       NONE
** Returned value:          操作成功       : 返回当前还剩没有发送的字节数
**                          操作失败       : OPERATE_FAIL
*********************************************************************************************************/
INT32U i2cGetRemainBytes(INT8U ID)
{
	if (ID < __IIC_MAX_NUM) 
	{
		return g_arrI2CInfoTab[ID]->usDataNum;
	}
	return OPERATE_FAIL;
}
/*********************************************************************************************************
   END FILE
*********************************************************************************************************/

